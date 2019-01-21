#include "common.h"
#ifndef sendfile
#define BUF_SIZE 8192
ssize_t sendfile(int out_fd, int in_fd, off_t * offset, size_t count )
{
    off_t orig;
    //long
    char buf[BUF_SIZE];
    size_t toRead, numRead, numSent, totSent;
    //int(16bit) 

    if (offset != NULL) { //현재 저장한 파일이 있을 때 
        /* Save current file offset and set offset to value in '*offset' */
        orig = lseek(in_fd, 0, SEEK_CUR); //현재커서에서 0바이트만큼 이동, 이동한 포인터 반환, 이동하기 전의 포인터를 orig에 저장 
        if (orig == -1) //변경된 포인터를 반환
            return -1;
        if (lseek(in_fd, *offset, SEEK_SET) == -1) // 파일의 시작에서 offset만큼 이동한 값이 실패일때, 
            return -1;
    }

    totSent = 0;
    while (count > 0) {
        toRead = count<BUF_SIZE ? count : BUF_SIZE;

        numRead = read(in_fd, buf, toRead);
        if (numRead == -1)
            return -1;
        if (numRead == 0)
            break;                      /* EOF */

        numSent = write(out_fd, buf, numRead);
        if (numSent == -1)
            return -1;
        if (numSent == 0) {               /* Should never happen */
            perror("sendfile: write() transferred 0 bytes");
            exit(-1);
        }

        count -= numSent; //총 파일의 크기 -= 보낸 크기 
        totSent += numSent; //총 보낸 크기 += 현재 보낸 크기 
    }

    if (offset != NULL) {
        /* Return updated file offset in '*offset', and reset the file offset
           to the value it had when we were called. */
        *offset = lseek(in_fd, 0, SEEK_CUR);
        if (*offset == -1)
            return -1;
        if (lseek(in_fd, orig, SEEK_SET) == -1) //offset을 재설정 
            return -1;
    }
    
    return totSent; //총 보낸 값을 반환 
}
#endif

/**
 * Generates response message for client
 * @param cmd Current command
 * @param state Current connection state
 */
void response(Command *cmd, State *state)
{
  switch(lookup_cmd(cmd->command)){ //저장된 배열의 순서 값을 enum으로 대치해줌 
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
    case NOOP: //니생각: 로그인 하기 전에 보여주는  
      if(state->logged_in){
        state->message = "200 Nice to NOOP you!\n";
      }else{
        state->message = "530 NOOB hehe.\n";
      }
      write_state(state);
      break;
    default: 
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
{
  const int total_usernames = sizeof(usernames)/sizeof(char *);
  if(lookup(cmd->arg,usernames,total_usernames)>=0){ //배열 순서를 반환해줌 
    state->username = malloc(32);
    memset(state->username,0,32);
    strcpy(state->username,cmd->arg);
    state->username_ok = 1;
    state->message = "331 User name okay, need password\n";
  }else{
    state->message = "530 Invalid username\n";
  }
  write_state(state);
}

/** PASS command */
void ftp_pass(Command *cmd, State *state)
{
  if(state->username_ok==1){
    state->logged_in = 1;
    state->message = "230 Login successful\n";
  }else{
    state->message = "500 Invalid username or password\n";
  }
  write_state(state);
}

/** PASV command */
void ftp_pasv(Command *cmd, State *state)
{
  if(state->logged_in){
    int ip[4];
    char buff[255];
    char *response = "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\n";
    Port *port = malloc(sizeof(Port));
    gen_port(port);
    getip(state->connection,ip);

    /* Close previous passive socket? */
    close(state->sock_pasv);

    /* Start listening here, but don't accept the connection */
    state->sock_pasv = create_socket((256*port->p1)+port->p2);
    printf("port: %d\n",256*port->p1+port->p2);
    sprintf(buff,response,ip[0],ip[1],ip[2],ip[3],port->p1,port->p2);
    state->message = buff;
    state->mode = SERVER;
    puts(state->message);

  }else{
    state->message = "530 Please login with USER and PASS.\n";
    printf("%s",state->message);
  }
  write_state(state);
}

/** LIST command */
void ftp_list(Command *cmd, State *state)
{
  if(state->logged_in==1){
    struct dirent *entry;
    struct stat statbuf;
    struct tm *time;
    char timebuff[80], current_dir[BSIZE];
    int connection;
    time_t rawtime;

    /* TODO: dynamic buffering maybe? */
    char cwd[BSIZE], cwd_orig[BSIZE];
    memset(cwd,0,BSIZE);
    memset(cwd_orig,0,BSIZE);
    
    /* Later we want to go to the original path */
    getcwd(cwd_orig,BSIZE);
    
    /* Just chdir to specified path */
    if(strlen(cmd->arg)>0&&cmd->arg[0]!='-'){
      chdir(cmd->arg);
    }
    
    getcwd(cwd,BSIZE);
    DIR *dp = opendir(cwd);

    if(!dp){
      state->message = "550 Failed to open directory.\n";
    }else{
      if(state->mode == SERVER){

        connection = accept_connection(state->sock_pasv);
        state->message = "150 Here comes the directory listing.\n";
        puts(state->message);

        while(entry=readdir(dp)){
          if(stat(entry->d_name,&statbuf)==-1){
            fprintf(stderr, "FTP: Error reading file stats...\n");
          }else{
            char *perms = malloc(9);
            memset(perms,0,9);

            /* Convert time_t to tm struct */
            rawtime = statbuf.st_mtime;
            time = localtime(&rawtime);
            strftime(timebuff,80,"%b %d %H:%M",time);
            //형식대로, 80바이트 안에서 버퍼에 저장 
            str_perm((statbuf.st_mode & ALLPERMS), perms);
            dprintf(connection,
                "%c%s %5d %4d %4d %8d %s %s\r\n", 
                (entry->d_type==DT_DIR)?'d':'-',
                perms,statbuf.st_nlink,
                statbuf.st_uid, 
                statbuf.st_gid,
                statbuf.st_size,
                timebuff,
                entry->d_name);
          }
        }
        write_state(state);
        state->message = "226 Directory send OK.\n";
        state->mode = NORMAL;
        close(connection);
        close(state->sock_pasv);

      }else if(state->mode == CLIENT){
        state->message = "502 Command not implemented.\n";
      }else{
        state->message = "425 Use PASV or PORT first.\n";
      }
    }
    closedir(dp);
    chdir(cwd_orig);
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  state->mode = NORMAL;
  write_state(state);
}


/** QUIT command */
void ftp_quit(State *state)
{					//안녕 친구. 내가 이런 식으로 죽을 줄 몰랐어! 
  state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
  write_state(state);
  close(state->connection);
  exit(0);
}

/** PWD command */
void ftp_pwd(Command *cmd, State *state)
{
  if(state->logged_in){ //로그인 된 상태이면 
    char cwd[BSIZE];
    char result[BSIZE];
    memset(result, 0, BSIZE);
    if(getcwd(cwd,BSIZE)!=NULL){ //현재 작업 디렉토리의 이름을 얻어옴 
      strcat(result,"257 \"");
      strcat(result,cwd);
      strcat(result,"\"\n");
      state->message = result; // 257 \"cwd\"
    }else{
      state->message = "550 Failed to get pwd.\n";
    }
    write_state(state);
  }
}

/** CWD command */
void ftp_cwd(Command *cmd, State *state)
{
  if(state->logged_in){
    if(chdir(cmd->arg)==0){ //현재 작업 디렉토릴르 cmd->arg로 이동한다. 
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
 * 전체 경로 디렉토리 생성 
 */
void ftp_mkd(Command *cmd, State *state)
{
  if(state->logged_in){ //로그인 된 상태라면 
    char cwd[BSIZE];
    char res[BSIZE];
    memset(cwd,0,BSIZE);
    memset(res,0,BSIZE);
    getcwd(cwd,BSIZE); //현재 작업 디렉토리의 이름을 얻어옴 

    /* TODO: check if directory already exists with chdir? */
    // chdir에 이미 디렉토리가 있는지 확인 

    /* Absolute path */
    //절대경로 
    if(cmd->arg[0]=='/'){ // / 이면 
      if(mkdir(cmd->arg,S_IRWXU)==0){
      	// , read+write+execute(user) 로 만들어서 
        strcat(res,"257 \""); 
        strcat(res,cmd->arg);
        strcat(res,"\" new directory created.\n"); // 257 \ cmd->arg\" new directory created.\n"
        state->message = res;
      }else{
      					//디렉토리를 만들지 못했다. 권한 또는 경로를 확인하라. 
        state->message = "550 Failed to create directory. Check path or permissions.\n";
      }
    }
    /* Relative path */
    //상대 경로 
    else{ // /이 아니면 
      if(mkdir(cmd->arg,S_IRWXU)==0){
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

  if(fork()==0){ //자식 프로세스 생성했을 경우 실행 
    int connection;
    int fd;
    struct stat stat_buf;
    off_t offset = 0;
    int sent_total = 0;
    if(state->logged_in){ //로그인된 경우 

      /* Passive mode */
      if(state->mode == SERVER){ //서버일 경우 
        if(access(cmd->arg,R_OK)==0 && (fd = open(cmd->arg,O_RDONLY))){ //지정 파일이 존재하고 열면 
        	//(지정 파일이 존재하는지, 읽기 가능?) && RDONLY로 염 
          fstat(fd,&stat_buf); //파일 정보를 읽어오는 함수
          
          state->message = "150 Opening BINARY mode data connection.\n";
          
          write_state(state);
          
          connection = accept_connection(state->sock_pasv);
          close(state->sock_pasv);
          if(sent_total = sendfile(connection, fd, &offset, stat_buf.st_size)){
            //https://kldp.org/node/123849, https://msdn.microsoft.com/ko-kr/library/sx0a40c2(v=vs.110).aspx
            //파일 기술자 사이의 데이터 전송, 디스크립터에서 다른 디스크립터로 
            //read, write를 커널 내에서 
            
            if(sent_total != stat_buf.st_size){
              perror("ftp_retr:sendfile");
              exit(EXIT_SUCCESS);
            }

            state->message = "226 File send OK.\n";
          }else{ //아닐 경우 
            state->message = "550 Failed to read file.\n";
          }
        }else{ //지정 파일이 존재하지 않을때
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
//								권한 확인
void ftp_stor(Command *cmd, State *state)
{
  if(fork()==0){ //프로세스 생성 
    int connection, fd;
    off_t offset = 0;
    int pipefd[2];
    int res = 1;
    const int buff_size = 8192;

    FILE *fp = fopen(cmd->arg,"w"); //w로 open 

    if(fp==NULL){ //open 실패일 경우 
      /* TODO: write status message here! */
      //상태 메시지를 씀 
      perror("ftp_stor:fopen");
    }else if(state->logged_in){ //open하고, 로그인 상태일 경우 
      if(!(state->mode==SERVER)){ //SERVER 모드가 아닐 경우, port대신 pasv 사용을 권장 
        state->message = "550 Please use PASV instead of PORT.\n";
      }
      /* Passive mode */
      else{
        fd = fileno(fp); //fp와 연관된 파일 핸들을 판단
        connection = accept_connection(state->sock_pasv);
        close(state->sock_pasv); //소켓 close 
        if(pipe(pipefd)==-1)perror("ftp_stor: pipe");
        //pipe 생성해서 실패할 경우 error msg 

        state->message = "125 Data connection already open; transfer starting.\n";
        //					데이터 연결이 이미 열려있음, 전송을 시작합니다. 
        write_state(state);

        /* Using splice function for file receiving.
         * 파일 수신에 splice 기능 사용가능 
         * The splice() system call first appeared in Linux 2.6.17.
         * 커널 공간 내의 파이프에서 다른 파이프로 데이터 전송 가능 
         */

        while ((res = splice(connection, 0, pipefd[1], NULL, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE))>0){
        			  //connection에서 pipefd[1]로 buff_size 만큼, 읽어들인 크기를 반환 
          splice(pipefd[0], NULL, fd, 0, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE);
          //pipefd[0]에서 fd로 buff_size 만큼 
        }

        /* TODO: signal with ABOR command to exit */

        /* Internal error */
        //내부 오류 
        if(res==-1){
          perror("ftp_stor: splice");
          exit(EXIT_SUCCESS);
        }else{
          state->message = "226 File send OK.\n";
        }
        close(connection);
        close(fd);
      }
    }else{ //open하고 로그인 상태가 아닐 경우 
      state->message = "530 Please login with USER and PASS.\n";
    }
    close(connection);
    write_state(state);
    exit(EXIT_SUCCESS);
  }
  state->mode = NORMAL; //전달할 것이 없음 
  close(state->sock_pasv);
}

/** ABOR command */
void ftp_abor(State *state)
{
  if(state->logged_in){ //로그인 된 상태이면 
    state->message = "226 Closing data connection.\n";
    				//데이터 연결이 열려있습니다. 진행중인 전송이 없습니다. 
    state->message = "225 Data connection open; no transfer in progress.\n";
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  write_state(state);

}

/** 
 * Handle TYPE command.
 * TYPE 명령 처리 
 * BINARY only at the moment.
 * 순간만 바이너리 
 */
void ftp_type(Command *cmd,State *state)
{
  if(state->logged_in){ //로그인된 상태이면 
    if(cmd->arg[0]=='I'){ //
      state->message = "200 Switching to Binary mode.\n";
    }else if(cmd->arg[0]=='A'){

      /* Type A must be always accepted according to RFC */
      // A는 RFC에 따라 항상 허용되어야 한다. 
      state->message = "200 Switching to ASCII mode.\n";
    }else{
    					//해당 매개변수에 대해 명령이 구현되지 않았다. 
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
    if(unlink(cmd->arg)==-1){ //링크 삭제 실패했을 경우 
      state->message = "550 File unavailable.\n";
    }else{ //성공 
      state->message = "250 Requested file action okay, completed.\n";
    }
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }
  write_state(state);
}

/** Handle RMD */
void ftp_rmd(Command *cmd, State *state)
{
  if(!state->logged_in){
    state->message = "530 Please login first.\n";
  }else{
    if(rmdir(cmd->arg)==0){ 
      //요청한 파일을 완료했습니다. 
      state->message = "250 Requested file action okay, completed.\n";
    }else{
      state->message = "550 Cannot delete directory.\n";
    }
  }
  write_state(state);

}

/** Handle SIZE (RFC 3659) */
void ftp_size(Command *cmd, State *state)
{
  if(state->logged_in){ //로그인된 상태이면 
    struct stat statbuf; //파일의 정보를 담는 구조체 
    char filesize[128];
    memset(filesize,0,128); //filesize에 메모리 할당 
    /* Success */
    if(stat(cmd->arg,&statbuf)==0){ //파일정보를 읽어옴(경로, 버퍼) 성공 
      sprintf(filesize, "213 %d\n", statbuf.st_size); //filesize에 "213 사이즈" 넣음 
      state->message = filesize;
    }else{ //아니면 
      state->message = "550 Could not get file size.\n";
    }
  }else{
    state->message = "530 Please login with USER and PASS.\n";
  }

  write_state(state);

}

/** 
 * Converts permissions to string. e.g. rwxrwxrwx 
 * 사용권한을 문자열로 변환함 
 * @param perm Permissions mask 권한 마스크 
 * @param str_perm Pointer to string representation of permissions
 				사용권한 string의 * 
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
  for(i = 6; i>=0; i-=3){ //각 권한들이 3비트씩 차지하므로 -3씩 한다. 
    /* Explode permissions of user, group, others; starting with users */
    // 사용자, 그룹, 그외로 분해 
    curperm = ((perm & ALLPERMS) >> i ) & 0x7; // & 7
    
    memset(fbuff,0,3);
    /* Check rwx flags for each*/
    //각 rwx 플래그 확인 
    read = (curperm >> 2) & 0x1; // (curperm/2^2)& 1
    write = (curperm >> 1) & 0x1;
    exec = (curperm >> 0) & 0x1;

    sprintf(fbuff,"%c%c%c",read?'r':'-' ,write?'w':'-', exec?'x':'-');
    						//read가 r이면 r로, 아니면 - 
    strcat(str_perm,fbuff);
	
  }
}
