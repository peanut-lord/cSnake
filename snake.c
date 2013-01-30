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

// gcc -Wall linked_list.c snake.c
#include "snake.h"
#include "linked_list.h"

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

	int x, y = 0;

	do {
		x = rand() % width + 0;
		y = rand() % height + 0;
	} while(x == 0 && x == height && y == 0 && y == width);

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
		process_input();

		if (pause_game == 1) {
			// skip frame
			continue;
		}

		move_snake();

		if (collides()) {
			// @todo a blinking game over screen would be cool :)
			break;
		}

		if (eats_apple()) {
			expand_snake();
			spawn_apple();
		}

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
