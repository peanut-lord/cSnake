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

	return element;
}

llnode *linked_list_add_node(coord part, llnode *current) {
	// We need a node first
	llnode *next = linked_list_create_node(part);

	if (next == NULL) {
		return NULL;
	}

	current->next = next;
	return next;
}

llnode *linked_list_get_last(llnode *node) {
	llnode *target = node;
	while (target->next != NULL) {
		target = target->next;
	}

	return target;
}

void linked_list_destroy(llnode *node) {
	// We don't want elements get lost in memory
	if (node->next != NULL) {
		linked_list_destroy(node->next);
	}

	free(node);
}
