#include "common.h"

void asvn_creat(Command *cmd, State *state) //enum 23
{
	char *asvnpath = NULL;
	int fd = 0;

	asvnpath = getenv("ASVN");
	strcat(asvnpath, "/repos");

	fd = open(asvnpath, O_WRONLY, 0777);
	write(fd, asvnpath, strlen(asvnpath));

	if(mkdir("./.asvn", 0777) ==-1){
		perror("mkdir");
		exit(1);
	}

	fd = creat("./.asvn/log", 0644);

	char *temp = "# date username cmd cmt\n";
	write(fd, temp, strlen(temp));
}

void ftp_quit(State *state)
{
  state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
  //write_state(state); 
  exit(0);
}

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
int rnum(int sock, int no){//socknumber, send num
    no = 0;
if ((readn(sock,(char *)&no,sizeof(no)))< 0)////////
        {printf("read  error :%d\n", errno); exit(0); }
//        printf("no : %d\n",no);
    no = ntohs(no);/////////////////////
   return no;
}
*/


char *rstr(int sock, char fname[],int len){//socketNumber, str, strlen
char name[len];
memset(name,'\0',len);
    if((read(sock,fname,len)) < 0) {
        printf("server: filename read error :%d\n",errno);
		printf("now im in read error\n");
        }
//	printf("get name : %s\n",fname);
	return fname;
}


int wnum(int sock,int setnum){//socketNUM, WRITING NUM
	int sendnum;
	sendnum = setnum;
    sendnum = htons(sendnum);//////////////////////
    if ((writen(sock,(char *)&sendnum,sizeof(sendnum)))< 0){
	printf("write  error :%d\n", errno); exit(0);
           }
//	printf("sendnum : %d\n",sendnum);

	return sendnum;
}


char * wstr(int sock, char fname[], int len){
if((writen(sock,fname,len)<0)){
	printf("client :  write error %d\n",errno);
	exit(0);
	}
//	printf("fname : %s\n",fname);
	return fname;
}

void asvn_update(State *state){
  dostr(state->sock);//real work
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

int writen(int sd,const char *ptr,int size)
{         int no_left,no_written;
          no_left = size;
          while (no_left > 0) 
                     { no_written = write(sd,ptr,no_left);
                       if(no_written <=0)  return(no_written);
                       no_left -= no_written;
                       ptr += no_written;
                     }
          return(size - no_left);
}



void response(Command *cmd, State *state)
{
   switch(lookup_cmd(cmd->command)){ //user function -> command's enum number
    case CREAT: asvn_creat(cmd,state); break;
    /*case USER: ftp_user(cmd,state); break;
    case PASS: ftp_pass(cmd,state); break;
    case PASV: ftp_pasv(cmd,state); break;
    case LIST: ftp_list(cmd,state); break;
    case CWD:  ftp_cwd(cmd,state); break;
    case PWD:  ftp_pwd(cmd,state); break;
    case MKD:  ftp_mkd(cmd,state); break;
    case RMD:  ftp_rmd(cmd,state); break;
    case RETR: ftp_retr(cmd,state); break;
    case STOR: ftp_stor(cmd,state); break;
    case DELE: ftp_dele(cmd,state); break;
    case SIZE: ftp_size(cmd,state); break;
    case ABOR: ftp_abor(state); break;*/
    case QUIT: ftp_quit(state); break;
    case UP: asvn_update(state); break;
//    case RETR: asvn_retr(state);break;
//    case TYPE: ftp_type(cmd,state); break;
/*    case NOOP:
      if(state->logged_in){
        state->message = "200 Nice to NOOP you!\n";
      }else{
        state->message = "530 NOOB hehe.\n";
      }
      write_state(state);
      break;*/
    default:
      state->message = "500 Unknown command\n";
      //write_state(state);
      break;
  }
}
