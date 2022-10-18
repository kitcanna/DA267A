#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

typedef struct doubleLinkedListElement{
  int data;
  struct doubleLinkedListElement *next;
  struct doubleLinkedListElement *previous;
} doubleLinkedListElement;

typedef struct doubleLinkedList{
  struct doubleLinkedListElement *first;
  struct doubleLinkedListElement *last;
} doubleLinkedList;

void printDoubleLinkedList (struct doubleLinkedList* list);

/* 
 * This function should add a new element with value 'value' to the argument linked list.
 * A new value should be added even if the list already contains an element with the 
 * argument value. 
 *
 * When adding elements, you should make sure that the linked list is sorted based on the
 * 'data' field in the linkedListElements, so that the element with smallest value is the
 * first element in the list, followed by the second smallest element, etc. 
 *
 * Please note that you need to dynamically allocate memory (using malloc or calloc) to 
 * your new element. 
 * 
 * When adding an element to your double linked list, you have the choice to start either 
 * from the beginning or the end of the list when searching for the position where to 
 * insert a new element. It is up to you how to decide which search strategy to use. 
 *
 * 
 * Return values:
 *   'value' - If a new element with value 'value' successfully added to the list.
 *   INT_MIN (defined in limits.h) - If a new element could not be added to the list.
 */
int addElementDoubleLinkedList(struct doubleLinkedList* list, int value);

/* 
 * This function should initialize the linked list structure that is given as
 * argument to the function, that is setting list->first and list->last to NULL
 */
void initDoubleLinkedList(struct doubleLinkedList* list);


/* 
 * This function should remove the first element in the linked list, i.e., list->first.
 * 
 * Don't forget to free memory (using free) of the struct element you remove.
 *
 * Return values:
 *  list->first->data if the element was successfully removed.
 *  INT_MIN (defined in limits.h) - If no element could be removed (i.e. the list->first == NULL).
 */
int removeFirstElementDoubleLinkedList(struct doubleLinkedList* list);


/* 
 * This function should remove the last element in the linked list.
 * 
 * Don't forget to free memory (using free) of the struct element you remove.
 *
 * Return values:
 *  list->first->data if the element was successfully removed.
 *  INT_MIN (defined in limits.h) - If no element could be removed (i.e. the list->first == NULL).
 */
int removeLastElementDoubleLinkedList(struct doubleLinkedList* list);




#endif
