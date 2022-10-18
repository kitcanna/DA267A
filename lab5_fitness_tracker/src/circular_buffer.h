#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H


/* 
 * Data structure used to hold a circular buffer.
 */
struct circularBuffer{
  u_int32_t * data;
  int head;
  int tail;
  int maxLength;
  u_int32_t size;
};

int modulus_inc(int maxLength, int val);

int decrement(int maxLength, int val); 

/*
 * Initialize an empty buffer.
 */
void initCircularBuffer(struct circularBuffer* bufferPtr, uint32_t* data, int maxLen);

/*
 * This function should add the value specified by the 'value' 
 * argument at the tail of the buffer.
 *
 * The function should return:
 *  - 'value' if the value was successfully added to the queue.
 *  - INT_MIN (defined in limits.h) if the value was not added.
 */
u_int32_t addElement(struct circularBuffer* bufferPtr, u_int32_t value);


/* 
 * Remove the oldest element, which is at the head of the queue. 
 * 
 * The function should return:
 *   - 'value' if the head element was successfully removed
 *   - INT_MIN (defined in limits.h) if no element was removed (i.e., the
 *     queue was empty when the function was called.       
 */
u_int32_t removeHead(struct circularBuffer* bufferPtr);


u_int32_t getsize(struct circularBuffer* bufferPtr);


#endif
