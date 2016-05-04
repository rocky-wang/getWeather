#include <string.h>
#include <stdlib.h>
#include "getAK.h"

int getAKInfo(const char *filename,void *ak_key,size_t *len)
{
    FILE *akfp;
    char buf[128];
    size_t ak_len;
    char *tmp1;
    
    if (filename == NULL) {
        filename = DEFAULT_AKFILENAME;
    }
    
    akfp = fopen(filename, "r");
    if (akfp == NULL) {
        return -1;
    }
    
    if (fgets(buf, sizeof(buf), akfp) == NULL) {
        return -1;
    }
    if (buf[strlen(buf) - 1] == '\n') {
        buf[strlen(buf) - 1] = 0;
    }
    fclose(akfp);
    
    tmp1 = strstr(buf, "ak=");
    if (tmp1 == NULL) {
        return -1;
    }
    tmp1 += 3;
    
    ak_len = strlen(tmp1);
    if (*len < ak_len) {
        return -1;
    }
    
    *len = ak_len;
    sprintf(ak_key, "%s",tmp1);
    
    return 0;
}