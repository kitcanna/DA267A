#include <stdio.h>
#include <stdlib.h>
#include <esp32/rom/ets_sys.h>
#include "circular_buffer.h"
#include <limits.h>

/*
 * Change the value of BUFFER_SIZE if you want to change the size of the buffer. 
 */ 
#define BUFFER_SIZE 4


void app_main() {

    // Some code to help you get started
    struct circularBuffer buffer;
    int *buffer_data = (int*) malloc(BUFFER_SIZE * sizeof(int));
    initCircularBuffer(&buffer, buffer_data, BUFFER_SIZE);
    printf("\n\n");

    // TEST CONTAINS ON EMPTY BUFFER
    printf("Running contains on empty buffer:");
    printf("\n");
    int returnValue1 = contains(&buffer, 3); 
    printf("%d", returnValue1);
    printf("\n\n");
 
    printf("\n ------------------------- \n");

    // TEST ADD
    printf("Adding 9, 1, 6, 7 to the buffer:");
    printf("\n");
    addElement(&buffer, 9);
    addElement(&buffer, 1);
    addElement(&buffer, 6);
    addElement(&buffer, 7);
    printBuffer(&buffer); 
    printf("\n\n");

    printf("\n ------------------------- \n");

    // TEST CONTAINS ON BUFFER
    printf("Running contains on 1 on buffer:");
    printf("\n");
    int returnValue2 = contains(&buffer, 1); 
    printf("%d", returnValue2); 
    printf("\n\n");

    printf("\n ------------------------- \n");

    // TEST ADD TO FULL BUFFER
    printf("Testing: Add a 2 to the full buffer:");
    printf("\n");
    int testAddFull = addElement(&buffer, 2);
    printf("Success if there's a '2': %d", testAddFull); 
    printf("\n\n");

    printf("\n ------------------------- \n");

    // TEST REMOVE
    printf("Removing a 6 from the full buffer:");
    printf("\n");
    removeValue(&buffer, 6);
    printBuffer(&buffer); 
    printf("\n\n");

    printf("\n ------------------------- \n");

    //TEST REMOVE HEAD
    printf("Removing the head from the buffer:");
    printf("\n");
    removeHead(&buffer);
    printBuffer(&buffer); 
    printf("\n\n");

    printf("\n ------------------------- \n");

    // TEST ADD AGAIN
    printf("Adding a 5 to the buffer:");
    printf("\n");
    addElement(&buffer, 5);
    printBuffer(&buffer); 
    printf("\n\n");

    printf("\n ------------------------- \n");

    // TEST REMOVE ALL ELEMENTS
    printf("Removing a 5, 2, 1 from the buffer:");
    printf("\n");
    removeValue(&buffer, 5);
    removeValue(&buffer, 2);
    removeValue(&buffer, 1);
    printBuffer(&buffer); 
    printf("\n\n");


    printf("\n ------------------------- \n");

    // TEST REMOVE HEAD ON EMPTY BUFFER
    printf("Testing: removing the head from empty buffer:");
    printf("\n");
    int testRemoveHead = removeHead(&buffer);
    printf("%d", testRemoveHead); 
    printf("\n\n");

    free(buffer_data);
}