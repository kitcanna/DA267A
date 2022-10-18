#include <limits.h>
#include <stdio.h>
#include "circular_buffer.h"

void initCircularBuffer(struct circularBuffer* bufferPtr, int* data, int maxLength) {

    bufferPtr->data = data;
    bufferPtr->head = 0;
    bufferPtr->tail = 0;
    bufferPtr->maxLength = maxLength;

}

int modulus_inc(int maxLength, int val) { 

    return (val + 1) % maxLength; 
}

int addElement(struct circularBuffer* bufferPtr, int value) {

    if (modulus_inc(bufferPtr->maxLength, bufferPtr->tail) == bufferPtr->head) {
        
        return INT_MIN;
    } 
    
    else {
       
       bufferPtr->data[bufferPtr->tail] = value;
        bufferPtr->tail = modulus_inc(bufferPtr->maxLength, bufferPtr->tail);
        return value;
    }
}

int removeHead(struct circularBuffer* bufferPtr) {

    if (bufferPtr->head == bufferPtr->tail) {
        
        return INT_MIN;
    } 
    
    else {
        
        int temp = bufferPtr->data[bufferPtr->head];
        bufferPtr->head = modulus_inc(bufferPtr->maxLength, bufferPtr->head);
        return temp;
    }
}