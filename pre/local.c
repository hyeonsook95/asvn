// local에서 실행될 main program, client program이라고도 할 수 있음, 항상 실행되고 있지는 않음 1회성
#include "common.h"

int asvn_create(Command *cmd, Satate *state)
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
  sprintf(orgpath, "%s %s\n", orgpath, state->username);
  write_fp(path, orgpath);

  free(path);
}

void asvn_log(Command *cmd, State *state)
{
  char path[BSIZE]; //log file
  char *temp;
	
  memset(path, '\0', BSIZE);
	
  getcwd(path, sizeof(path));
  strcat(path, "/.asvn/logs/LOG");

  read_fp(path, temp);

  free(temp);
}

main(int argc, char* args[])
{
  Command *cmd = malloc(sizeof(Command));
  State *state = malloc(sizeof(State));

  init(cmd, state);
  set_command(cmd, argc, args);
   
  if(cmd->command[0]<=127 && cmd->command[0]>=0){
     response(cmd);
  }

  free(cmd);
  free(state);
}
