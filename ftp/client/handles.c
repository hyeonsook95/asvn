#include "common.h"
#ifndef sendfile
#define BUF_SIZE 8192
ssize_t sendfile(int out_fd, int in_fd, off_t * offset, size_t count )
{//off_t long형 정수
//파일을 보내는 함 수
 
    off_t orig;
    char buf[BUF_SIZE];
    size_t toRead, numRead, numSent, totSent;

    if (offset != NULL) {
        /* Save current file offset and set offset to value in '*offset' */
        //offset !=null , 파일이 있을 때 
        orig = lseek(in_fd, 0, SEEK_CUR);//in_fd를 0만큼 현재커서에서 읽어온다. 
        if (orig == -1)//에러처리 
            return -1;
        if (lseek(in_fd, *offset, SEEK_SET) == -1)//여기에서 offset이 변경되므로 
            return -1;
    }

    totSent = 0;
    while (count > 0) {
        toRead = count<BUF_SIZE ? count : BUF_SIZE;//파일의 크기가 버퍼사이즈보다 작으면 count 아니면 버퍼사이즈를 읽는다. 

        numRead = read(in_fd, buf, toRead);//buf로 toRead만큼in_fd에 읽어들인다. 
		if (numRead == -1)//오류 
            return -1;
        if (numRead == 0)//읽을게 없다. 
            break;                      /* EOF */

        numSent = write(out_fd, buf, numRead);//읽어들인 만큼 buf로 읽어들여서 out_fd로 쓴다. 
        if (numSent == -1)//에러 
            return -1;
        if (numSent == 0) {//쓸게 없다.       /* Should never happen */
            perror("sendfile: write() transferred 0 bytes");
            exit(-1);
        }

        count -= numSent; //count는 쓴만큼 -- 
        totSent += numSent;//totSent는 보낸만큼 값을 ++ 
    }

    if (offset != NULL) { //offset을 초기화 
        /* Return updated file offset in '*offset', and reset the file offset
           to the value it had when we were called. */
        *offset = lseek(in_fd, 0, SEEK_CUR); //in_fd에서 0부터 현재까지 읽어들임. 
        if (*offset == -1)
            return -1;
        if (lseek(in_fd, orig, SEEK_SET) == -1)//orig부터 처음까지 in_fd 저장, 오류 처리 
            return -1;
    }
    return totSent;//총 보낸 값을 저장 
}
#endif

/**
 * Generates response message for client
 * @param cmd Current command
 * @param state Current connection state
 */
void response(Command *cmd, State *state)
{ //입력한 명령어를 판단하는 함수 
  switch(lookup_cmd(cmd->command)){//명령어를 들고와서 어떤 명령어와 일치하는지 비교 
  //enum이라서 숫자로 비교가능! 
    case USER: ftp_user(cmd,state); break;
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
    case ABOR: ftp_abor(state); break;
    case QUIT: ftp_quit(state); break;
    case TYPE: ftp_type(cmd,state); break;
    case NOOP:
      if(state->logged_in){//로그인 상태면 
        state->message = "200 Nice to NOOP you!\n";
      }else{ //등록되지 않은 사람임을 알려줌 
        state->message = "530 NOOB hehe.\n";
      }
      write_state(state);//현재상태를 기록 
      break;
    default: // 어떤 명령어에도 포함되지 않는 경우 
      state->message = "500 Unknown command\n";
      write_state(state);
      break;
  }
}

/**
 * Handle USER command
 * @param cmd Command with args
 * @param state Current client connection state
 */
void ftp_user(Command *cmd, State *state)
{ //사용자 로그인 
  const int total_usernames = sizeof(usernames)/sizeof(char *);//유저이름이라고 등록된 총 길이를 계산 
  if(lookup(cmd->arg,usernames,total_usernames)>=0){
    state->username = malloc(32); //username크기를 잡는다. 
    memset(state->username,0,32); //username을 0으로 32만큼 초기화 
    strcpy(state->username,cmd->arg);//arg로 들어와 있는 값을 username에 넣는다. 
    state->username_ok = 1;//사용자의 이름체크를 1로 설정
    state->message = "331 User name okay, need password\n";//username 통과, 
  }else{ //등록할 수 없는 username임! 
    state->message = "530 Invalid username\n";
  }
  write_state(state);
}

/** PASS command */
void ftp_pass(Command *cmd, State *state)
{
  if(state->username_ok==1){//username 등록이 통과되었으면 
    state->logged_in = 1; // logged_in상태를 1로 변경 
    state->message = "230 Login successful\n"; // login 성공 
  }else{ //username이 통과 안되었으면 알맞지 않은 사용자임을 알림 
    state->message = "500 Invalid username or password\n";
  }
  write_state(state);
}

/** PASV command */
void ftp_pasv(Command *cmd, State *state)
{
  if(state->logged_in){
    int ip[4];//IP가 127.0.0.1처럼 네 등분으로 들어가니까! 
    char buff[255];
    char *response = "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\n";
    /*
	typedef struct Port
	{
	  int p1;
	  int p2;
	} Port;
	*/
    Port *port = malloc(sizeof(Port)); //Port입력받은 크기 만큼을 저장 
    gen_port(port);//랜덤하게 p1, p2의 값을 구함. 
    getip(state->connection,ip);//getip(int sock, int *ip) 

    /* Close previous passive socket? */
    close(state->sock_pasv); //이전에 열린 소켓 닫기 

    /* Start listening here, but don't accept the connection */
    state->sock_pasv = create_socket((256*port->p1)+port->p2); //소켓생성 
    printf("port: %d\n",256*port->p1+port->p2);
    sprintf(buff,response,ip[0],ip[1],ip[2],ip[3],port->p1,port->p2);///////////기록하는건가..? 
    state->message = buff; //Client에 보낼 메세지에 buff 내용을 채운다. 
    state->mode = SERVER; //Connection mode: NORMAL, SERVER, CLIENT 를 SERVER로 설정. 
    puts(state->message); //state->message를 출력

  }else{ //login 해주세요. 
    state->message = "530 Please login with USER and PASS.\n" ;
    printf("%s",state->message);
  }
  write_state(state);
}

/** LIST command */
void ftp_list(Command *cmd, State *state)
{
  if(state->logged_in==1){ //로그인 된 상태면 
  /**
  	struct dirent{
	  long d_ino; //inode 넘버
	  off_t d_off; //offset
	  unsigned short d_reclen; //d_name 길이
	  char d_name[NAME_MAX+1]; //파일이름 
	}
	
	struct tm - 날짜와 시간을 나타내는 구조체이다.{
		int tm_sec - 초(0-59)
		int tm_min - 분(0-59)
		int tm_hour - 시(0-23)
		int tm_mday - 일(1-31)
		int tm_mon - 월(0-11)
		int tm_year - 년(1900년 기준)
		int tm_wday - 요일(일요일부터 시작, 0-6)
		int tm_yday - 연중 일자(0-365)
		int tm_isdst - 서머타임 설정 여부
		}
		
	struct stat {
     dev_t           st_dev;        /*ID of device containing file 
     ino_t            st_ino;          /*inode number
     mode_t       st_mode;     /*protection
     nlink_t         st_nlink;       /*number of hard links
     uid_t            st_uid;          /*user ID of owner
     gid_t            st_gid;          /*group ID of owner
     dev_t           st_rdev;        /*device ID (if special file)
     off_t             st_size;         /*total size, in byte
     blksize_t      st_blksize;    /*blocksize for file system I/O
     blkcnt_t       st_blocks;     /*number of 512B blocks allocated
     time_t;        st_atime;      /*time of last access
     time_t;        st_mtime;     /*time of last modification
     time_t         st_xtime;       /*time of last status change
		}
  	**/
    struct dirent *entry; //디렉터리의 정보를 저장하는 구조체 
    struct stat statbuf;//파일에 대한 정보를 알려주는 구조체 
    struct tm *time;//시간정보를 저장하는 구조체
    char timebuff[80], current_dir[BSIZE];
    int connection;
    time_t rawtime;//long타입 

    /* TODO: dynamic buffering maybe? */
    char cwd[BSIZE], cwd_orig[BSIZE]; //1024
    memset(cwd,0,BSIZE); //0으로 BSIZE만큼 채우기 
    memset(cwd_orig,0,BSIZE); //0으로 BSIZE만큼 cwd_orig를 채운다. 
    
    /* Later we want to go to the original path */
    getcwd(cwd_orig,BSIZE); //현재 작업디렉토리를 저장, cwd_orig에 BSIZE만큼 담는다. 
    //현재 디렉토리 경로를 cwd_orig 저장 
    
    /* Just chdir to specified path */
    if(strlen(cmd->arg)>0&&cmd->arg[0]!='-'){//cmd->arg가 안에 내용있고 들어간 내용이 '-'아니면 
      chdir(cmd->arg);//현재 작업 디렉토리를 변경한다.  cmd->arg로 
    }
    
    getcwd(cwd,BSIZE); //현재 작업디렉토리경로를 cwd에 저장 
    DIR *dp = opendir(cwd); //cwd 경로를 연다.

    if(!dp){//dp가 false면 
      state->message = "550 Failed to open directory.\n";
    }else{//아니면 mode를 SERVER로 접속 
      if(state->mode == SERVER){ //SERVER상태라면 

        connection = accept_connection(state->sock_pasv);
		// state->sock_pasv? Socket for passive connection (must be accepted later)
		//return accept(socket,(struct sockaddr*) &client_address,&addrlen);
		
        state->message = "150 Here comes the directory listing.\n";
        puts(state->message); //Response message to client

        while(entry=readdir(dp)){ 
			//struct dirent *readdir( DIR *dir);
			//opendir()에서 열기한 디렉토리 정보
			//entry는 
			// 디렉토리 안에 있는 모든 파일과 디렉토리 출력
			
          if(stat(entry->d_name,&statbuf)==-1){ 
          //파일이름이  statbuf안에 있는 이름과 일치하지 않으면..? 
          //statbuf를 가지고 읽는 듯 
            fprintf(stderr, "FTP: Error reading file stats...\n");
            //파일 읽기 실패.. 
          }else{
            char *perms = malloc(9); //9만큼 malloc 
            memset(perms,0,9);//초기화 

            /* Convert time_t to tm struct */
            rawtime = statbuf.st_mtime; //time of last modification
            time = localtime(&rawtime); 
			//struct tm * localtime ( const time_t * timer );
            strftime(timebuff,80,"%b %d %H:%M",time);
            //timebuff에 80만큼 ""같은 형식으로 time을 저장하라. 
            
            
            str_perm((statbuf.st_mode & ALLPERMS), perms);////////////
            
            dprintf(connection,
                "%c%s %5d %4d %4d %8d %s %s\r\n", 
                (entry->d_type==DT_DIR)?'d':'-',
                perms,statbuf.st_nlink,
                statbuf.st_uid, 
                statbuf.st_gid,
                statbuf.st_size,
                timebuff,
                entry->d_name);//sprintf와 write를 연달아 쓰는 함수 
                
            /*
            buf내용을 len만큼! 
			write(connection,
                buf,len );
                +
			len = sprintf("%c%s %5d %4d %4d %8d %s %s\r\n", 
                (entry->d_type==DT_DIR)?'d':'-',
                perms,statbuf.st_nlink,
                statbuf.st_uid, 
                statbuf.st_gid,
                statbuf.st_size,
                timebuff,
                entry->d_name)) ;
            */
          }
        }
        write_state(state);
        state->message = "226 Directory send OK.\n";
        state->mode = NORMAL;
        close(connection);
        close(state->sock_pasv);//pasv connection을 닫는다. 

      }else if(state->mode == CLIENT){ //client면 
        state->message = "502 Command not implemented.\n"; //command가 포함되지 않습니다. 
      }else{
        state->message = "425 Use PASV or PORT first.\n"; //pasv나 port를 먼저 설정하세요. 
      }
    }
    closedir(dp);//
    chdir(cwd_orig); //cwd_orig경로로 바꾼다. 
  }else{ //로그인 안됬으면 
    state->message = "530 Please login with USER and PASS.\n";
  }
  state->mode = NORMAL;// NORMAL상태로 돌린다. 
  write_state(state);
}


/** QUIT command */
void ftp_quit(State *state)
{ //통신 끝내기 
  state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
  write_state(state); //상태작성 
  close(state->connection); //닫기 
  exit(0);
}

/** PWD command */
void ftp_pwd(Command *cmd, State *state)
{ //현재 경로를 저장 
  if(state->logged_in){
    char cwd[BSIZE];
    char result[BSIZE];
    memset(result, 0, BSIZE);
    if(getcwd(cwd,BSIZE)!=NULL){
      strcat(result,"257 \"");
      strcat(result,cwd);
      strcat(result,"\"\n");
      state->message = result;
    }else{
      state->message = "550 Failed to get pwd.\n";
    }
    write_state(state);
  }
}

/** CWD command */
void ftp_cwd(Command *cmd, State *state)
{ //디렉터리 변경 
  if(state->logged_in){
  	
  	/*
typedef struct Command
{
  char command[5];
  char arg[BSIZE];
} Command;
*/

    if(chdir(cmd->arg)==0){
      state->message = "250 Directory successfully changed.\n";
    }else{
      state->message = "550 Failed to change directory.\n";
    }
  }else{
    state->message = "500 Login with USER and PASS.\n";
  }
  write_state(state);

}

/** 
 * MKD command 
 * TODO: full path directory creation
 */
void ftp_mkd(Command *cmd, State *state)
{
  if(state->logged_in){
    char cwd[BSIZE];
    char res[BSIZE];
    memset(cwd,0,BSIZE);
    memset(res,0,BSIZE);
    getcwd(cwd,BSIZE);

    /* TODO: check if directory already exists with chdir? */

    /* Absolute path */
    if(cmd->arg[0]=='/'){
      if(mkdir(cmd->arg,S_IRWXU)==0){
        strcat(res,"257 \"");
        strcat(res,cmd->arg);
        strcat(res,"\" new directory created.\n");
        state->message = res;
      }else{
        state->message = "550 Failed to create directory. Check path or permissions.\n";
      }
    }
    /* Relative path */
    else{
      if(mkdir(cmd->arg,S_IRWXU)==0){ //cmd_arg : 경로 입력 
        sprintf(res,"257 \"%s/%s\" new directory created.\n",cwd,cmd->arg);
        state->message = res;
      }else{
        state->message = "550 Failed to create directory.\n";
      }
    }
  }else{
    state->message = "500 Good news, everyone! There's a report on TV with some very bad news!\n";
  }
  write_state(state);
}

/** RETR command */
void ftp_retr(Command *cmd, State *state)
{

  if(fork()==0){
    int connection;
    int fd;
    struct stat stat_buf;
    off_t offset = 0;
    int sent_total = 0;
    if(state->logged_in){

      /* Passive mode */
      if(state->mode == SERVER){
        if(access(cmd->arg,R_OK)==0 && (fd = open(cmd->arg,O_RDONLY))){ //access : 지정된 파일이 존재하는지 확인 
          fstat(fd,&stat_buf); //stat구조체 정보를 모두 저장 
          
          state->message = "150 Opening BINARY mode data connection.\n";
          
          write_state(state);
          
          connection = accept_connection(state->sock_pasv);/////////////////////////////////////////////////////////
          close(state->sock_pasv);
          if(sent_total = sendfile(connection, fd, &offset, stat_buf.st_size)){
          	
            if(sent_total != stat_buf.st_size){
              perror("ftp_retr:sendfile");
              exit(EXIT_SUCCESS);
            }

            state->message = "226 File send OK.\n";
          }else{
            state->message = "550 Failed to read file.\n";
          }
        }else{
          state->message = "550 Failed to get file\n";
        }
      }else{
        state->message = "550 Please use PASV instead of PORT.\n";
      }
    }else{
      state->message = "530 Please login with USER and PASS.\n";
    }

    close(fd);
    close(connection);
    write_state(state);
    exit(EXIT_SUCCESS);
  }
  state->mode = NORMAL;
  close(state->sock_pasv);
}

/** Handle STOR command. TODO: check permissions. */
void ftp_stor(Command *cmd, State *state)
{
  if(fork()==0){
    int connection, fd;
    off_t offset = 0;
    int pipefd[2];
    int res = 1;
    const int buff_size = 8192;

    FILE *fp = fopen(cmd->arg,"w");

    if(fp==NULL){
      /* TODO: write status message here! */
      perror("ftp_stor:fopen");
    }else if(state->logged_in){
      if(!(state->mode==SERVER)){
        state->message = "550 Please use PASV instead of PORT.\n";
      }
      /* Passive mode */
      else{
        fd = fileno(fp);//파일 핸들을 판별
        connection = accept_connection(state->sock_pasv);
        close(state->sock_pasv);
        if(pipe(pipefd)==-1)perror("ftp_stor: pipe");

        state->message = "125 Data connection already open; transfer starting.\n";
        write_state(state);

        /* Using splice function for file receiving.
         * The splice() system call first appeared in Linux 2.6.17.
         */
		//커널 공간내에서 파일 수신내에서 splice 이용가능 . 
        while ((res = splice(connection, 0, pipefd[1], NULL, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE))>0){
        	//splice write와 read를 대신하는 함수. 읽어들인 크기를 반환 connection->pipefd에 읽어들여옴 
          splice(pipefd[0], NULL, fd, 0, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE);
          //pipefd에서 fd로 buff_size만큼 내용이동 
        }

        /* TODO: signal with ABOR command to exit */

        /* Internal error */
        if(res==-1){
          perror("ftp_stor: splice");
          exit(EXIT_SUCCESS);
        }else{
          state->message = "226 File send OK.\n";
        }
        close(connection);
        close(fd);
      }
    }else{
      state->message = "530 Please login with USER and PASS.\n";
    }
    close(connection);
    write_state(state);
    exit(EXIT_SUCCESS);
  }
  state->mode = NORMAL;
  close(state->sock_pasv);

}

/** ABOR command */
void ftp_abor(State *state)
{
  if(state->logged_in){
    state->message = "226 Closing data connection.\n";
    state->message = "225 Data connection open; no transfer in progress.\n";
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  write_state(state);

}

/** 
 * Handle TYPE command.
 * BINARY only at the moment.
 */
void ftp_type(Command *cmd,State *state)
{//자료 상태를 설정 
  if(state->logged_in){
    if(cmd->arg[0]=='I'){
      state->message = "200 Switching to Binary mode.\n";
    }else if(cmd->arg[0]=='A'){

      /* Type A must be always accepted according to RFC */
      state->message = "200 Switching to ASCII mode.\n";
    }else{
      state->message = "504 Command not implemented for that parameter.\n";
    }
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  write_state(state);
}

/** Handle DELE command */
void ftp_dele(Command *cmd,State *state)
{
  if(state->logged_in){
    if(unlink(cmd->arg)==-1){ //매개변수 받은 것의 명령에 대해 삭제. 
      state->message = "550 File unavailable.\n";
    }else{
      state->message = "250 Requested file action okay, completed.\n";
    }
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  write_state(state);
}

/** Handle RMD */
void ftp_rmd(Command *cmd, State *state)
{ //rmdir
  if(!state->logged_in){
    state->message = "530 Please login first.\n";
  }else{
    if(rmdir(cmd->arg)==0){
      state->message = "250 Requested file action okay, completed.\n";
    }else{
      state->message = "550 Cannot delete directory.\n";
    }
  }
  write_state(state);

}

/** Handle SIZE (RFC 3659) */
void ftp_size(Command *cmd, State *state)
{ //
  if(state->logged_in){
    struct stat statbuf;
    char filesize[128];
    memset(filesize,0,128);
    /* Success */
    if(stat(cmd->arg,&statbuf)==0){
      sprintf(filesize, "213 %d\n", statbuf.st_size);
      state->message = filesize;
    }else{
      state->message = "550 Could not get file size.\n";
    }
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }

  write_state(state);

}

/** 
 * Converts permissions to string. e.g. rwxrwxrwx 
 * @param perm Permissions mask
 * @param str_perm Pointer to string representation of permissions
 */
void str_perm(int perm, char *str_perm)
{
  int curperm = 0;
  int flag = 0;
  int read, write, exec;
  
  /* Flags buffer */
  char fbuff[3];

  read = write = exec = 0;
  
  int i;
  for(i = 6; i>=0; i-=3){
    /* Explode permissions of user, group, others; starting with users */
    curperm = ((perm & ALLPERMS) >> i ) & 0x7;
    
    memset(fbuff,0,3);
    /* Check rwx flags for each*/
    read = (curperm >> 2) & 0x1;
    write = (curperm >> 1) & 0x1;
    exec = (curperm >> 0) & 0x1;

    sprintf(fbuff,"%c%c%c",read?'r':'-' ,write?'w':'-', exec?'x':'-');
    strcat(str_perm,fbuff);

  }
}
