#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

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
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de; //directory entry 目录项
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path); //copy from path to buf
    p = buf+strlen(buf);// means p->buf[0] advance strlen(buf)--> p->buf[strlen(buf)]也就是现在p->buf[strlen(buf)]
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0) //此文件夹无文件
         continue;
      memmove(p, de.name, DIRSIZ); //拼接路径字符串 like ./a/b + p
      p[DIRSIZ] = 0;  //将p[14]->0代表结束?
      if(stat(buf, &st) < 0){ //将buf(也就是path) 进行stat
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0);
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0);
}
