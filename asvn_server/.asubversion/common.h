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
#define MY_PORT_ID 6081
#define SERVER_HOST_ADDR "127.0.0.1"
#define MAXLINE 256
#define MAXSIZE 512   

#define ACK                   2
#define NACK                  3
#define REQUESTFILE           100
#define COMMANDNOTSUPPORTED   150
#define COMMANDSUPPORTED      160
#define BADFILENAME           200
#define FILENAMEOK            400
#define STARTTRANSFER         500
#ifndef BSIZE
  #define BSIZE 1024
#endif

#ifndef FAUTH
	#define FAUTH 0644
#endif


char dataBuf[512];
char buf[512];
char no_read[512]; // file info
char temp_buf[512]; //file trans info
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

  int sock;


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
  CREAT, QUIT, UP
} cmdlist;

/* String mappings for cmdlist */
static const char *cmdlist_str[] = 
{
   "CREAT", "QUIT", "UP"
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

void asvn_update(State *state);

/*send function*/
char * wstr(int , char [], int );
int wnum(int ,int );
char *rstr(int, char[],int );
int rnum(int , int );

int readn(int ,const char *,int );
int writen(int ,const char *,int );
/*socket function*/
int create_socket(int port);

void asvn_retr(int sockid);
void dofile(int newsd, char * filename);//file
void dostr(int newsd);//str
//client to server
void docftp(char *filename,int len, int socketDisc);
