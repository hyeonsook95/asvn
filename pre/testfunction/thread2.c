#include "testcommon.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void* scan_thread(void *path)
{
  int thr_id;
  int status;
  pthread_t p_thread[1];

  DIR *dir_ptr = NULL;
  struct dirent *file = NULL;
  struct stat buf;
  char *ext;
  char filename[BSIZE];

  memset(filename, '\0', BSIZE);

  if((dir_ptr = opendir(path)) == NULL){
    exit(0);
  }

  //dir 열고 list load
  while((file = readdir(dir_ptr)) != NULL){
    if(strcmp(file->d_name, ".") == 0){
      continue;
    }
    else if(strcmp(file->d_name, "..") == 0){
      continue;
    }

    printf("file->d_name: %s\n", file->d_name);
    sprintf(filename, "%s/%s", (char*)path, file->d_name);

    //파일의 속성 얻기
    if(stat(filename, &buf) == -1){
      continue;
    }

    //dir이면
    if(S_ISDIR(buf.st_mode)){
      thr_id = pthread_create(&p_thread[0], NULL, scan_thread, (char*)filename);

      pthread_join(p_thread[0], (void **) &status);
    }
  }
}

int main()
{
  char path[1024];

  memset(path, '\0', 1024);

  if(getcwd(path, sizeof(path)) == NULL) { 
    fprintf(stderr, "Current working directory get error: %s\n", strerror(errno));
    return -1;
  }

  scan_thread(path);

  return 0;
}
