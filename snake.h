// +----------------------------------------------------------------+
// | cSnake                                                         |
// +----------------------------------------------------------------+

/**
 * Port of my python snake to c - memory leaks included :)
 *
 * @author  Christopher Marchfelder <marchfelder@googlemail.com>
 * @license GPL
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include "globals.h"

// Variables
int   width      = 50;
int   height     = 10;
int   direction  = KEY_RIGHT;
int   pause_game = 0;
int   game_over  = 0;
int   run_game   = 1;
int   score      = 0;
float frame_rate = 400.0;

char TOKEN_SNAKE = 'S';
char TOKEN_APPLE = 'A';

// For the screen update
struct timeb last_update, now;
float diff;

// Methods
void setup();
void tear_down();
void run();

void spawn_apple();
void move_snake();
void frame();

void process_input();
int  collides();
int  eats_apple();
void recalculate_framerate();


#endif /* SNAKE_H_ */
