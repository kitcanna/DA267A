#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "single_linked_list.h"

void printSingleLinkedList (struct singleLinkedList* list) {
    
    singleLinkedListElement *position = list->first; 
    printf("\n");

    if (position == NULL) {
      printf("List is empty.");
      printf("\n");
    }

    else {
      while (position != NULL) {
        printf("%d -> ", position -> data);
        position = position -> next;
      }
      printf("\n");
    }
}

int addElementSingleLinkedList(struct singleLinkedList* list, int value){
    //return INT_MIN;
    int returnval = INT_MIN;

    //Allocate memory
    struct singleLinkedListElement* new_data = malloc(sizeof(singleLinkedListElement));

    //Put in the data 
    new_data -> data = value; 

    //If the list is empty.
    if (list -> first == NULL) {
        new_data -> next = NULL; 
        list -> first = new_data; 
        returnval = value; 
    }

    //If the list already has values.
    else {

        //If value needs to go in the first place 
        if (new_data -> data <= list -> first -> data) {

          new_data -> next = list -> first;
          list -> first = new_data;         
          returnval = value;

        }

        else {
            //Create a temporary pointer to keep track of the looping element.
            singleLinkedListElement *position = list -> first;

            //Gets the position based on:
            //While next value isn't NULL and the added value is 
            //less than/equal to the next value
            while (position -> next != NULL && position -> next -> data <= new_data -> data) {
              position = position -> next;
            }
            new_data -> next = position -> next;
            position -> next = new_data;
            returnval = value; 
        }
    }

    return returnval;
} 

void initSingleLinkedList(struct singleLinkedList* list) {
  list->first = NULL;
}

int removeFirstElementSingleLinkedList(struct singleLinkedList* list){
      
      //If list is completely empty.
      if (list -> first == NULL) {
        return INT_MIN;
      }

      //Get position and set it to position + 1.
      singleLinkedListElement* position = list -> first;
      list -> first = position -> next;
      
      //Free the memory.
      free(position);

      return position -> data;
} 

int removeLastElementSingleLinkedList(struct singleLinkedList* list){

      //If the list is completely empty.
      if (list->first == NULL) {
        return INT_MIN;
      }

      singleLinkedListElement* position = list -> first;
      singleLinkedListElement* previous = position;

      while (position -> next != NULL) {
          previous = position;
          position = position -> next;
      }

      if (position == list -> first) {
          list -> first = NULL;
      }

      previous -> next = NULL;

      free(position);

      return position -> data;
} 