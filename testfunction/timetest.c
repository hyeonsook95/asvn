#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <netinet/in.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

void main()
{
  char temp[1024]="\0";
  time_t timer=time(NULL);
  struct tm *t;

  char current_time[20];

  memset(temp, '\0', 1024);

  /* set current time */
  t = localtime(&timer);
  sprintf(current_time, "%4d-%2d-%2d-%2d:%2d:%2d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

  char* username="testusername";
  char* cmd="testcmd";
  char* cmt="testcmt";

  sprintf(temp, "%s %s %s %s", current_time, username, cmd, cmt);

  printf("temp: %s", temp);

  return ;
}

