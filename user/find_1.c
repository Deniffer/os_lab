#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// argv[3]={ find , directory , file }

/* pseudo code
 for (each file in target_path)
 find ->file strcmp ->if match echo file name and path, else continue
      ->DIR if "."/".." do noting else: find()
*/
char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--) //p initial advance len(path) and back
    ;
  p++; // if path_name = ./file1/a/b/file_c  then p-> "file_c"

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;//return file name
  memmove(buf, p, strlen(p)); //if len(p)<DIRSIZ, move *p content into buf,
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p)); //对buf[len(p)]开始的部分进行初始化为空，就是占满buf的空间
  return buf;
}


void
find(char *path,const char * file){
    //cd target DIR
    int fd;
    char buf[512],*p=0;
    struct dirent de;
    struct stat st;

    if ((fd = open(path,0)) < 0){
        fprintf(2,"find :can not open %s\n",path);
    }
    if(fstat(fd,&st)<0){
        fprintf(2,"find:cannot stat %s\n",path);
        close(fd);
        return ;
    }

    switch (st.type)
    {
    case T_FILE/* constant-expression */:
        /* code */
        if (strcmp(fmtname(path),file) == 0){
            printf("%s\n",path);
        }
        break;
    
    case T_DIR:
        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
            fprintf(2,"can not open%p",path);
            break;
        }

        strcpy(buf,path);
        p = buf + strlen(buf);
        *p ++ = '/';

        while(read(fd,&de,sizeof(de)) == sizeof(de)){
            if (de.inum == 0){
                continue;
            }
            int t = strlen(de.name)>DIRSIZ?DIRSIZ:strlen(de.name);
            memmove(p,de.name,t);
            p[t] = 0;
            if (stat(buf,&st)<0){
                printf("can not stat%buf",buf);
                continue;
            }
            if(strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0){
                continue;
            }
            find(buf,file);
        }
        break;
    } 
    close(fd);
}

void 
main(int argc,char *argv[]){
    if (argc<3){
        fprintf(2,"find must receice a Dir && string");
        exit(1);
    }
    else{
    //file name  fmtname(path)
        find(argv[1],argv[2]);
        exit(0);
    }
}