#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main (int argc,char *argv[]){
    
    int child_fd[2],parent_fd[2];
    pipe(child_fd);pipe(parent_fd);
/*    if (fork()==0){
        //char *content = read(parent_fd[0],,);
        char c = "a";
        write(child_fd[1],c,sizeof(c))
    }
    else
    {
        //close(1);
        wait();
        char b = "k";
        write(parent_fd[1],b,sizeof(b));
        read(child_fd[1],,);
    }*/
    char *c = "a";//use a pointer towards c
    write(parent_fd[1],&c,sizeof(c));
    if(fork()==0){
        read(parent_fd[1],&c,sizeof(c));
        printf("%d:received ping\n",getpid());
        write(child_fd[1],&c,sizeof(c));
	exit(0);//need explicit exit important
    }
    read(child_fd[0],&c,sizeof(c));
    printf("%d:receid pong\n",getpid());
    exit(0);
}
