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
#include <arpa/inet.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

#define SERVER_PORT_ID 6081
#define CLIENT_PORT_ID 6089
#define SERVER_HOST_ADDR "127.0.0.1"
#define MAXSIZE 512
#define MAXLINE 53

#define ACK 2
#define NACK 3
#define REQUESTFILE 100
#define COMMANDNOTSUPPORTED 150
#define COMMANDSUPPORTED 160
#define BADFILENAME 200
#define FILENAMEOK 400
#define STARTTRANSFER 500

#ifndef BSIZE
#define BSIZE 1024
#endif

#ifndef FAUTH
#define FAUTH 0644
#endif

char dataBuf[512];
char buf[512];
char no_read[512]; // file info
char temp_buf[512];
int sock_len;

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
  CREAT,
  UP,
  LOG,
  MKDIR,
  DELETE,
  PWD,
  QUIT
} cmdlist;

/* String mappings for cmdlist */
static const char *cmdlist_str[] =
    {
        "CREAT", "UP", "LOG", "MKDIR", "DELETE", "PWD", "QUIT"};

static char *welcome_message = "A very warm welcome!";

/* Commands User functions */
/*네트워크가 사용되지 않는 사용자 명령어*/
void asvn_creat(Command *, State *);
void asvn_mkdir(Command *, State *);
void asvn_readlog(Command *, State *);
void asvn_pwd(Command *, State *);
void asvn_remove(Command *, State *);

/* 네트워크가 사용되는 사용자 명령어 */
void asvn_update(State *state);

/* Commands handle functions*/
/* 사용자에게 입력받은 명령어 처리를 위한 함수 */
int lookup_cmd(char *);
int lookup(char *, const char **, int);
void parse_command(int, char **, Command *);
void response(Command *, State *);

void str_perm(int, char *);
void my_wait(int);

/* 로그 기록을 위한 함수 */
void logfile(char[]);
void write_log(Command *, State *);
void creat_log(Command *, State *, char[]);

/*sending function*/
int writen(int, const void *, int);
int readn(int, const char *, int);

int wnum(int, int);
char *rstr(int, char fname[], int);
int rnum(int, int);
char *wstr(int, char fname[], int);
int sock();
void docftp(char *, int, int);
void doingstr(int sockid, char name[]);  ////num&str
void doingfile(int sockid, char name[]); ///file
void doftp(int sockid);                  //send to server
