#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// gcc -o object *.c  argc=4,argv[0]=gcc,argv[1]=-o
int 
main(int argc, char *argv[]){ 
// argc the num of argument,argv the string of argument
   // int i ;
    if(argc < 2){
        fprintf(2,"Usage: uptime func need one argument");
        exit(1);//exit(1) stands for unsuccessful termination
    }
   // i = atoi(argv[1]);//contain [a-Z] how to process?
    printf("%d\n",uptime());
    exit(0);

}
