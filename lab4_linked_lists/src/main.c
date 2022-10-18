#include <stdio.h>
#include <stdlib.h>

#include "single_linked_list.h"
#include "double_linked_list.h"

void testing_singly_list () {
    singleLinkedList* list;
    list = malloc(sizeof(singleLinkedList));
    printf("\n\n");
    printf("-----TESTING SINGLY LINKED LIST-----");
    printf("\n\n");

    printf("-----Initialize and print empty list-----");
    initSingleLinkedList(list);
    printSingleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Adding a 3 to the list-----");
    addElementSingleLinkedList(list, 3);
    printSingleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Adding a 1, 8, 7 to the list-----");
    addElementSingleLinkedList(list, 1);
    addElementSingleLinkedList(list, 8);
    addElementSingleLinkedList(list, 7);
    printSingleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Removing the FIRST element of the list-----");
    removeFirstElementSingleLinkedList(list);
    printSingleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Removing the LAST element of the list-----");
    removeLastElementSingleLinkedList(list);
    printSingleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Adding a 9, 2 to the list-----");
    addElementSingleLinkedList(list, 9);
    addElementSingleLinkedList(list, 2);
    printSingleLinkedList(list);
    printf("---------------\n\n");

}

void testing_doubly_list() {
    doubleLinkedList* list;
    list = malloc(sizeof(doubleLinkedList));
    printf("\n\n");
    printf("-----TESTING DOUBLY LINKED LIST-----");
    printf("\n\n");

    printf("-----Initialize and print empty list-----");
    initDoubleLinkedList(list);
    printDoubleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Adding 14 to the list-----");
    addElementDoubleLinkedList(list, 14);
    printDoubleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Adding 11, 18, 12 to the list-----");
    addElementDoubleLinkedList(list, 11);
    addElementDoubleLinkedList(list, 18);
    addElementDoubleLinkedList(list, 12);
    printDoubleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Removing the FIRST element of the list-----");
    removeFirstElementDoubleLinkedList(list);
    printDoubleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Removing the LAST element of the list-----");
    removeLastElementDoubleLinkedList(list);
    printDoubleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Adding a 19, 9 to the list-----");
    addElementDoubleLinkedList(list, 19);
    addElementDoubleLinkedList(list, 9);
    printDoubleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Adding a 2 and 20 to the list-----");
    addElementDoubleLinkedList(list, 2);
    addElementDoubleLinkedList(list, 20);
    printDoubleLinkedList(list);
    printf("---------------\n\n");

    printf("-----Removing the FIRST element of the list-----");
    removeFirstElementDoubleLinkedList(list);
    printDoubleLinkedList(list);
    printf("---------------\n\n"); 
}

void app_main() {
    //testing_singly_list();
    testing_doubly_list();
}