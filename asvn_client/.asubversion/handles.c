#include "common.h"

/*new*/
void asvn_remove(Command *cmd, State *state)
{
	if (remove(cmd->arg) ==-1){
		perror("remove");
		exit(1);
	}

	write_log(cmd, state);
}

/*new*/
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
/*new*/
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

/*
int rnum(int sock, int no){//소켓넘버,받으려는 수
    no = 0;
if ((readn(sock,(char *)&no,sizeof(no)))< 0)////////
        {printf("read  error :%d\n", errno); exit(0); }
        printf("no : %d\n",no);
    no = ntohs(no);/////////////////////
   return no;
}
*/

int rnum(int sock, int no){//소켓넘버,받으려는 수
    no = 0;
    //read
    sock_len =read(sock,dataBuf,512);
    if(sock_len<0){
    	printf("read error : %d\n",errno);
        exit(0);
    }
    sprintf(dataBuf,"%d",FILENAMEOK);
    //write
    sock_len = write(sock,dataBuf,strlen(dataBuf));
    if(sock_len<0){
	printf("file Ok Recomand Fail %d\n",errno);
    }
    printf("data : %s\n",dataBuf);
    //char to int
    no=atoi(dataBuf);
   return no;
}
/*
char *rstr(int sock, char fname[],int len){//socketNumber, str, strlen
char name[len];
memset(name,'\0',len);
    if((read(sock,fname,len)) < 0) {
        printf("server: filename read error :%d\n",errno);
		printf("now im in read error\n");
        }
//	printf("fname :%s\n",fname);
//	printf(" name : %s , fname : %s \n", name, fname);
	strncpy(name,fname,len-1);
printf(" name : %s , fname : %s \n", name, fname);
	return name;
}


int wnum(int sock,int setnum){//socketNUM, WRITING NUM
//printf("start\n");
	int sendnum;
	sendnum = setnum;
    sendnum = htons(sendnum);//////////////////////
    if ((writen(sock,(char *)&sendnum,sizeof(sendnum)))< 0){
	printf("write  error :%d\n", errno); exit(0);
           }
	printf("sendnum : %d\n",sendnum);
  //  printf("COMPLETE\n");

	return sendnum;
}


char * wstr(int sock, char fname[], int len){
if((writen(sock,fname,len)<0)){
	printf("client :  write error %d\n",errno);
	exit(0);
	}
	printf("fname : %s\n",fname);
	return fname;
}


int readn(int sd,const char *ptr,int size)

{         int no_left,no_read;
          no_left = size;
          while (no_left > 0) 
                     { no_read = read(sd,ptr,no_left);
                       if(no_read <0)  return(no_read);
                       if (no_read == 0) break;
                       no_left -= no_read;
                       ptr += no_read;
                     }
          return(size - no_left);
}


int writen(int sd,const void *ptr,int size)
{         int no_left,no_written;
 //        const char *ptr =NULL;
//             ptr=ptr;
           no_left = size;
           while (no_left > 0) 
                      { 
			if((no_written = write(sd,ptr,no_left))<=0){
                        if(no_written <0&& errno==EINTR)
				no_written =0; 
			else
				return(-1);
			}
                        no_left -= no_written;
                       ptr += no_written;
                     } 
           return(no_left);
}
*/
void asvn_update(){
int sockid;
 printf("socket staring\n");
   sockid= sock();
//클라이언트로부터 파일을 보낼 때
doftp(sockid);
printf("asvn is updatign .. \n");

}

void response(Command *cmd, State *state)
{
   switch(lookup_cmd(cmd->command)){ //user function -> command's enum number
    case CREAT: asvn_creat(cmd,state); break;
    case UP: asvn_update(); break;
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
