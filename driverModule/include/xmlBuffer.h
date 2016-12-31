#ifndef XMLBUFFER_H
typedef struct objXmlBuf {
	unsigned int tail=0;
	unsigned int head=0;
	unsigned int msgCount=0;
	unsigned int msgSizeMax=0;
	unsigned int msgCountMax=0;
	void *bufAddr;
} XmlBuffer;

XmlBuffer *getBuffer(unsigned int msgCountMax, unsigned int msgSizeMax);
void freeBuffer(XmlBuffer *buf);
#endif
