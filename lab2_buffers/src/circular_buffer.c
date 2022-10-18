#include <stdio.h>
#include <limits.h>
#include "circular_buffer.h"

void initCircularBuffer(struct circularBuffer *bufferPtr, int *data, int maxLength)
{
  bufferPtr->data = data;
  bufferPtr->head = 0;
  bufferPtr->tail = 0;
  bufferPtr->maxLength = maxLength;
}

int contains(struct circularBuffer *bufferPtr, int value)
{
  /*
   * Traversing using modulus to not exeed the maxlength, to find a specific value. 
   */
  for (int i = bufferPtr->head; i != bufferPtr->tail; i = modulus_inc(bufferPtr->maxLength, i))
  {

    if (bufferPtr->data[i] == value)
    {
      return value;
    }
  }

  return INT_MIN;
}

/*
 * Making sure index is not higher than maxlength
 */
int modulus_inc(int maxLength, int i)
{

  return (i + 1) % maxLength;
}

int addElement(struct circularBuffer *bufferPtr, int value) {
      
      /*
       * This runs if the buffer is completely empty.
       */
      if (bufferPtr -> tail == bufferPtr -> head) {
        bufferPtr->data[bufferPtr->head] = value;
        printf("%d added at index: %d \n", value, bufferPtr -> tail);

        bufferPtr->tail = modulus_inc(bufferPtr->maxLength, bufferPtr->tail);
        return value;
      }

        /*
        * This runs while the buffer isn't full.
        */
      else if (modulus_inc(bufferPtr->maxLength, bufferPtr->tail) != bufferPtr->head) {
            
          // adds the value to the tail.
          bufferPtr->data[bufferPtr->tail] = value;
          printf("%d added at index: %d \n", value, bufferPtr -> tail);

          // increase tail by 1 step using modulus to not exeed the maxlength.
          bufferPtr->tail = modulus_inc(bufferPtr->maxLength, bufferPtr->tail);
          return value;
        
      }

      /*
       * This runs when the tail is at the last position.
       * This adds the value and if we add another value, it will overwrite the current value.
       */  
      else if (modulus_inc(bufferPtr->maxLength, bufferPtr->tail) == bufferPtr->head) {
            
          // add the value to the bufferPtr of the index of the buffer where it points to tail.
          bufferPtr->data[bufferPtr->tail] = value;
          printf("%d added at index: %d \n", value, bufferPtr -> tail);

          // set tail to the modulus of maxlength and tail (next position)
          return value;
        
      }
      else {
        return INT_MIN;
      }
}

/* 
 *
 *
 */
int removeValue(struct circularBuffer* bufferPtr, int value) {

    int returnval = INT_MIN;

    for (int i = bufferPtr->head; i != bufferPtr->tail; i = modulus_inc(bufferPtr->maxLength, i)) {
      
        if (bufferPtr->data[i] == value) {
            returnval = value;
            bufferPtr->data[i] = 0;

            for (int j = i; j != bufferPtr->tail; j = modulus_inc(bufferPtr->maxLength, j)) {
                
                bufferPtr->data[j] = bufferPtr->data[j+1];
            
            }
            bufferPtr->data[bufferPtr->tail] = 0;
            bufferPtr->tail = decrement(bufferPtr->maxLength, bufferPtr->tail);
        }
    }
    return returnval;
}


int removeHead(struct circularBuffer *bufferPtr) {

  if (bufferPtr->head == bufferPtr->tail) {

    // if the head and tail are same and has no value (empty buffer)
    if (bufferPtr->head == 0) {
      return INT_MIN;
    }

    // if the head and tail are the same but holds one value
    else {
      int valRemoved = bufferPtr->data[bufferPtr->head];
      bufferPtr->data[bufferPtr->head] = 0;
      bufferPtr->head = modulus_inc(bufferPtr->maxLength, bufferPtr->head);
      return valRemoved;
    }
  }

  /*
   * Store the current head in a temporary variable, set the data in it to null,
   * move the head and return the value removed.
   * Move the rest of the list towards head so that head is always 0.
   */
  else {
    int valRemoved = bufferPtr->data[bufferPtr->head];
    bufferPtr->data[bufferPtr->head] = 0;

    for (int j = bufferPtr-> head; j != bufferPtr->tail; j = modulus_inc(bufferPtr->maxLength, j)) {
      bufferPtr->data[j] = bufferPtr->data[j+1]; 
    }
    
    bufferPtr->data[bufferPtr->tail] = 0;

    return valRemoved;
  }
}

void printBuffer(struct circularBuffer *bufferPtr) {

  for (int i = bufferPtr->head; i != bufferPtr -> tail; i = modulus_inc(bufferPtr -> maxLength, i)) {
    printf("Index %d: %d\n", i, bufferPtr->data[i]);
  }
  printf("Index %d: %d\n", bufferPtr -> tail, bufferPtr->data[bufferPtr -> tail]);

}

/*
 * Decrements the value of the variable (head).
 */
int decrement(int maxLength, int value)
{
  if (value == 0)
  {
    return maxLength - 1;
  }

  else
  {
    return value - 1;
  }
}
