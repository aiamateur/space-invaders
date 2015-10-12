#ifndef GAMESCREEN_H_
#define GAMESCREEN_H_

#include <stdio.h>
#include <stdint.h>

#include "../globals.h"
#include "../screen/screen.h"
#include "../text/text.h"

/** Floor static game element **/
#define FLOOR_COLOR		SCREEN_COLOR_GREEN
#define FLOOR_ORIGIN_X	0
#define FLOOR_ORIGIN_Y	(SCREEN_HEIGHT - (5*SCREEN_EDGE_BUFFER)/3)
#define FLOOR_WIDTH		32
#define FLOOR_HEIGHT	2
#define FLOOR_SCALE		1

/** Score static game element **/
// the text
#define SCORE_TXT_COLOR		SCREEN_COLOR_WHITE
#define SCORE_TXT_ORIGIN_X	SCREEN_EDGE_BUFFER
#define SCORE_TXT_ORIGIN_Y	SCREEN_EDGE_BUFFER
#define SCORE_TXT_WIDTH		30
#define SCORE_TXT_HEIGHT	5
#define SCORE_TXT_SCALE		2
// the number
#define SCORE_VAL_COLOR		SCREEN_COLOR_GREEN
#define SCORE_VAL_ORIGIN_X	(SCORE_TXT_ORIGIN_X+SCORE_TXT_WIDTH*SCORE_TXT_SCALE) + 10
#define SCORE_VAL_ORIGIN_Y	SCREEN_EDGE_BUFFER
#define SCORE_VAL_WIDTH		5
#define SCORE_VAL_HEIGHT	5
#define SCORE_VAL_PADDING	0
#define SCORE_VAL_SCALE		2
// Max score
#define SCORE_VAL_MAX_LEN	6
#define SCORE_VAL_MAX		999999

/** Lives static game element **/

// Initialize the screen by drawing all the
// static elements (floor, lives, score)
void gameScreen_init();

void gameScreen_setScore(uint16_t score);


#endif /* GAMESCREEN_H_ */