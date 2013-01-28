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

// Variables
int width     = 50;
int height    = 10;
int direction = KEY_RIGHT;

char TOKEN_SNAKE = 'S';

// Methods
void setup();
void tear_down();
int  process_input();
void move_snake();
void frame();
void run();

#endif /* SNAKE_H_ */
