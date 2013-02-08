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
#if 0
		expand_snake();
#endif
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

void expand_snake() {
	llnode *last = linked_list_get_last(snake);

	coord p;
	p.x = 0;
	p.y = 0;

	// @todo find a better solution...
	switch(direction) {
		case KEY_UP:
			p.x = last->part.x;
			p.y = last->part.y+1;
			break;
		case KEY_DOWN:
			p.x = last->part.x;
			p.y = last->part.y-1;
			break;
		case KEY_LEFT:
			p.x = last->part.x+1;
			p.y = last->part.y;
			break;
		case KEY_RIGHT:
			p.x = last->part.x-1;
			p.y = last->part.y;
			break;
		default:
			// Nothing
			break;
	}

	//
	linked_list_add_node(p, last);
}

void move_snake() {
#if 0
	llnode *HEAD = snake;
	coord new;

	new.x = HEAD->part.x;
	new.y = HEAD->part.y;

	// @todo if snake has more than one element, it can't move into the opposite direction
	switch(direction) {
		case KEY_UP:
			new.y--;
			break;
		case KEY_DOWN:
			new.y++;
			break;
		case KEY_LEFT:
			new.x--;
			break;
		case KEY_RIGHT:
			new.x++;
			break;
		default:
			// Nothing
			break;
	}

	// The next element in the snake linked list is the
	// future position of the current element
	coord prev;

	while (HEAD != NULL) {
		prev.x = HEAD->part.x;
		prev.y = HEAD->part.y;

		HEAD->part = new;
		new = prev;

		HEAD = HEAD->next;
	}
#else
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

	llnode *prepend = linked_list_create_node(pos);
	prepend->next = snake;
	snake = prepend;

	if(!eats_apple()) {
		// Get the next to last to erease the pointer
		// @todo find cleaner way, perhaps double linked list
		llnode *nextToLast = snake;
		while(nextToLast->next != NULL && nextToLast->next->next != NULL) {
			nextToLast = nextToLast->next;
		}

		// Remove last
		llnode *last = linked_list_get_last(snake);
		linked_list_destroy(last);

		nextToLast->next = NULL;
	}

#endif
}

void run() {
	// Init ncurses and set some defaults
	setup();

	// The snake shall begin in the center of the game
	coord head;
	head.x = width / 2;
	head.y = height / 2;

	snake = linked_list_create_node(head);

	// Spawn the first apple
	spawn_apple();

	while (run_game) {
		// Get the input and draw a frame
		frame();

		// Don't like this one; change
		// usleep(500000);
		usleep(500000);
	}

	// Game's over
	tear_down();
}

int main() {
	run();
	return 1;
}
