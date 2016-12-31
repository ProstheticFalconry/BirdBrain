#include <stdio.h>
#include <xmlBuffer.h>


XmlBuffer *getBuffer(unsigned int msgCountMax, unsigned int msgSizeMax){
	XmlBuffer *buf=(XmlBuffer *)malloc(sizeof(XmlBuffer));
	buf->msgSizeMax=msgSizeMax;
	buf->msgCountMax=msgCountMax;
	buf->bufAddr=malloc(msgCountMax*msgSizeMax);
}

void freeBuffer(XmlBuffer *buf){
	free(buf->bufAddr);
	free(buf);
}
