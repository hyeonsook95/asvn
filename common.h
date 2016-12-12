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

/* Command struct */
typedef struct Command
{
  char command[10];
  char arg[BSIZE]; /*new*/
  char cmt[BSIZE];
} Command;

/* Commands enumeration */
typedef enum cmdlist 
{ 
  LOG, CREAT, COMMIT, CHECKOUT, UPDATE, PWD, QUIT, MKDIR, DELETE, ADD,
  MV, DIFF, MERGE
} cmdlist;

/* String mappings for cmdlist */
static const char *cmdlist_str[] = 
{
   "LOG", "CREAT", "COOMIT", "CHECKOUT", "UPDATE", "PWD", "QUIT", "MKDIR", "DELETE", "ADD", 
   "MV", "DIFF", "MERGE"
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


