// +----------------------------------------------------------------+
// | cSnake                                                         |
// +----------------------------------------------------------------+

/**
 * Port of my python snake to c - memory leaks included :)
 *
 * @author  Christopher Marchfelder <marchfelder@googlemail.com>
 * @license GPL
 */
#ifndef DRAW_H_
#define DRAW_H_

void draw_init();
void draw_shutdown();

void draw_borders();
void draw_snake();
void draw_apple();
void draw_game_over();
void draw();

#endif /* DRAW_H_ */
