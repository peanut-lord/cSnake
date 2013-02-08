// +----------------------------------------------------------------+
// | cSnake                                                         |
// +----------------------------------------------------------------+

/**
 * Port of my python snake to c - memory leaks included :)
 *
 * @author  Christopher Marchfelder <marchfelder@googlemail.com>
 * @license GPL
 */
#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

typedef struct coord {
	int x;
	int y;
} coord;

// Linked list node
typedef struct llnode {
	struct coord part;
	struct llnode *next;
	struct llnode *prev;
} llnode;

llnode *linked_list_create_node(coord part);
llnode *linked_list_append_node(coord part, llnode *current);
llnode *linked_list_prepend_node(coord part, llnode *current);
llnode *linked_list_get_last(llnode *element);
void    linked_list_destroy(llnode *node);

#endif /* LINKED_LIST_H_ */
