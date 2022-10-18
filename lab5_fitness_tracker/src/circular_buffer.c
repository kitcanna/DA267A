#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"

void initCircularBuffer(struct circularBuffer* bufferPtr, u_int32_t* data, int maxLength) {
    
    bufferPtr->data = data;
    bufferPtr->head = 0;
    bufferPtr->tail = 0;
    bufferPtr->maxLength = maxLength;

    //Added
    bufferPtr->size = 0;
}

u_int32_t addElement(struct circularBuffer* bufferPtr, u_int32_t value) {

    if (modulus_inc(bufferPtr->maxLength, bufferPtr->tail) == bufferPtr->head) {
        return INT_MIN;
    } 
    
    else {
        bufferPtr->data[bufferPtr->tail] = value;
        bufferPtr->tail = modulus_inc(bufferPtr->maxLength, bufferPtr->tail);
        
        bufferPtr->size++;
        return value;
    }
}

u_int32_t removeHead(struct circularBuffer* bufferPtr) {
    
    if (bufferPtr->head == bufferPtr->tail) {
        return INT_MIN;
    } 
    
    else {
        u_int32_t temp = bufferPtr->data[bufferPtr->head];
        bufferPtr->head = modulus_inc(bufferPtr->maxLength, bufferPtr->head);
        bufferPtr->size--;
        return temp;
    }
}

u_int32_t getsize(struct circularBuffer* bufferPtr) { 
    return bufferPtr->size; 
}

int modulus_inc(int maxLength, int val) { 
    return (val + 1) % maxLength; 
}

int decrement(int maxLength, int val) {
    if (val == 0) {
        return maxLength - 1;
    } 
    else {
        return val - 1;
    }
}

