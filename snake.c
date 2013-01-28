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
	timeout(1);

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

void frame() {
	// We dont want to draw the whitespace's
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

	if (i != KEY_RIGHT || i != KEY_LEFT || i != KEY_UP || i != KEY_DOWN) {
		// @todo raise error, key not accepted
	}

	direction = i;
	return 1;
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

	while (1) {
		// @todo  calculate stuff
		process_input();
		move_snake();
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
