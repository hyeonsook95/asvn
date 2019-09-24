#include "common.h"

/* args[1] = command, args[2] = orgpath, (args[3] = newpath, args[4~] = cmt)
** Parse inputed command from user
*/
void parse_command(int argc, char **args, Command *cmd)
{
	int n = argc - 2;
	int i = 0;
	int sockid;
	
	switch(lookup_cmd(cmd->command)){
		/*Only command*/
		case LOG: break;
		case CREAT: break;
		case COMMIT: break;
		case CHECKOUT: break;
   printf("socket staring\n");
   sockid= sock();
//서버로 부터 파일을 받아올 때
docftp(args[2],sockid,strlen(args[2]));
break;
		case UPDATE: 
   printf("socket staring\n");
   sockid= sock();
//클라이언트로부터 파일을 보낼 때
doftp(sockid);
break;
		case PWD: break;
		case QUIT: 
			break;
		/*command and path*/
		case MKDIR: break;
		case DELETE: break;
		case ADD:
			strcpy(cmd->arg, args[2]); 
			n--; 
			break;
		/*command, origin path, new path*/
		case MV: break;
		case DIFF: break;
		case MERGE :
			strcpy(cmd->arg, args[2]); n -= 2;
			strcat(cmd->arg, " "); strcat(cmd->arg, args[3]);
			break;
		default:
			printf("500 Unknown command\n");
			break;
	}

	memset(cmd->cmt, '\0', sizeof(cmd->cmt));

	/*inputed comment on the command from user*/		
	for(i=n; i<argc-3; i++){
		if(argc != i-1){
			strcat(cmd->cmt, args[i]);
			strcat(cmd->cmt, " ");
		}
		else
			strcat(cmd->cmt, "\n");
	}
	
	return;
}

void creat_log(Command *cmd, State *state, char temp[500])
{
	time_t timer=time(NULL);
	struct tm *t;

	char current_time[20];

	memset(temp, '\0', BSIZE);
	
	/*set current_time*/
	t = localtime(&timer);
	sprintf(current_time, "%d-%d-%d-%d:%d:%d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
	
	/*set log [date, username, cmd, cmt]*/
	strcat(temp, current_time);
	strcat(temp, " ");
	strcat(temp, cmd->command);
	strcat(temp, " ");
	strcat(temp, cmd->arg);
	strcat(temp, " ");
	strcat(temp, cmd->cmt);
	strcat(temp, "\n");

}
/*
void rename_path(char *path)
{

	char *cmdpath = malloc(sizeof(char)*BSIZE);
	
	memset(cmdpath, '\0', BSIZE);
	strcpy(cmdpath, path);

	if(strstr("./", cmdpath) ==0 || strchr('/', cmdpath) ==NULL){
		if(strstr("./", cmdpath) ==0){
			cmdpath = strtok(cmdpath, "/");
			cmdpath = strtok(NULL, "/");
		}
		getcwd(path, 1024);
		strcat(path, "/");
	}

	strcat(path, cmdpath);
	printf("rename_path path: %s\n", path);
}*/

/*new*/
void write_log(Command* cmd, State *state)
{
	char path[200];
	char logpath[BSIZE];
	char temp[500];
	FILE *fp_log;
	
	memset(logpath, '\0', BSIZE);
	memset(temp, '\0', 500);
	
	/*set .asvn path*/
	logfile(path);

	if((fp_log=fopen(path, "a")) ==NULL){
		perror("fopen");
		exit(1);
	}

	creat_log(cmd, state, temp);

	/*write log in .asvn*/
	if(fwrite(temp, 1, strlen(temp), fp_log) ==-1){
		perror("fwrite");
		exit(1);
	}

//	printf("Success!\n");

	if(fclose(fp_log) ==-1){
		perror("fclose");
		exit(1);
	}
}

int lookup_cmd(char *cmd){
  const int cmdlist_count = sizeof(cmdlist_str)/sizeof(char *);
  return lookup(cmd, cmdlist_str, cmdlist_count); 
}

int lookup(char *needle, const char **haystack, int count)
{ 
  int i;
  for(i=0;i<count; i++){ //(enum cmdlist & char* cmflist_str)'s count
    if(strcmp(needle,haystack[i])==0)return i; //needel == haystack[i] -> return i
  }
  return -1;
}


/*new*/
void logfile(char path[100])
{
	char *asvnpath = NULL;
	char logpath[BSIZE];
	char temp[BSIZE];
	FILE *fp_log;
	
	memset(temp, '\0', BSIZE);
	
	asvnpath=getenv("HOME");
	strcat(asvnpath, "/asvn/repos");
	getcwd(logpath, 1024);
	
	if(!(fp_log=fopen(asvnpath, "r"))){
		perror("repos fpoen");
		exit(1);
	}
	while(fgets(temp, 1024, fp_log)){
		if(strstr(logpath, temp)){
			printf("temp: %s\n", temp);
			strcpy(logpath, temp);
		}
	}

	strcat(logpath, "/.asvn/log");

	strcpy(path, logpath);
}

int sock(){
	int sockid;
    struct sockaddr_in my_addr, server_addr; 

 printf("client: creating socket\n");
    if ((sockid = socket(AF_INET,SOCK_STREAM,0)) < 0)
      { printf("client: socket error : %d\n", errno); exit(0); }
  

    printf("client: binding my local socket\n");
    bzero((char *) &my_addr,sizeof(my_addr));

    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(CLIENT_PORT_ID);
    if (bind(sockid ,(struct sockaddr *) &my_addr,sizeof(my_addr)) < 0)
           {printf("client: bind  error :%d\n", errno); exit(0);
           }
   printf("client: starting connect\n");
    bzero((char *) &server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST_ADDR);
    server_addr.sin_port = htons(SERVER_PORT_ID);
     if (connect(sockid ,(struct sockaddr *) &server_addr,
                                             sizeof(server_addr)) < 0)
           {
		printf("client: connect  error :%d\n", errno); exit(0);
           }
	return sockid;
}


void docftp(char *filename, int socketDisc, int len){ //doftp를 실행한다.
int i,fsize,recv_fsize,fd,msg_ok,size;
    int fail = FILENAMEOK;
    int fail1,req,c,ack,file_info;
    int no_read ,num_blks , num_blks1,num_last_blk,num_last_blk1,tmp;
    char fname[MAXLINE+1];
    char out_buf[MAXSIZE];
    FILE *fp;
    char text[MAXLINE];
    char name[MAXLINE];
    int sendnum=0;

     no_read = 0;
     num_blks = 0;
     num_last_blk = 0;

    memset(fname,'\0',MAXLINE);
    strncpy(fname,filename,len);

    printf("filename : %s\n", fname);

    sprintf(dataBuf,"%d",len);
    printf("sizeof : %d strlen : %d\n",sizeof(dataBuf),strlen(dataBuf));
   
    //write
    sock_len = write(socketDisc,dataBuf,strlen(dataBuf));
    if (sock_len <0){
       printf("write  error :%d\n", errno); 
       exit(0);
    }
    printf("dataBuf : %s\n",dataBuf);       
    //read
    sock_len = read(socketDisc,dataBuf,sizeof(buf));
    if(sock_len<0){
      	printf("recv Fail!");
    }
    sprintf(buf,"%d",FILENAMEOK);
    printf("data Buf : %s buf : %s \n",dataBuf,buf);
    if((strcmp(dataBuf,buf))==0){
        printf("send file name : %s!!\n",fname);
        //write name
	sock_len = write(socketDisc,fname,len);
	if(sock_len<0){
		printf("write file name send fail %d\n",errno);
	}
	//read file Send Ok or No
        sock_len = read(socketDisc,dataBuf,sizeof(dataBuf));
        if(sock_len<0){
		printf("file send info fail %d\n",errno);
                
	}
    }
    //file info
    file_info=atoi(dataBuf);
    if(file_info == STARTTRANSFER){
        sprintf(dataBuf,"start");
        printf("send command start?? : %s\n",dataBuf);
        //write recv data from server start!!!!
   	sock_len = write(socketDisc,dataBuf,sizeof(dataBuf));
       
    }
    else{
          printf("file send info fail !!");
          exit(0);
    }
    


     /* IF SERVER CANT OPEN FILE THEN INFORM CLIENT OF THIS AND TERMINATE */
     if((fp = fopen(fname,"w")) == NULL){ /*cant open file*/
        	perror("fopen error: \n");
     }
     //read 1. read file all size
     sock_len = read(socketDisc,dataBuf,sizeof(dataBuf));
     fsize = atoi(dataBuf);
     //printf("atoi : %d",atoi(dataBuf));
     if(fsize!=0){
        //printf("here come ");
        //fsize = atoi(dataBuf);//num_blks , num_blks1,num_last_blk
        //write ok
	sock_len = write(socketDisc,"size_ok",sizeof("size_ok"));
        //read 2. num_blks_ok
        sock_len = read(socketDisc,dataBuf,sizeof(dataBuf));
        if(sock_len<0){
  		printf("num_blcks_no recv!! error\n");
                exit(0);
        }
        if(atoi(dataBuf)!=0){
		num_blks = atoi(dataBuf);
                //write ok
                sock_len = write(socketDisc,"num_blks_ok",sizeof("num_blks_ok"));
                //read 3. num_last_blk
                sock_len = read(socketDisc,dataBuf,sizeof(dataBuf));
                if(sock_len < 0){
		      printf("num_last_blk no recv!! error\n");
                      exit(0);
		}
                if(strcmp(dataBuf,"none")==0){
		      num_last_blk=0;
                      //write
                      sock_len = write(socketDisc,"num_last_ok",sizeof("num_last_ok"));
	        }else if(atoi(dataBuf)!=0){
	              num_last_blk=atoi(dataBuf);
                      //write 
                      sock_len = write(socketDisc,"num_last_ok",sizeof("num_last_ok"));
                      printf("send ready \nsize : %d\nnum_blks : %d\nnum_last_blk %d\n",fsize,num_blks,num_last_blk);
		}
	} 
     }
     for(i = 0 ; i < num_blks ; i++){
        //read file data
        
	sock_len = read(socketDisc,dataBuf,sizeof(dataBuf));
        printf("read\n\n\n%s\n",dataBuf);        
	if(sock_len <0){
		printf("error\n");
		exit(0);
	}
	size =fwrite(dataBuf,sizeof(char),MAXSIZE,fp);
        if(size<0){
		printf("error\n");
		exit(0);
	}
        recv_fsize=MAXSIZE*(i+1);
        printf("recv data : %d/%d\n",recv_fsize,fsize);
     }
     if(num_last_blk!=0){
             //read last data
             sock_len = read(socketDisc,dataBuf,num_last_blk);
             //dataBuf[num_last_blk]='\0';
             recv_fsize+=num_last_blk;
	     size = fwrite(dataBuf,sizeof(char),num_last_blk,fp);
             //printf("recv data : %d/%d\n",recv_fsize,fsize);
             if(fsize==recv_fsize){
		    printf("All file recv\n");
            }        
                    //write file recv ok             
     }
	 fclose(fp);  
	sock_len = write(socketDisc,"file_recv",sizeof("file_recv"));
	close(socketDisc);
   
} 

/*
void doingstr(int sockid, char name[]){////num&str
int req;
int fail;
int no_read ,tmp, len;
char fname[MAXLINE];
fail = FILENAMEOK;


//printf("filename : %s\n", name);//들어온 파일이름확인
//printf("fail : %d\n", fail);//fail설정값확인

len= strlen(name);//len, 들어온 이름값의 길이확인
printf("filelen : %d\n", len);



len=wnum(sockid,len);//번호쓰기
memset(fname,'\0',MAXLINE);
strncpy(fname,name,len);
wstr(sockid, fname, len);//스트링쓰기
wnum(sockid,FILENAMEOK);//번호쓰기

memset(fname,'\0',MAXLINE);

//char *filename, int sockid ,int len
//req=rnum(sockid,req);//번호읽기

//printf("last req : %d\n",req);
printf("server: FILE TRANSFER COMPLETE on socket %d\n",sockid);
close(sockid);

}
*/


void doingstr(int sockid, char com[]){////num&str
int  len, len2,len3;
char fname[MAXLINE];
char name[MAXLINE];
char *filename="sun.jpeg";

/////len= strlen(com);//len(들어온 명령어의 길이)
/////printf("filelen : %d  %d %s\n", len, filename);
/////len=wnum(sockid,len);//번호쓰기
/*
memset(fname,'\0',MAXLINE);

strncpy(fname,com,len);
wstr(sockid, fname, len);//명령어쓰기

len=wnum(sockid,len2);//번호쓰기
memset(name,'\0',MAXLINE);
//strncpy(name,com,len2);
wstr(sockid, filename, len2);//파일이름쓰기

len3=rnum(sockid,len3);//번호읽기
 printf("len2 : %d\n", len3);
*/
//if(len3==STARTTRANSFER){//답장이 파일을 보낸다는 답장과 같으면
//}

}




void doingfile(int sockid, char name[]){///file

int req;
int fail;
int no_read ,tmp, len;
char fname[MAXLINE];
fail = FILENAMEOK;

//printf("filename : %s\n", name);//들어온 파일이름확인
//printf("fail : %d\n", fail);//fail설정값확인

len= strlen(name);//len, 들어온 이름값의 길이확인
printf("filelen : %d\n", len);

/*
len=wnum(sockid,len);//번호쓰기

wstr(sockid, fname, len);//스트링쓰기
//wnum(sockid,FILENAMEOK);//번호쓰기

memset(fname,'\0',MAXLINE);
*/
strncpy(fname,name,len);//초기화한 fname에 name붙이기
//printf("fname : %s\n", fname);

docftp(name,sockid,len);
//char *filename, int sockid ,int len
//req=rnum(sockid,req);//번호읽기

//printf("last req : %d\n",req);
printf("server: FILE TRANSFER COMPLETE on socket %d\n",sockid);
close(sockid);

}




void doftp(int sockid){ //doftp를 실행한다.

    int newsockid,i,getfile,ack,msg,msg_2,c;
    int no_writen,start_xfer,fsize=0,recv_fsize, num_blks,num_last_blk;
    struct sockaddr_in my_addr, server_addr;
    FILE *fp;
    int string_len,size;
    char t_buf[512];
    char in_buf[MAXSIZE];
    char fname[MAXLINE];
    int file_info;
    no_writen = 0;
    num_blks = 0;
    num_last_blk = 0;
    int req=0;
    //memset(fname,'\0',MAXLINE);
    //printf("name : %s ", fname);

//클라이언트로 부터 파일이름을 읽어들여서 그 파일로 쓴다.
//   fail = FILENAMEOK;
/////////////////클라이언트로부터 파일이름을 읽어들인다.
	memset(fname,'\0',MAXLINE);
      req=rnum(sockid,req);//번호읽기
      printf("get num : %d\n", req);
      //read file name
      sock_len = read(sockid,fname,512);
      //printf("sock_len : %d",sock_len);
      if(sock_len<0){
         printf("client: filename read error :%d\n",errno);
         printf("now im in read error\n");
         file_info = BADFILENAME ;  //fail to 200 is read error
      }
      printf("fname : %s\n\n\n",fname);
      
      if(sock_len>=0){
	 if((fp = fopen(fname,"rb")) == NULL){
		printf(" client: local open file error \n");
		exit(0);
     	 }
         //file send ready!!! 500 is read error
         file_info = STARTTRANSFER;
      	 printf("server read file start !!\n");   
      }      
      
         sprintf(dataBuf,"%d",file_info);
         //write file send OK!!
         sock_len = write(sockid,dataBuf,sizeof(dataBuf));
         if(sock_len <0){
             printf("write file send ok error\n");
	     exit(0);
	 }
         //read file trans info
         sock_len = read(sockid,dataBuf,sizeof(dataBuf));
         if(strcmp(dataBuf,"start")==0){
       		printf("start file send!!\n");
                //write file size
                
	 }
         while ((c = fgetc(fp)) != EOF) {
		fsize++;
	 }

         fp = fopen(fname,"rb");
         printf("fffsize : %d\n\n\n\n",fsize);
    	 num_blks = fsize / MAXSIZE;
    		//num_blks1 = htons(num_blks);
    	 num_last_blk = fsize % MAXSIZE;
         
         //write 1. send fileSize
         sprintf(dataBuf,"%d",fsize);
         sock_len = write(sockid,dataBuf,sizeof(dataBuf));
         printf("size send OK %s\n",dataBuf); 
         //read 1. read fileSize Ok
         sock_len = read(sockid,dataBuf,sizeof(dataBuf));
         if((strcmp(dataBuf,"size_ok"))==0){
        	//write num_blks
                sprintf(dataBuf,"%d",num_blks);
                sock_len = write(sockid,dataBuf,sizeof(dataBuf));
                printf("num_blks send OK temp_buf%s\n",dataBuf);
                //read 2. read num_blks Ok
                sock_len = read(sockid,dataBuf,sizeof(dataBuf));
                if((strcmp(dataBuf,"num_blks_ok"))==0){
                   //write num_last_blk
                   sprintf(dataBuf,"%d",num_last_blk);
                   sock_len = write(sockid,dataBuf,sizeof(dataBuf));
                   printf("num_last_blk send OK %s\n",dataBuf);
                   //read 3. read num_last_blk Ok
                   sock_len = read(sockid,dataBuf,sizeof(dataBuf));
                   if((strcmp(dataBuf,"num_last_ok"))==0){
                        printf("All recv Ok!!\n");
                      
                       for(i=0; i<num_blks ; i ++){
                                //n=fread(buf, sizeof(char) * 2, 1, rfp  
				size = fread(temp_buf,sizeof(char),512,fp);
                                if(size<0){
					printf("error\n");
					exit(0);
				}else {
                                        //write data
                                        recv_fsize=MAXSIZE*(i+1);
                                        //printf("write\n\n\n%s\n",temp_buf);
                                        //printf("\n\n\n file : \n%s\n\n\n",temp_buf);
					sock_len = write(sockid,temp_buf,sizeof(temp_buf));
                                        //printf("send data : %d/%d\n",recv_fsize,fsize);
				}                         
		       }//for
                       
		       //printf("??? : %d",num_last_blk);
		       if(num_last_blk!=0){
		           size = fread(temp_buf,sizeof(char),num_last_blk,fp);
                                if(size<0){
					printf("error\n");
					exit(0);
				}
                           //printf("last_size : %d\n",size);
                           //temp_buf[num_last_blk]='\0';
                           
                           recv_fsize+=num_last_blk;
                           printf("send last : %s\n\n\n\n\n\n",temp_buf);
                           //write last data
                           sock_len = write(sockid,temp_buf,num_last_blk);
                                if(sock_len<0){
					printf("error\n");
					exit(0);
				}// if(sock_len<0)
                           printf("send data : %d/%d\n",recv_fsize,fsize);
                           //read recv info
                           sock_len = read(sockid,dataBuf,sizeof(dataBuf));
                                if(sock_len<0){
					printf("error\n");
					exit(0);
				}// if(sock_len<0)
                           if((strcmp(dataBuf,"file_recv"))==0){
				printf("file send complate!!\n");
                                fclose(fp);
                                close(sockid);
				exit(0);
			        }//if((strcmp(dataBuf,"file_recv"))==0){
		       }//if(num_last_blk!=0){
                     
                       
		   }	// if((strcmp(dataBuf,"num_last_ok"))==0){
		}// if((strcmp(dataBuf,"size_ok"))==0){
	}
}






/*main!*/
void main(int argc, char* args[])
{
	char *fname[2];
	int sockid,i=0;
	fname[0]="text.txt";
	fname[1]="sun.jpeg";
//	fname[1]="CH";
	printf("1: %s\n",fname[0]);
	printf("2: %s\n",fname[1]);
   Command *cmd = malloc(sizeof(Command));
   State *state = malloc(sizeof(State));
   state->logged_in =1;

   //args[1] 로 명령어를 입력받은 후 다음 것을 어떻게 사용할지 결정한다.
   printf("args[1]: %s\n", args[1]);
   strcpy(cmd->command, args[1]);
   printf("command: %s\n", cmd->command);

   if(cmd->command[0]<=127 || cmd->command[0]>=0){ //is command ASKII code
      response(cmd,state); //user function
   }


}
