#include <stdio.h>
#include <stdlib.h>
#include <xmlBuffer.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>

#ifdef SLAB_H
#define F_KMEM GFP_KERNEL
char *xmlMsgStrdup(char *src)
{
	char *str;
    	char *p;
    	int len = 0;
    	while (src[len])
        	len++;
    	str = kmalloc(len + 1,F_KMEM);
   	p = str;
    	while (*src)
        *p++ = *src++;
    	*p = '\0';
    	return str;
}

void *xmlMsgMalloc(size_t size){
	return kmalloc(size,F_KMEM);
}

void *xmlMsgRealloc(void *ptr, size_t new_size){
	return krealloc(ptr,new_size,F_KMEM);
}

xmlMemSetup(kfree,xmlMsgMalloc,xmlMsgRealloc,xmlMsgStrdup);
#endif

XmlBuffer *getXmlMsgBuffer(unsigned int msgCountMax, unsigned int msgSizeMax){
	XmlMsgBuffer *buf=(XmlBuffer *)malloc(sizeof(XmlBuffer));
	buf->msgSizeMax=msgSizeMax;
	buf->msgCountMax=msgCountMax;
	buf->bufAddr=malloc(msgCountMax*msgSizeMax);

void freeXmlMsgBuffer(XmlMsgBuffer *buf){
	free(buf->bufAddr);
	free(buf);
}


