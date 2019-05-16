//https://dojang.io/mod/page/view.php?id=577

#include "testcommon.h"

int lookup(char *cmd, const char **haystack, int count)
{
  int i;
  for(i=0; i<count; i++){
    if(strcmp(cmd, haystack[i]) == 0) return i;
  }
  return -1;
}

int lookup_cmd(char *cmd)
{
  const int cmdlist_count = sizeof(cmdlist_str) / sizeof(char *);

  return lookup(cmd, cmdlist_str, cmdlist_count);
}

void asvn_log(Command *cmd)
{
  time_t rawt;
  struct tm *t;
  char content[1024];
  char c_time[20];

  memset(content, '\0', BSIZE);
  memset(c_time, '\0', 20);

  char user[] = "user1";
  char mess[] = "message test";

  time(&rawt);
  t = localtime(&rawt);
  sprintf(c_time, "%4d-%2d-%2d-%2d:%2d:%2d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

  sprintf(content, "%s %s %s %s", c_time, cmd->command, user, mess);

  // local의 기록 저장소를 찾아서 기록 저장소에 log 기록.

}


void response(Command *cmd)
{
  switch(lookup_cmd(cmd->command)){
    case log: asvn_log(cmd); break;
    case test: printf("test function\narg:%s\ncmt:%s", cmd->arg, cmd->comment); break;
    default: printf("Unknown command\narg:%s\ncmt:%s", cmd->arg, cmd->comment); break;
  }
}

void refactory_command(int args, ...)
{
  va_list ap;

  va_start(ap, args);
  for (int i = 0; i<args; i++){
    
}

void main(int argc, char* args[])
{
  Command cm = {args[1], args[2], args[3]};
  Command *cmd = malloc(sizeof(Command));

  // args의 개수가 몇개인지 확인
  cmd = &cm;

/*  strcpy(cmd->command, args[1]);
  strcpy(cmd->arg, args[2]);
  strcpy(cmd->comment, args[3]);
*/

//  printf("%s function\narg:%s\ncmt:%s\n", cmd->command, cmd->arg, cmd->comment);

  /* response */
  response(cmd);

}
