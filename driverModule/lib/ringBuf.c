#include "ringBuf.h"
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>
//#include <stdlib.h>
#include <asm/div64.h>

MODULE_LICENSE("GPL");              ///< The license type -- this affects runtime behavior
MODULE_AUTHOR("Derek Molloy");      ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux driver for the BBB.");  ///< The description -- see modinfo
MODULE_VERSION("0.1");              ///< The version of the module



void *memAlloc(size_t size){
	//return malloc(size);
	return kmalloc(size,GFP_KERNEL);
}

void memFree(void* ptr){
	//free(ptr);
	kfree(ptr);
}
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
	unsigned long long  bufUse=ringBuf->bufUse;
        ptr=ringBuf->buf;
        if (bufUse==capacity){
                return 1;
        }
	*(ptr+index)=*input;
	bufUse++;
        while(*input++ != '\0'){
                if(bufUse == capacity){
                        *(ptr+mod64(index,capacity))='\0';
			ringBuf->bufUse=bufUse;
			ringBuf->head=mod64(index,capacity);
                        return 1; 
                }
		bufUse++;
                *(ptr+mod64(index,capacity))=*input;
        }
	ringBuf->bufUse=bufUse;
	ringBuf->head=mod64(index,capacity)+1;
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
	while(*(ptr+mod64(index,capacity)) != '\0'){
		if(bufUse==0 || ++counter==bufSize){
			*output='\0';
			ringBuf->bufUse=bufUse;
			return 1;
		}
		index++;
		bufUse--;
		*++output=*(ptr+mod64(index,capacity));	
	}
	ringBuf->tail=mod64(index,capacity)+1;
	ringBuf->bufUse=bufUse;
	return 0;
}

unsigned long long mod64(unsigned long long num, unsigned long long base){
	return num-do_div(num,base);
}
