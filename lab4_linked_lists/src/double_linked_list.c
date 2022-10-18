#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "double_linked_list.h"

void printDoubleLinkedList (struct doubleLinkedList* list) {

    doubleLinkedListElement *current = list -> first;

    printf("\n");

    if (current == NULL) {
      printf("List is empty.");
      printf("\n");
    }

    else {
      printf("Printing list: ");
      while (current != NULL) {
        printf("%d <-> ", current -> data);
        current = current -> next;
      }
      printf("\n");
      printf("First: %d | Last: %d \n", list -> first -> data, list -> last -> data);

    }
}

int addElementDoubleLinkedList(struct doubleLinkedList* list, int value){
    //return INT_MIN;
    int returnval = INT_MIN;

    //Allocate memory
    struct doubleLinkedListElement* new_node = malloc(sizeof(doubleLinkedListElement));

    //Put in the data 
    new_node -> data = value; 

    //If list is empty.
    if (list -> first == NULL) {
        //Set next and previous of the new node 
        new_node -> next = NULL;
        new_node -> previous = NULL;

        //Set first and last of the list
        list -> first = new_node;
        list -> last = new_node;

        returnval = value;
    }

    //If the list has some values.
    else {

         //If value needs to go in the first place 
        if (new_node -> data <= list -> first -> data) {
          
          //Set next and previous of the new node
          new_node -> next = list -> first; 
          new_node -> previous = NULL;

          //Set first of the list
          list -> first = new_node;  

          /*
          * AVKOMMENTERAD 
          */
          //list -> last = new_node -> next; 

          returnval = value;
        }

        else {
            //Create a temporary pointer to keep track of the looping element.
            doubleLinkedListElement *position = list -> first;

            //Gets the position based on:
            //While next value isn't NULL and the added value is 
            //less than/equal to the next value
            while (position -> next != NULL && position -> next -> data <= new_node -> data) {
              position = position -> next;
            }
            
            //Set next and previous of the new node 
            new_node -> next = position -> next;
            new_node -> previous = position -> previous;

            //Set first and last of the list
            position -> next = new_node;
            position -> previous = new_node -> next;

            /*
            * Ver 2. Sets the last element to correct element. 
            */
            if (position -> next -> next == NULL) {
                list -> last = position -> next;
            }

            returnval = value; 
        }
    }

    return returnval;
} 


void initDoubleLinkedList(struct doubleLinkedList* list) {
  list->first = NULL;
  list->last = NULL;
}


int removeFirstElementDoubleLinkedList(struct doubleLinkedList* list){
      //If list is completely empty.
      if (list -> first == NULL) {
        return INT_MIN;
      }

      //Get position and set it to position + 1.
      doubleLinkedListElement* position = list -> first;

      list -> first = position -> next;
      position -> previous = NULL;
      
      //Free the memory.
      free(position);

      return position -> data;
} 


int removeLastElementDoubleLinkedList(struct doubleLinkedList* list) {
      
      //If the list is completely empty.
      if (list->first == NULL) {
        return INT_MIN;
      }

      doubleLinkedListElement* position = list -> first ;
      doubleLinkedListElement* previous = position;

      while (position -> next != NULL) {
            previous = position;
            position = position -> next;
      }

      if (position == list -> first) {
            list -> first = NULL;
            list ->last = NULL;
      }
      
      /*
      * Ver 2. Sets the last to correct element.
      */
      if (position -> next == NULL) {
            list -> last = previous; 
      }
      previous -> next = NULL;

      
      free(position);

      return position -> data;
}






