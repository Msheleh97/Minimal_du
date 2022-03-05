/**
 * (Detailed description of the module.)
 *
 * This module is used to Create a single linked list 
 *
 * @author  Mohammad Mshaleh
 * @since   2021-11-15
 *
 * @{
 */


#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <stdbool.h>


// The type for a node in the list.
typedef struct node{
    struct node *next;
    struct node *prev;
    char *value;
}node;

// The type for a list.
typedef struct list{
    struct node head;
}List;

// List position.
typedef struct list_pos{
    struct node *node;
} ListPos;


/**
 * @name: list-create.
 * @brief Returns a pointer to a list.
 *  Create and return an empty list.
 *
 *
 * @param void.
 * @return Returns a pointer .
 */
List *list_create(void);


/**
 * @name: list destroy.
 * @brief Returns a pointer to a list.
 * Deallocate the list (and all of its values, if any).
 *
 *
 * @param a pointer to a list.
 * @return Returns nothing .
 */
void list_destroy(List *lst);


/**
 * @name: list is empty.
 * @brief Returns a true or false.
 * Check if the list is empty.
 *
 *
 * @param a constant pointer to a list.
 * @return Returns a bool .
 */
bool list_is_empty(const List *lst);


/**
 * @name: list insert.
 * @brief Returns a list position.
 * Insert the value before the position and return the position of the new
 * element.
 *
 * @param  a list position and a pointer to a constant char.
 * @return Returns a list position.
 */
void list_insert(List *lst, char *value);

/**
 * @name: list remove.
 * @brief Returns a list position.
 * Remove the value at the position and return the position of the next element.
 *
 *
 * @param  a list position.
 * @return Returns a list position.
 */
void list_remove(List *lst);

/**
 * @name: list_inspect_first.
 * @brief Returns the value in the first position in the list.
 *
 * @param  list The list to inspect.
 * @return Returns the value in the first position.
 */
char *list_inspect_first(List* lst);

#endif /* LINKEDLIST_H */
