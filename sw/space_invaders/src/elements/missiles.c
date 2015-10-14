#include "missiles.h"

// some missiles "wiggle" as they move across the screen.
// This global defines which state we're in.
static bool wobble;

// function declarations
void shiftMissileOrigin(missile_t* missile);
bool missiles_inBounds(missile_t* missile);
void missiles_impact(missile_t* missile);
void missile_bunker_impact(missile_t* missile);
void missile_missile_impact(missile_t* missile);
void missile_alien_impact(missile_t* missile);
void missile_tank_impact(missile_t* missile);
void missiles_deactivate(missile_t* missile);
void missiles_check_impact(missile_t* missile);
bool missile_in_block(missile_t* missile, point_t target_origin, uint16_t target_width, uint16_t target_height);
bool missile_kill_alien_in_row(missile_t* missile, uint16_t row);

//-----------------------------------------------------------------------------

void missiles_init() {
	uint8_t i;

	for (i=0; i<MISSILE_COUNT; i++) {
		// Turn off all missiles
		missile_array[i].active = false;
	}
}

//-----------------------------------------------------------------------------

// update the position of all missiles
void missiles_move(){
	uint8_t i;
	wobble = !wobble;
	for(i=0; i<MISSILE_COUNT; i++){
		missile_t* missile = &missile_array[i];
		if (missile->active) {
			int8_t dir = missile->up ? -1 : 1;
			const uint32_t* symbol = wobble ? missile->symbol_r : missile->symbol_l;

			// shift it on the screen
			screen_shiftElement(symbol, missile->origin, missile->size,
								0, dir*MISSILE_SHIFT, MISSILE_SCALE, MISSILE_COLOR);

			// now update the origin in the missile struct
			shiftMissileOrigin(missile);

			// Should we keep drawing this missile?
			if (missiles_inBounds(missile)) {
				missiles_check_impact(missile);
			} else {
				missiles_deactivate(missile);
			}

		}
	}
}

//-----------------------------------------------------------------------------

// fire the tank missile
void missiles_tankFire(){
	missile_t *missile = &missile_array[TANK_MISSILE];

	if (!missile->active){ // no tank missile on screen

		missile->active = true; // set status to active
		point_t tank_gun_origin = tank_get_origin(); // get tank location
		// compute the coordinates of the cannon
		tank_gun_origin.x += (TANK_WIDTH*TANK_SCALE)/2 - (missile->size.w*MISSILE_SCALE)/2;
		tank_gun_origin.y -= MISSILE_HEIGHT*MISSILE_SCALE;

		missile->origin = tank_gun_origin;
		// draw the missile at the tip of the tank's cannon
		screen_drawSymbol(missile->symbol_r, missile->origin, missile->size,
				MISSILE_SCALE, SCREEN_COLOR_WHITE);
	}
}

//-----------------------------------------------------------------------------

// initiate an alien missile
bool missiles_alienFire(uint16_t x, uint16_t y) {
	uint8_t i = 0;

	// First off, make sure that alien is even living
	if (!alien_isAlive(x,y)) return false;

	// start at i=1 (the tank is 0)
	for(i=1; i<MISSILE_COUNT; i++){
		// find a missile that is not already active
		if (!missile_array[i].active) {

			// Get the given aliens origin
			point_t alienOrigin = aliens_getAlienOrigin(x, y);

			// transform alien origin into the origin/tip of the missile
			alienOrigin.y += (ALIEN_HEIGHT*ALIEN_SCALE);
			alienOrigin.x += (ALIEN_WIDTH*ALIEN_SCALE)/2;
			missile_array[i].origin = alienOrigin;

			// draw the symbol to the screen
			screen_drawSymbol(missile_array[i].symbol_r, missile_array[i].origin,
					missile_array[i].size, MISSILE_SCALE, MISSILE_COLOR);

			missile_array[i].active = true; // set missile status to active

			return true;
		}
	}

	return true;
}

//-----------------------------------------------------------------------------
// Private Helper Methods
//-----------------------------------------------------------------------------

// update the missile origin (shift it in the y direction)
void shiftMissileOrigin(missile_t* missile) {
	int8_t dir = (missile->up) ? -1 : 1;
	missile->origin.y += dir*MISSILE_SHIFT*MISSILE_SCALE;
}

//-----------------------------------------------------------------------------

void missiles_erase(missile_t* missile){
	const uint32_t* symbol = wobble ? missile->symbol_r : missile->symbol_l;
	screen_drawSymbol(symbol, missile->origin,
						missile->size, MISSILE_SCALE, SCREEN_COLOR_BLACK);
}

//-----------------------------------------------------------------------------

bool missiles_inBounds(missile_t* missile){
	bool outOfBounds = (missile->origin.y > MISSILE_BOTTOM_BUFFER ||
						missile->origin.y < MISSILE_TOP_BUFFER);
	return !outOfBounds;
}

//-----------------------------------------------------------------------------

void missiles_deactivate(missile_t* missile){
	missiles_erase(missile);
	missile->active = false;
}

//-----------------------------------------------------------------------------

point_t missiles_get_tip(missile_t* missile){
	point_t origin;
	uint16_t x = missile->origin.x;
	uint16_t y = missile->origin.y;
	origin.x = x + 1;
	origin.y = (missile->up) ? y-1 : y + missile->size.h*MISSILE_SCALE;
//	int sym = 1;
//	symbolsize_t size = {.w=1,.h=1};
//	screen_drawSymbol(&sym, origin, size, 1, SCREEN_COLOR_RED);
	return origin;
}

//-----------------------------------------------------------------------------

void missiles_check_impact(missile_t* missile){
	if(missile->up){
		// Then it's a tank's missile
		missile_bunker_impact(missile);
		missile_alien_impact(missile);
	} else {
		// Then it's an alien's missile
		missile_tank_impact(missile);
		missile_bunker_impact(missile);
	}
}

//-----------------------------------------------------------------------------

void missile_missile_impact(missile_t* missile){

}

//-----------------------------------------------------------------------------

void missile_bunker_impact(missile_t* missile){
	int i, j;
	for(i=0; i<BUNKER_COUNT; i++){
		bunker_t bunker = bunkers_get_bunker(i);
		if(missile_in_block(missile, bunker.origin, bunker.size.w*BUNKER_SCALE, bunker.size.h*BUNKER_SCALE)){
			xil_printf("missile in bunker\n\r");
			for(j=0; j<BUNKER_SUB_ORIGIN_COUNT; j++){
//				xil_printf("sub point (%d, %d), bunker point (%d, %d)\n\r", bunker.sub_points[i].x, bunker.sub_points[i].y,
//						bunker.origin.x, bunker.origin.y);
				if(missile_in_block(missile, bunker.sub_points[j], BUNKER_SUB_ORIGIN_WIDTH, BUNKER_SUB_ORIGIN_HEIGHT)){
					xil_printf("in sub block\n\r");
					if(!bunker_point_eroded(i,j)){
						bunkers_damage(i, j);
						missiles_deactivate(missile);
						return;
					}

				}
			}
		}

	}
}

//-----------------------------------------------------------------------------

void missile_alien_impact(missile_t* missile){
	uint16_t y;
	uint16_t height = missile->origin.y;

	for(y=0; y<ALIEN_ROW_COUNT; y++){
		int16_t y_dist = height - aliens_getAlienOrigin(0,y).y;
		if(y_dist <= ALIEN_HEIGHT*ALIEN_SCALE){
			if(missile_kill_alien_in_row(missile, y)){
				return;
			}
		}
	}
}

//-----------------------------------------------------------------------------

bool missile_kill_alien_in_row(missile_t* missile, uint16_t row){
	point_t alien_origin;
	uint16_t x;
	for(x=0; x<ALIEN_COL_COUNT; x++){
		alien_origin = aliens_getAlienOrigin(x,row);
		if(!alien_isAlive(x, row)){
			continue;
		}
		bool hit = missile_in_block(missile, alien_origin, ALIEN_WIDTH*ALIEN_SCALE, ALIEN_HEIGHT*ALIEN_SCALE);
		if(hit){
			uint16_t index = alien_xy_to_index(x, row);
			missiles_deactivate(missile);
			aliens_kill(index);
			return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------

void missile_tank_impact(missile_t* missile){
	point_t tank_origin = tank_get_origin();
	int dist = (missile->origin.x - tank_origin.x);
	if(dist < (TANK_WIDTH*TANK_SCALE) && dist > 0){
		tank_kill();
	}
}

//-----------------------------------------------------------------------------

bool missile_in_block(missile_t* missile, point_t target_origin, uint16_t target_width, uint16_t target_height){
	bool up = missile->up;
	int x_dist = (missile->origin.x - target_origin.x);
	int y_dist = up ? (missile->origin.y - target_origin.y) : (target_origin.y - missile->origin.y);

	if (x_dist < target_width && x_dist > 0 && y_dist < target_height && y_dist > -MISSILE_HEIGHT*MISSILE_SCALE){
		return true;
	}
	else{
		return false;
	}
}

//-----------------------------------------------------------------------------
