#ifndef _CLONE_DETECT_C
#define _CLONE_DETECT_C

#include <stdio.h>
extern int clone_check(int argc, char * argv[]);
int main(int argc, char * argv[]){
  clone_check(argc, argv);
}

#endif
