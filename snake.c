// +----------------------------------------------------------------+
// | cSnake                                                         |
// +----------------------------------------------------------------+

/**
 * Port of my python snake to c - memory leaks included :)
 *
 * @author  Christopher Marchfelder <marchfelder@googlemail.com>
 * @license GPL
 */
// System libs
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <sys/timeb.h>

// gcc -g -Wall -l ncurses linked_list.c snake.c draw.c
#include "globals.h"
#include "snake.h"
#include "linked_list.h"
#include "draw.h"

#define DEBUG 1

//
llnode *snake;
coord  apple;

// DEBUG FILE
FILE *f;

void setup() {
	draw_init();

#if DEBUG == 1
	// DEBUG
	f = fopen("/tmp/snake", "a+");
#endif
}

void tear_down() {
	// Destroy the snake
	linked_list_destroy(snake);

#if DEBUG == 1
	// Close file
	fclose(f);
#endif

	draw_shutdown();
}

int collides() {
	llnode *HEAD = snake;

	// Head hits the borders
	if (HEAD->part.x == 0 || HEAD->part.x == width || HEAD->part.y == 0 || HEAD->part.y == height) {
		return 1;
	}

	// Head hits it's own tail
	llnode *next = HEAD->next;
	while (next != NULL) {
		if (HEAD->part.x == next->part.x && HEAD->part.y == next->part.y) {
			return 1;
		}

		next = next->next;
	}

	return 0;
}

void spawn_apple() {
	// Set the seed
	srand(time(0));

	int valid, x, y = 0;

	llnode *next = snake;
	while(1) {
		x = rand() % width + 0;
		y = rand() % height + 0;

		// Check if we hit the wall
		if (y != 0 && y != height && x != 0 && x != width) {
			valid = 1;
		}

		// Check if coords are inside the snake
		while(next != NULL) {
			if (next->part.x == x && next->part.y == y) {
				valid = 0;
				break;
			}

			next = next->next;
		}

#if DEBUG == 1
		fprintf(f, "width: %d, height: %d, x: %d, y: %d, valid: %d\n", width, height, x, y, valid);
#endif

		if (valid == 1) {
			break;
		}
	}

	apple.x = x;
	apple.y = y;
}

int eats_apple() {
	return snake->part.x == apple.x && snake->part.y == apple.y;
}

void frame() {
	process_input();

	if (pause_game == 1) {
		// Skip frame
		return;
	}

	move_snake();

	if (collides()) {
		game_over = 1;
	}

	if (eats_apple()) {
		// Yipiieeee, we get another round in hell
		score++;
		spawn_apple();
	}

	draw();
}

void process_input() {
	int i = getch();

	// getch returns -1 when no user input given
	if (i == -1) {
		return;
	}

	if (i == 'q') {
		run_game = 0;
	}

	if (i == ' ') {
		pause_game = !pause_game;
	}

	if (game_over == 1 && i == ' ') {
		// quit
		run_game = 0;
	}

	// If the game is paused, we don't take any directions from here
	if (pause_game == 1) {
		return;
	}

	if (i != KEY_RIGHT && i != KEY_LEFT && i != KEY_UP && i != KEY_DOWN) {
		// Ignore key, not valid
		return;
	}

	direction = i;
}

void move_snake() {
	coord pos;
	pos.x = snake->part.x;
	pos.y = snake->part.y;

	switch(direction) {
		case KEY_UP:
			pos.y--;
			break;
		case KEY_DOWN:
			pos.y++;
			break;
		case KEY_LEFT:
			pos.x--;
			break;
		case KEY_RIGHT:
			pos.x++;
			break;
		default:
			// Nothing
			break;
	}

	snake = linked_list_prepend_node(pos, snake);
	if(!eats_apple()) {
		llnode *last = linked_list_get_last(snake);

		// We need to remove the reference in the next to last to the last
		llnode *nextToLast = last->prev;

		linked_list_destroy(last);
		nextToLast->next = NULL;
	}

}

void run() {
	// Init ncurses and set some defaults
	setup();

	// The snake shall begin in the center of the game
	coord head;
	head.x = width / 2;
	head.y = height / 2;

	snake = linked_list_create_node(head);

	ftime(&last_update);

	// Spawn the first apple
	spawn_apple();

	while (run_game) {
		ftime(&now);

		diff = (1000.0 * (now.time - last_update.time) + (now.millitm - last_update.millitm));
		if (diff < frame_rate) {
			// Skip Frame
			continue;
		}

		// Get the input and draw a frame
		frame();

		last_update = now;
	}

	// Game's over
	tear_down();
}

int main() {
	run();
	return 1;
}
