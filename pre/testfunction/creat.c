#include "testcommon.h"

int write_fp(char *path, char *temp)
{
  FILE *fp;

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

int asvn_create(Command *cmd)
{
  char *path = malloc(sizeof(BSIZE));
  char orgpath[1024]; //home/h/ref

  memset(orgpath, '\0', 1024);

  if(getcwd(orgpath, sizeof(orgpath)) == NULL) { 
    fprintf(stderr, "Current working directory get error: %s\n", strerror(errno));
    return -1;
  }
  strcpy(path, orgpath); 

  strcat(path, "/.asvn");
  if(mkdir(path, 0777) == -1) {
    fprintf(stderr, "%s directory make error: %s\n", path, strerror(errno));
    return -1;
  }

  strcat(path, "/conf");
  if(creat(path, 0644) == -1) {
    fprintf(stderr, "%s file make error: %s\n", path, strerror(errno));
    return -1;
  }

  strcpy(path, orgpath);
  strcat(path, "/.asvn/logs");
  if(mkdir(path, 0777) == -1) {
    fprintf(stderr, "%s directory make error: %s\n", path, strerror(errno));
    return -1;
  }

  strcat(path, "/LOG");
  if(creat(path, 0644) == -1) {
    fprintf(stderr, "%s file make error: %s\n", path, strerror(errno));
    return -1;
  }

  if((path = getenv("HOME")) == NULL) {
    fprintf(stderr, "Current working directory get error: %s\n", strerror(errno));
    return -1;
  }

  strcat(path, "/.asvn/repos");
  write_fp(path, orgpath);

}

void main()
{
  Command *cmd = malloc(sizeof(Command));
  State *state = malloc(sizeof(State));

  asvn_create(cmd);

}
