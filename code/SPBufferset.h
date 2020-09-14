#include <stdio.h>

#ifndef SPBUFFERSET_H_
#define SPBUFFERSET_H_
#define BUG_FIX 1
#if BUG_FIX
#define SP_BUFF_SET() {	               \
	setvbuf(stdout,NULL,_IONBF,0); 	   \
	setvbuf(stderr,NULL,_IONBF,0);     \
	setvbuf(stdin,NULL,_IONBF,0);      \
}
#else
#define SP_BUFF_SET(){}
#endif

#endif /* SPBUFFERSET_H_ */
