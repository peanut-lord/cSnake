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
#include <time.h>

// @todo including .c files seems rather wrong...
#include "snake.h"
#include "linked_list.c"

//
llnode *snake;
coord  apple;

// DEBUG FILE
FILE *f;

void setup() {
	WINDOW *win = initscr();

	// We don't want to wait for the EOF && getch shall not block the terminal
	cbreak();
	timeout(0);

	// Without keypad enabled KEY_* and getch won't match
	keypad(win, TRUE);

	// No cursor
	curs_set(0);

	// DEBUG
	f = fopen("/tmp/snake", "a+");
}

void tear_down() {
	// Destroy the snake
	linked_list_destroy(snake);

	// Close file
	fclose(f);

	endwin();
}

int collides() {
	llnode *HEAD = snake;

	// Head hits the borders
	if (HEAD->part.x == 0 || HEAD->part.x == width || HEAD->part.y == 0 || HEAD->part.y == height) {
		return 1;
	}

	// Head hits it's own tail
	/*
	llnode *next;
	while (next != NULL) {
		if (HEAD->part.x == next->part.x && HEAD->part.y == HEAD->part.y) {
			return 1;
		}

		next = next->next;
	}
	*/

	return 0;
}

void spawn_apple() {
	// @todo Spawn apple not within snake or within the borders
	// Set the seed
	srand(time(0));

	int x, y = 0;
	while(1) {
		x = rand() % width + 0;
		y = rand() % height + 0;

		if (x != width && x != 0 && y != height && y != 0) {
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
	// We don't want to draw the whitespace's
	erase();

	// Draw borders
	mvhline(0, 0, ACS_PLMINUS, width);
	mvhline(height, 0, ACS_PLMINUS, width);

	mvvline(0, 0, ACS_PLMINUS, height+1);
	mvvline(0, width, ACS_PLMINUS, height+1);

	// Draw the snake
	llnode *HEAD = snake;
	while(HEAD != NULL) {
		mvaddch(HEAD->part.y, HEAD->part.x, TOKEN_SNAKE);
		HEAD = HEAD->next;
	}

	// Draw apple
	mvaddch(apple.y, apple.x, TOKEN_APPLE);

	refresh();
}

int process_input() {
	int i = getch();

	// getch returns -1 when no user input given
	if (i == -1) {
		return 0;
	}

	if (i == 'q') {
		// @todo quit game
	}

	// @todo check for key strokes like pause game and quit after game over

	if (i != KEY_RIGHT && i != KEY_LEFT && i != KEY_UP && i != KEY_DOWN) {
		// Ignore key, not valid
		return 0;
	}

	direction = i;
	return 1;
}

void expand_snake() {
	llnode *last = linked_list_get_last(snake);

	coord p;

	//
}

void move_snake() {
	llnode *HEAD = snake;
	coord new;

	new.x = HEAD->part.x;
	new.y = HEAD->part.y;

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

	while (1) {
		// @todo  calculate stuff
		process_input();
		move_snake();

		if (collides()) {
			// @todo a blinking game over screen would be cool :)
			break;
		}

		if (eats_apple()) {
			expand_snake();
			spawn_apple();
		}

		frame();

		// Don't like this one; change
		usleep(500000);
	}

	// Game's over
	tear_down();
}

int main() {
	run();
}
