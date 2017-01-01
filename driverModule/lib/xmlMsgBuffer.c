#include <stdio.h>
#include <stdlib.h>
#include <xmlBuffer.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>

#ifdef SLAB_H
	char *ft_strdup(char *src)
	{
    		char *str;
    		char *p;
    		int len = 0;

    		while (src[len])
        		len++;
    		str = malloc(len + 1);
   	 	p = str;
    		while (*src)
        	*p++ = *src++;
    		*p = '\0';
    		return str;
	}
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


