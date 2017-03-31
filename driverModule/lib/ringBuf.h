#ifndef RINGBUF_H
#define RINGBUF_H
#include <stddef.h>
typedef struct objRingBuf {
	char isLocked;
	unsigned long long head;
	unsigned long long tail;
	unsigned long long capacity;
	unsigned long long bufUse;
	unsigned long long elementCount;
	char* buf;
} RingBuf;
void* memAlloc(size_t size);
void memFree(void *ptr);
RingBuf* bufInit(unsigned long long capacity);
void bufFree(RingBuf *ringBuf);
int bufPush(RingBuf *ringBuf, char* input);
int bufPull(RingBuf *ringBuf, char* output, unsigned long long bufSize);
unsigned long long mod64(unsigned long long num, unsigned long long base);
#endif
