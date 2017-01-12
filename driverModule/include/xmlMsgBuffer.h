#ifndef XMLBUFFER_H
typedef struct objXmlMsgBuffer {
	unsigned int head=0;
	unsigned int tail=0;
	unsigned int count=0;
	unsigned int msgSizeMax=0;
	unsigned int msgCountMax=0;
	void *bufAddr;
} XmlMsgBuffer;

XmlMsgBuffer *getXmlMsgBuffer(unsigned int msgCountMax, unsigned int msgSizeMax);
void freeXmlMsgBuffer(XmlBuffer *buf);

#endif
