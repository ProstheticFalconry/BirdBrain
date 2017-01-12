#include <ringBuf.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#ifndef _LINUX_SLAB_H
void *memAlloc(size_t size){ 
	return malloc(size);
}

void memFree(void* ptr){
	free(ptr);
}
#else
void *memAlloc(size_t size){
	return (char*)kmalloc(size,GFP_KERNEL);
}

void memFree(void* ptr){
	kfree(ptr);
}
#endif
RingBuf* bufInit(unsigned long long capacity){
	RingBuf ringBuf;
	ringBuf.head=0;
	ringBuf.tail=0;
	ringBuf.bufUse=0;
	ringBuf.capacity=capacity;
	ringBuf.buf=(char*)memAlloc(sizeof(char)*capacity);
	RingBuf* ptr=(RingBuf* )memAlloc(sizeof(RingBuf));
	if(ringBuf.buf==NULL || ptr==NULL){
		memFree(ptr);
		memFree(ringBuf.buf);
		return NULL;
	}
	*ptr=ringBuf;
	return ptr;
}

void bufFree(RingBuf* ringBuf){
	memFree(ringBuf->buf);
	memFree(ringBuf);
}

int bufPush(RingBuf *ringBuf,char* input){
        char* ptr;
	unsigned long long index=ringBuf->head;
	unsigned long long capacity=ringBuf->capacity;
	unsigned long long bufUse=ringBuf->bufUse;
        ptr=ringBuf->buf;
        if (bufUse==capacity){
                return 1;
        }
	*(ptr+index)=*input;
	bufUse++;
        while(*input++ != '\0'){
                if(bufUse == capacity){
                        *(ptr+index%capacity)='\0';
			ringBuf->bufUse=bufUse;
			ringBuf->head=index%capacity;
                        return 1; 
                }
		bufUse++;
                *(ptr+(++index%capacity))=*input;
        }
	ringBuf->bufUse=bufUse;
	ringBuf->head=++index%capacity;
        return 0;
}

int bufPull(RingBuf *ringBuf,char* output,unsigned long long bufSize){
	char* ptr;
	unsigned long long index=ringBuf->tail;
	unsigned long long counter=0;	
	unsigned long long capacity=ringBuf->capacity;
	unsigned long long bufUse=ringBuf->bufUse;
	ptr=ringBuf->buf;
	if(ringBuf->bufUse==0){
                return 1;
        }
	*output=*(ptr+index);
	bufUse--;
	while(*(ptr+index%capacity) != '\0'){
		if(bufUse==0 || ++counter==bufSize){
			*output='\0';
			ringBuf->bufUse=bufUse;
			return 1;
		}
		index++;
		bufUse--;
		*++output=*(ptr+index%capacity);	
	}
	ringBuf->tail=++index%capacity;
	ringBuf->bufUse=bufUse;
	return 0;
}


