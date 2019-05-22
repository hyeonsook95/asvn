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

/**
 * Creat log contents
 * @type System def
 * @param Command Executed command
 * @param State Status of the user who executed the command
 * @param content Log content, Executed result
 */

void creat_log(Command *cmd, State *state, char *content)
{
  time_t rawt;
  struct tm *t;
  char c_time[20];

  memset(content, '\0', BSIZE);
  memset(c_time, '\0', 20);

  time(&rawt);
  t = localtime(&rawt);
  sprintf(c_time, "%4d-%2d-%2d-%2d:%2d:%2d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

  /* set log [date, username, cmd, cmt] */
  sprintf(content, "%s %s %s %s %s\n", c_time, state->username, cmd->command, cmd->comment, state->message);
}

//save log
/**
 * Logging to '/home/username/asvn/repos' file
 * @type System def
 * @param Command = command,arg,cmt ..., State = user's logged_in,username ...
 */
/*
지역 repository의 log file을 찾는다.
log file을 연다.

log content를 생성한다.

file에 log content를 기록한다.
*/
void save_log(Command* cmd, State *state)
{
  char path[BSIZE]; //log file
  char temp[BSIZE]; //log
	
  memset(path, '\0', BSIZE);
  memset(temp, '\0', BSIZE);
	
  getcwd(path, sizeof(path));
  strcat(path, "/.asvn/logs/LOG");

  strcpy(temp, "write fp function test\n");
  
  write_fp(path, temp);
}

int asvn_create(Command *cmd)
{
  char *path = malloc(sizeof(BSIZE));
  char orgpath[1024];
  char repos[BSIZE]; //repos


  memset(orgpath, '\0', 1024);
//  memset(path, '\0', 1024);
  memset(repos, '\0', 1024);

  if(getcwd(orgpath, sizeof(orgpath)) == NULL) {
    fprintf(stderr, "Current working directory get error: %s\n", strerror(errno));
    return -1;
  }
  printf("org: %s\n", orgpath);
//  strcat(orgpath, cmd->arg);
  strcpy(path, orgpath); //home/h/ref
//  printf("path: %s\n", path);

  //local .asvn folder 생성
  strcat(path, "/.asvn\n");
  printf("path/.asvn: %s\n", path);
  if(mkdir(path, 0777) == -1) {
    fprintf(stderr, "%s directory make error: %s\n", path, strerror(errno));
    return -1;
  }
/*  
  strcat(path, "/conf");
  if(creat(path, 0644) == -1) {
    fprintf(stderr, "%s file make error: %s\n", path, strerror(errno));
    return -1;
  }

  strcpy(path, orgpath);
  strcat(path, "/logs");
  if(mkdir(path, 0777) == -1) {
    fprintf(stderr, "%s directory make error: %s\n", path, strerror(errno));
    return -1;
  }

  strcat(path, "/LOG");
  if(creat(path, 0644) == -1) {
    fprintf(stderr, "%s file make error: %s\n", path, strerror(errno));
    return -1;
  }

  //pwd global repository에 기록
  if((path = getenv("HOME")) == NULL) {
    fprintf(stderr, "Current working directory get error: %s\n", strerror(errno));
    return -1;
  }

  strcat(path, "/.asvn/repos");
  write_fp(path, orgpath);*/
  return 0;
  
}

void main()
{
  Command *cmd = malloc(sizeof(Command));
  State *state = malloc(sizeof(State));

//  save_log(cmd, state);
  asvn_create(cmd);

}
