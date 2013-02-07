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

// gcc -W all -l ncurses linked_list.c snake.c draw.c
#include "globals.h"
#include "snake.h"
#include "linked_list.h"
#include "draw.h"

//
llnode *snake;
coord  apple;

// DEBUG FILE
FILE *f;

void setup() {
	draw_init();

	// DEBUG
	f = fopen("/tmp/snake", "a+");
}

void tear_down() {
	// Destroy the snake
	linked_list_destroy(snake);

	// Close file
	fclose(f);

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
	while(!valid) {
		x = rand() % width + 0;
		y = rand() % height + 0;

		// Check if coords are inside the snake
		while(next != NULL) {
			if (next->part.x != x && next->part.y != y) {
				valid = 1;
				break;
			}

			next = next->next;
		}

		// Check if we hit the wall
		if (x != 0 && x != height && y != 0 && y != width) {
			valid = 1;
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
		// skip frame
		return;
	}

	move_snake();

	if (collides()) {
		// @todo a blinking game over screen would be cool :)
		game_over = 1;
	}

	if (eats_apple()) {
		expand_snake();
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
		// @todo perhaps the frame method should handle input and drawing
		// @todo make a level file and end screen etc.
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
