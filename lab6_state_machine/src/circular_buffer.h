#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

struct circularBuffer{
  int * data;
  int head;
  int tail;
  int maxLength;
};

int modulus_inc(int maxLength, int val);

void initCircularBuffer(struct circularBuffer* bufferPtr, int* data, int maxLen);

int addElement(struct circularBuffer* bufferPtr, int value);

int removeHead(struct circularBuffer* bufferPtr);

#endif

