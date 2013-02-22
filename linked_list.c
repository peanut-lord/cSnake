// +----------------------------------------------------------------+
// | cSnake                                                         |
// +----------------------------------------------------------------+

/**
 * Port of my python snake to c - memory leaks included :)
 *
 * @author  Christopher Marchfelder <marchfelder@googlemail.com>
 * @license GPL
 */
// Standard libs
#include <stdio.h>
#include <stdlib.h>

// Linked list header
#include "linked_list.h"

llnode *linked_list_create_node(coord part) {
	// Try to allocate memory
	llnode *element = malloc(sizeof(llnode));

	if (element == NULL) {
		return NULL;
	}

	element->part = part;
	element->next = NULL;
	element->prev = NULL;

	return element;
}

llnode *linked_list_append_node(coord part, llnode *current) {
	// We need a node first
	llnode *next = linked_list_create_node(part);

	if (next == NULL) {
		return NULL;
	}

	current->next = next;
	next->prev    = current;

	return next;
}

llnode *linked_list_prepend_node(coord part, llnode *current) {
	// We need a node first
	llnode *node = linked_list_create_node(part);

	if (node == NULL) {
		return NULL;
	}

	// Rearrange the nodes to insert this one in between
	if (current->prev != NULL) {
		node->prev       = current->prev;
		node->prev->next = node;
	}

	node->next    = current;
	current->prev = node;

	return node;
}

llnode *linked_list_get_last(llnode *node) {
	llnode *target = node;
	while (target->next != NULL) {
		target = target->next;
	}

	return target;
}

int linked_list_count(llnode *node) {
	llnode *next = node;
	int cnt;

	while(next != NULL) {
		cnt++;
		next = next->next;
	}

	return cnt;
}

void linked_list_destroy(llnode *node) {
	// We need to erase a pointer pointing to us
	if (node->prev != NULL) {
		node->prev->next = NULL;
	}

	if (node->next != NULL) {
		linked_list_destroy(node->next);

		node->next = NULL;
		node->prev = NULL;
	}

	free(node);
}
