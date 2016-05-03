#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#ifdef DEBUG

//blue
#define DEBUG_LOG(arg...)   do{ fprintf(stdout,"[debug]:");\
	fprintf(stdout,arg);\
	fflush(stdout);}while(0);

//green
#define INFO(arg...)	do{ fprintf(stdout,"[info]:");\
	fprintf(stdout,arg); \
	fflush(stdout);}while(0);

//red
#define ERROR(arg...)	do{ fprintf(stderr,"[ERROR]:");\
	fprintf(stderr,arg); }while(0);

#else

#define DEBUG_LOG(arg...) 

#define INFO(arg...)

#define ERROR(arg...) do{\
	fprintf(stderr,"[error]:");\
	fprintf(stderr,arg); fprintf(stderr,"\033[0m");}while(0);

#endif

#endif

