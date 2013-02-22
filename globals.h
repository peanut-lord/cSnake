// +----------------------------------------------------------------+
// | cSnake                                                         |
// +----------------------------------------------------------------+

/**
 * Port of my python snake to c - memory leaks included :)
 *
 * @author  Christopher Marchfelder <marchfelder@googlemail.com>
 * @license GPL
 */
#ifndef GLOBALS_H_
#define GLOBALS_H_

extern int   width;
extern int   height;
extern int   direction;
extern int   pause_game;
extern int   game_over;
extern int   run_game;
extern int   score;
extern float frame_rate;

extern char TOKEN_SNAKE;
extern char TOKEN_APPLE;

#endif /* GLOBALS_H_ */
