//handles.c
#include "common.h"

/*
* 파일 or 디렉토리를 삭제하고 로그에 
*/
void asvn_remove(Command *cmd, State *state)
{
	if (remove(cmd->arg) ==-1){
		perror("remove");
		exit(1);
	}

	write_log(cmd, state);
}

/*현재 위치 출력*/
void asvn_pwd(Command *cmd, State *state)
{
	char currentpath[BSIZE];
	
	memset(currentpath, '\0', BSIZE);

	getcwd(currentpath, BSIZE);
	printf("%s\n", currentpath);
}

/*new*/
void asvn_readlog(Command *cmd, State *state)
{
	char logpath[BSIZE];
	char temp[BSIZE];
	char *path=NULL;
	FILE *fp_log;

	memset(logpath, '\0', BSIZE);
	memset(temp, '\0', BSIZE);
	logfile(logpath);

	if(!(fp_log=fopen(logpath, "r"))){
		perror("fopen");
		exit(1);
	}

	printf("---------------------------------------------------------------\n");
	while(fgets(temp, 1024, fp_log)){
		printf("%s", temp);
	}
	printf("---------------------------------------------------------------\n");

	fclose(fp_log);

	
}
/*
* directory 생성
* 생성한 directory를 log에 기록
*/
void asvn_mkdir(Command *cmd, State *state)
{
	char *dirpath = NULL;
	int fd = 0;
	
	if(mkdir(cmd->arg, 0755)){
		perror("mkdir");
		exit(1);
	}
	
	write_log(cmd, state);
}

void asvn_creat(Command *cmd, State *state) //enum 23
{
	char *asvnpath = NULL;
	int fd = 0;

	asvnpath = getenv("HOME");
	strcat(asvnpath, "/asvn/repos");
	
	/*write asvn/repos*/
	if((fd = open(asvnpath, O_WRONLY, 0777)) ==-1){
		perror("open");
		exit(1);
	}

	if(write(fd, asvnpath, strlen(asvnpath)) ==-1){
		perror("write");
		exit(1);
	}

	/*create .asvn & log file*/
	if(mkdir("./.asvn", 0777) ==-1){
		perror("mkdir");
		exit(1);
	}

	if((fd = creat("./.asvn/log", FAUTH)) ==-1){
		perror("creat");
		exit(1);
	}

	char *temp = "# date username cmd cmt\n";
	
	if(write(fd, temp, strlen(temp)) ==-1){
		perror("write");
		exit(1);
	}
}

void ftp_quit(State *state)
{
  state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
  //write_state(state); 
  exit(0);
}

void response(Command *cmd, State *state)
{
  switch(lookup_cmd(cmd->command)){ //user function -> command's enum number
    case CREAT: asvn_creat(cmd,state); break;
    case LOG: asvn_readlog(cmd,state); break;
    case MKDIR: asvn_mkdir(cmd,state); break;
    case DELETE: asvn_remove(cmd,state); break;
    case PWD: asvn_pwd(cmd, state); break;
    case QUIT: ftp_quit(state); break;
    default:
      state->message = "500 Unknown command\n";
      //write_state(state);
      break;
  }
}
