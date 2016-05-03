#ifndef jsonbuf_h
#define jsonbuf_h

#include <stdio.h>

struct jsonbuf{
    unsigned char *databuf;
    int flags;
    unsigned long offset;
};

struct jsonbuf* create_jsonbuf(unsigned long len);

void free_jsonbuf(struct jsonbuf* p);

int fill_jsonbuf(struct jsonbuf* p, unsigned long len, int sockfd);

extern struct jsonbuf *PJson;

#endif /* jsonbuf_h */
