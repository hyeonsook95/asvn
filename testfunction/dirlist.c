//https://downman.tistory.com/9
//gcc -pthread -o term term.c
#include "testcommon.h"

int write_fp(char *path, char *temp)
{
  FILE *fp;

  strcat(temp, "\n");

  if((fp = fopen(path, "w")) == NULL){
    fprintf(stderr, "%s file open error: %s\n", path, strerror(errno));
    return -1;
  }

  if(fwrite(temp, strlen(temp), 1, fp) <= 0){
    fprintf(stderr, "%s error: %s\n", temp, strerror(errno));
    return -1;
  }

  fclose(fp);       

  return 0;
}

void* scan_thread(void *path)
{
  Path *p = (Path *)path;
  int thr_id;
  int status;
  pthread_t p_thread[1];

  DIR *dir_ptr = NULL;
  struct dirent *file = NULL;
  struct stat buf;
  char *ext;
  char filename[BSIZE];

  //initialize
  memset(filename, '\0', BSIZE);

  if((dir_ptr = opendir(p->path)) == NULL){
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

    sprintf(filename, "%s/%s", p->path, file->d_name);
    printf("filename: %s\n", filename);

    //파일의 속성 얻기
    if(stat(filename, &buf) == -1){
      continue;
    }

    if(S_ISREG(buf.st_mode)){
      write_fp(p->pwd, filename);
    }
    //dir이면
    else if(S_ISDIR(buf.st_mode)){
      thr_id = pthread_create(&p_thread[0], NULL, scan_thread, (char *)filename);

      pthread_join(p_thread[0], (void **) &status);
    }
  }
}

void scan_dir()
{
  Path *path;
  path = malloc(sizeof(Path));

  if(getcwd(path->pwd, sizeof(path->path)) == NULL) { 
    fprintf(stderr, "Current working directory get error: %s\n", strerror(errno));
    exit(0);
  }

  strcpy(path->path, path->pwd);
  strcat(path->pwd, "/.asvn/info/struct");
  printf("pwd: %s\n", path->pwd);
  scan_thread(path);
}

void main()
{
  scan_dir();
}

