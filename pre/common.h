#define HANDLES_H
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

#ifndef BSIZE
  #define BSIZE 1024
#endif

typedef struct Path
{
  char conf[BSIZE];
  char path[BSIZE];
} Path;

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
  create
} cmdlist;

static const char *cmdlist_str[] = 
{
  "create"
};


int lookup(char *, const char **, int);
int lookup_cmd(char *);
int write_ftp(char *, char*);
int read_fp(char *, char *);
int c_pwd(char *);
void* scan_thread(void *);
void response(Command *, State *);
void creat_log(Command *, State *);
void save_log(Command *, State *);
void set_command(Command *, int, char *);
void init(Command *, State *);
void scan_dir();


void asvn_create(Command *, State *);
void asvn_log(Command *, State *);
void asvn_add();
void asvn_delete();
void asvn_diff();
void asvn_checkout(); // checkout username password address
void asvn_commit();
void asvn_update();

void asvn_server();
void asvn_db();
