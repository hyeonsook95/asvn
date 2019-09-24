#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
//#include <dirent.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <netinet/in.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

#include <pthread.h>

#ifndef BSIZE
  #define BSIZE 1024
#endif

typedef struct Command
{
  char command[10];
  char arg[BSIZE];
  char comment[BSIZE];
} Command;

typedef struct State
{
  char *username;
  char *message;  //명령 실행결과
} State;

typedef enum cmdlist
{
  log, commit, checkout, test
} cmdlist;

static const char *cmdlist_str[] = 
{
  "log", "commit", "checkout", "test"
};

typedef struct Path
{
  char conf[BSIZE];
  char path[BSIZE];
} Path;


/* System def */
void parse_command();
void creat_log();
void save_log();
