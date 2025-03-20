#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
#include <stdlib.h> // For malloc()

/**
 * @brief A linked list node storing MarkovNode data.
 *
 * @struct Node
 * @field data Pointer to MarkovNode data.
 * @field next Pointer to the next Node in the list.
 */
typedef struct Node {
    struct MarkovNode *data;
    struct Node *next;
} Node;

/**
 * @brief A linked list structure for storing nodes.
 *
 * This structure represents a doubly linked list with pointers
 * to the first and last nodes, as well as a size counter.
 */
typedef struct LinkedList {
    Node *first;
    Node *last;
    int size;
} LinkedList;

/**
 * Add data to new markov_node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add (LinkedList *link_list, void *data);

#endif //_LINKEDLIST_H_
