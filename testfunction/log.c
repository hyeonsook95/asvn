#include "testcommon.h"

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
int save_log(Command* cmd, State *state)
{
  char path[BSIZE]; //log file
  char temp[BSIZE]; //log
  FILE *fp_log;
	
  memset(path, '\0', 1024);
  memset(temp, '\0', 1024);
	
  getcwd(path, sizeof(path));
  strcat(path, "/.asvn/logs/LOG");


  if((fp_log = fopen(path, "w")) == NULL){
    fprintf(stderr, "%s file open error: %s\n", path, strerror(errno));
    return -1;
  }

  if(fwrite(temp, strlen(temp), 1, fp_log) <= 0){
    fprintf(stderr, "%s logging error: %s\n", temp, strerror(errno));
    return -1;
  }

  fclose(fp_log);       
}

void main()
{
  Command *cmd = malloc(sizeof(Command));
  State *state = malloc(sizeof(State));

  save_log(cmd, state);
}
