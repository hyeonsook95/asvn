//https://downman.tistory.com/9
//gcc -pthread -o term term.c
#include "testcommon.h"
//현재 작업경로를 찾아서 p에 저장해줌
int find_info(char *p)
{
  char path[1024];

  memset(path, '\0', 1024);

  if(getcwd(path, 1024) == NULL) { //현재 작업 경로
    fprintf(stderr, "Current working directory get error: %s\n", strerror(errno));
    exit(0);
  }

  strcpy(p, path);
}

int write_fp(char *path, char *temp)
{
  FILE *fp;

  strcat(temp, "\n");

  if((fp = fopen(path, "a")) == NULL){
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

//쓰는 건 해결함. 근데 ummm... thread 한번 들어갔다 나온 p->path값에 쓰레기 값이 들어감? 왜징

void* scan_thread(void *path)
{
  Path *p = path;

  int thr_id;
  int status;
  pthread_t p_thread[1];

  DIR *dir_ptr = NULL; //디렉토리
  struct dirent *file = NULL;
  struct stat buf;
  char *ext;
  char filename[BSIZE];

  //initialize
  memset(filename, '\0', BSIZE);
  strcpy(filename, p->path);
  printf("filename[1] %s\n", filename);

  if((dir_ptr = opendir(filename)) == NULL){
    exit(0);
  } //p->path를 연다.

  //list load
  while((file = readdir(dir_ptr)) != NULL){

    //무한루프 방지, . or .. 는 skip
    if(strcmp(file->d_name, ".") == 0){
      continue;
    }
    else if(strcmp(file->d_name, "..") == 0){
      continue;
    }

 //   sprintf(filename, "%s/%s", p->path, file->d_name);
    sprintf(p->path, "%s/%s", filename, file->d_name);
    printf("p->path %s\n", p->path);

    //파일의 속성 얻기
    if(stat(p->path, &buf) == -1){
      continue;
    }

    if(S_ISREG(buf.st_mode)){
      //conf에다가 filename path를 넣는다.
      write_fp(p->conf, p->path);
    }
    //dir이면 thread를 작동한다.
    else if(S_ISDIR(buf.st_mode)){
      printf("\nIN THREAD!\n");
      thr_id = pthread_create(&p_thread[0], NULL, scan_thread, (Path *)p);

      pthread_join(p_thread[0], (void **) &status);
    }
    //파일의 속성이 dir가 아니면
  }

  closedir(dir_ptr);
}

void scan_dir()
{
  Path *p = malloc(sizeof(Path));

  find_info(p->conf);

  strcpy(p->path, p->conf);
  strcat(p->conf, "/.asvn/info/struct");

  scan_thread(p);
  free(p);
}

void main()
{
  State *state = malloc(sizeof(State));

  scan_dir();
}

