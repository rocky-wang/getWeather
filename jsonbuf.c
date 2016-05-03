#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "debug.h"
#include "jsonbuf.h"

struct jsonbuf *PJson;

struct jsonbuf* create_jsonbuf(unsigned long len)
{
    struct jsonbuf *p = NULL;
    
    p = (struct jsonbuf *)malloc(sizeof(struct jsonbuf));
    if(p == NULL){
        ERROR("create json failed!\n");
        return NULL;
    }
    
    p->databuf = (unsigned char *)malloc(len);
    if( p->databuf == NULL ){
        ERROR("create json failed!\n");
        free(p);
        return NULL;
    }
    p->offset = 0;
    p->flags = 0;
    
    return p;
}

void free_jsonbuf(struct jsonbuf* p)
{
    if(p->databuf != NULL){
        free(p->databuf);
        free(p);
    }
}