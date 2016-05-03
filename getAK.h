#ifndef getAK_h
#define getAK_h

#include <stdio.h>

#define DEFAULT_AKFILENAME  "/etc/m-yun.conf"

extern int getAKInfo(const char *filename,void *ak_key,size_t *len);

#endif /* getAK_h */
