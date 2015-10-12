#include "gameScreen.h"

void drawFloor();
void drawScore();
void drawLives();
void gameScreen_getScoreAsArray(uint16_t score, uint16_t *scoreArray, uint16_t *scoreLength);

// ----------------------------------------------------------------------------

void gameScreen_init() {
	drawFloor();
	drawScore();

	// draw a '0' next to "SCORE"
	gameScreen_setScore(0);
}

void gameScreen_setScore(uint16_t score) {
	point_t origin = { .x = SCORE_VAL_ORIGIN_X, .y = SCORE_VAL_ORIGIN_Y };
	symbolsize_t size = { .w = SCORE_VAL_WIDTH, .h = SCORE_VAL_HEIGHT };
	uint8_t i = 0;
	uint16_t scoreArray[SCORE_VAL_MAX_LEN] = { 0 };
	uint16_t scoreLength = 0;

	// Get score info as an array so we can loop through each digit
	gameScreen_getScoreAsArray(score, scoreArray, &scoreLength);

	for (i=0; i<scoreLength; i++) {
		uint16_t digit = scoreArray[i];
		screen_drawSymbol(numbers_5x5[digit], origin, size, SCORE_VAL_SCALE, SCORE_VAL_COLOR);

		// Shift the origin over to the next spot
		origin.x += (SCORE_VAL_WIDTH*SCORE_VAL_SCALE) + SCORE_VAL_PADDING;
	}
}

// ----------------------------------------------------------------------------
// Private Helper Methods
// ----------------------------------------------------------------------------

void drawFloor() {
	point_t origin = { .x = FLOOR_ORIGIN_X, .y = FLOOR_ORIGIN_Y };
	symbolsize_t size = { .w = FLOOR_WIDTH, .h = FLOOR_HEIGHT };

	uint8_t i = 0;
	for (i=0; i<(SCREEN_WIDTH/FLOOR_WIDTH); i++) {
		screen_drawSymbol(floor_32x2, origin, size, FLOOR_SCALE, FLOOR_COLOR);

		// shift the origin over a bit
		origin.x += FLOOR_WIDTH*FLOOR_SCALE;
	}

}

// ----------------------------------------------------------------------------

void drawScore() {
	point_t origin = { .x = SCORE_TXT_ORIGIN_X, .y = SCORE_TXT_ORIGIN_Y };
	symbolsize_t size = { .w = SCORE_TXT_WIDTH, .h = SCORE_TXT_HEIGHT };

	// Draw the text "SCORE"
	screen_drawSymbol(word_score_30x5, origin, size, SCORE_TXT_SCALE, SCORE_TXT_COLOR);
}

// ----------------------------------------------------------------------------

void drawLives() {


}

// ----------------------------------------------------------------------------

void gameScreen_getScoreAsArray(uint16_t score, uint16_t *scoreArray, uint16_t *scoreLength) {

	uint16_t tmpArray[SCORE_VAL_MAX_LEN];
	uint16_t count = 0;
	int8_t i = (SCORE_VAL_MAX_LEN-1); // A backward counter to tell us which digit

	if (score == 0) {
		scoreArray[0] = 0;
		*scoreLength = 1;
		return;
	}

	while(score) { // while there's still a score to explode
		tmpArray[i] = (score % 10);
		score /= 10;
		i--;
		count++;
	}

	uint8_t j = 0;
	for (j=0; j<count; j++) {
		scoreArray[j] = tmpArray[i++];
	}



	*scoreLength = count;
//	scoreArray[0] = 1;
//	scoreArray[1] = 9;
}