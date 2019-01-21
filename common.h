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

#ifndef FAUTH
	#define FAUTH 0644
#endif

typedef struct State
{

  /* Is user loggd in? */
  int logged_in;

  /* Is this username allowed? */
  int username_ok;
  char *username;
  
  /* Response message to client e.g. 220 Welcome */
  char *message;

} State;

/* Command struct
* COMMAND: 10자 내외로 이루어진 명령어를 받는 변수
* ARG: 주소가 필요한 명령어일 경우, 해당되는 주소값을 받는 변수
* CMT: 사용자의 COMMENT가 필요한 명령어일 경우 COMMENT를 받는 변수
*/
typedef struct Command
{
  char command[10];
  char arg[BSIZE]; /*new*/
  char cmt[BSIZE];
} Command;

/* Commands enumeration 
* 명령어를 입력받고 처리할때 오류를 방지하기 위한 ENUMERATION
*/
typedef enum cmdlist 
{ 
  LOG, CREAT, COMMIT, CHECKOUT, UPDATE, PWD, QUIT, MKDIR, DELETE, ADD,
  MV, DIFF, MERGE, UP
} cmdlist;

/* String mappings for cmdlist */
static const char *cmdlist_str[] = 
{
   "LOG", "CREAT", "COOMIT", "CHECKOUT", "UPDATE", "PWD", "QUIT", "MKDIR", "DELETE", "ADD", 
   "MV", "DIFF", "MERGE", "UP"
};

static char *welcome_message = "A very warm welcome!";

/*Don't use Network*/
//void write_state(State *);
void asvn_creat(Command *, State *);
void asvn_mkdir(Command *, State *);
void asvn_readlog(Command *, State *);
void asvn_pwd(Command *, State *);

/* Commands handle functions*/
void response(Command *, State *);
void write_log(Command *, State *);
void creat_log(Command *, State *, char []);


int lookup_cmd(char *);
int lookup(char *, const char **, int);

//void rename_path(char *);
void parse_command(int, char **, Command *);
void str_perm(int, char *);
void my_wait(int);
void logfile(char []);


