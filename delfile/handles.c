//ftp_handles.c
#include "common.h"
#ifndef sendfile
#define BUF_SIZE 8192
<<<<<<< HEAD
ssize_t sendfile(int out_fd, int in_fd, off_t * offset, size_t count )
{//off_t long�� ����
//������ ������ �� ��
 
=======


//////////////////////////////////////////////////////////
//.asvn을 현재 작업 디렉토리에 생성. ->trunk일 때, 함께 생성. trunk 생성을 로그에 기록
void hs_cret(Command *cmd, State *state)
{
	char dirName[200];
	char *path
	int fp;

	//현재 위치에 .asvn 생성
	getcwd(dirName, 200);
	path = dirName;
	strcat(dirName, "/.asvn");
	mkdir(dirName, 0777);

	strcat(dirName, "/log");
	fp = creat(dirName, 0644);
}

//////////////////////////////////////////////////////////
ssize_t sendfile(int out_fd, int in_fd, off_t * offset, size_t count )
{
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
    off_t orig;
    char buf[BUF_SIZE];
    size_t toRead, numRead, numSent, totSent;

    if (offset != NULL) {
        /* Save current file offset and set offset to value in '*offset' */
<<<<<<< HEAD
        //offset !=null , ������ ���� �� 
        orig = lseek(in_fd, 0, SEEK_CUR);//in_fd�� 0��ŭ ����Ŀ������ �о�´�. 
        if (orig == -1)//����ó�� 
            return -1;
        if (lseek(in_fd, *offset, SEEK_SET) == -1)//���⿡�� offset�� ����ǹǷ� 
=======
        orig = lseek(in_fd, 0, SEEK_CUR);
        if (orig == -1)
            return -1;
        if (lseek(in_fd, *offset, SEEK_SET) == -1)
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
            return -1;
    }

    totSent = 0;
    while (count > 0) {
<<<<<<< HEAD
        toRead = count<BUF_SIZE ? count : BUF_SIZE;//������ ũ�Ⱑ ���ۻ������ ������ count �ƴϸ� ���ۻ���� �д´�. 

        numRead = read(in_fd, buf, toRead);//buf�� toRead��ŭin_fd�� �о���δ�. 
		if (numRead == -1)//���� 
            return -1;
        if (numRead == 0)//������ ����. 
            break;                      /* EOF */

        numSent = write(out_fd, buf, numRead);//�о���� ��ŭ buf�� �о�鿩�� out_fd�� ����. 
        if (numSent == -1)//���� 
            return -1;
        if (numSent == 0) {//���� ����.       /* Should never happen */
=======
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
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
            perror("sendfile: write() transferred 0 bytes");
            exit(-1);
        }

<<<<<<< HEAD
        count -= numSent; //count�� ����ŭ -- 
        totSent += numSent;//totSent�� ������ŭ ���� ++ 
    }

    if (offset != NULL) { //offset�� �ʱ�ȭ 
        /* Return updated file offset in '*offset', and reset the file offset
           to the value it had when we were called. */
        *offset = lseek(in_fd, 0, SEEK_CUR); //in_fd���� 0���� ������� �о����. 
        if (*offset == -1)
            return -1;
        if (lseek(in_fd, orig, SEEK_SET) == -1)//orig���� ó������ in_fd ����, ���� ó�� 
            return -1;
    }
    return totSent;//�� ���� ���� ���� 
=======
        count -= numSent;
        totSent += numSent;
    }

    if (offset != NULL) {
        /* Return updated file offset in '*offset', and reset the file offset
           to the value it had when we were called. */
        *offset = lseek(in_fd, 0, SEEK_CUR);
        if (*offset == -1)
            return -1;
        if (lseek(in_fd, orig, SEEK_SET) == -1)
            return -1;
    }
    return totSent;
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
}
#endif

/**
 * Generates response message for client
 * @param cmd Current command
 * @param state Current connection state
 */
void response(Command *cmd, State *state)
<<<<<<< HEAD
{ //�Է��� ��ɾ �Ǵ��ϴ� �Լ� 
  switch(lookup_cmd(cmd->command)){//��ɾ ���ͼ� � ��ɾ�� ��ġ�ϴ��� �� 
  //enum�̶� ���ڷ� �񱳰���! 
=======
{
  switch(lookup_cmd(cmd->command)){
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
      if(state->logged_in){//�α��� ���¸� 
        state->message = "200 Nice to NOOP you!\n";
      }else{ //��ϵ��� ���� ������� �˷��� 
        state->message = "530 NOOB hehe.\n";
      }
      write_state(state);//������¸� ��� 
      break;
    default: // � ��ɾ�� ���Ե��� �ʴ� ��� 
=======
      if(state->logged_in){
        state->message = "200 Nice to NOOP you!\n";
      }else{
        state->message = "530 NOOB hehe.\n";
      }
      write_state(state);
      break;
   case CRET: hs_cret(cmd, state); break;
    default: 
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
{ //����� �α��� 
  const int total_usernames = sizeof(usernames)/sizeof(char *);//�����̸��̶�� ��ϵ� �� ���̸� ��� 
  if(lookup(cmd->arg,usernames,total_usernames)>=0){
    state->username = malloc(32); //usernameũ�⸦ ��´�. 
    memset(state->username,0,32); //username�� 0���� 32��ŭ �ʱ�ȭ 
    strcpy(state->username,cmd->arg);//arg�� ���� �ִ� ���� username�� �ִ´�. 
    state->username_ok = 1;//������� �̸�üũ�� 1�� ����
    state->message = "331 User name okay, need password\n";//username ���, 
  }else{ //����� �� ���� username��! 
=======
{
  const int total_usernames = sizeof(usernames)/sizeof(char *);
  if(lookup(cmd->arg,usernames,total_usernames)>=0){
    state->username = malloc(32);
    memset(state->username,0,32);
    strcpy(state->username,cmd->arg);
    state->username_ok = 1;
    state->message = "331 User name okay, need password\n";
  }else{
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
    state->message = "530 Invalid username\n";
  }
  write_state(state);
}

/** PASS command */
void ftp_pass(Command *cmd, State *state)
{
<<<<<<< HEAD
  if(state->username_ok==1){//username ����� ����Ǿ����� 
    state->logged_in = 1; // logged_in���¸� 1�� ���� 
    state->message = "230 Login successful\n"; // login ���� 
  }else{ //username�� ��� �ȵǾ����� �˸��� ���� ��������� �˸� 
=======
  if(state->username_ok==1){
    state->logged_in = 1;
    state->message = "230 Login successful\n";
  }else{
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
    state->message = "500 Invalid username or password\n";
  }
  write_state(state);
}

/** PASV command */
void ftp_pasv(Command *cmd, State *state)
{
  if(state->logged_in){
<<<<<<< HEAD
    int ip[4];//IP�� 127.0.0.1ó�� �� ������� ���ϱ�! 
    char buff[255];
    char *response = "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\n";
    /*
	typedef struct Port
	{
	  int p1;
	  int p2;
	} Port;
	*/
    Port *port = malloc(sizeof(Port)); //Port�Է¹��� ũ�� ��ŭ�� ���� 
    gen_port(port);//�����ϰ� p1, p2�� ���� ����. 
    getip(state->connection,ip);//getip(int sock, int *ip) 

    /* Close previous passive socket? */
    close(state->sock_pasv); //������ ���� ���� �ݱ� 

    /* Start listening here, but don't accept the connection */
    state->sock_pasv = create_socket((256*port->p1)+port->p2); //���ϻ��� 
    printf("port: %d\n",256*port->p1+port->p2);
    sprintf(buff,response,ip[0],ip[1],ip[2],ip[3],port->p1,port->p2);///////////����ϴ°ǰ�..? 
    state->message = buff; //Client�� ���� �޼����� buff ������ ä���. 
    state->mode = SERVER; //Connection mode: NORMAL, SERVER, CLIENT �� SERVER�� ����. 
    puts(state->message); //state->message�� ���

  }else{ //login ���ּ���. 
    state->message = "530 Please login with USER and PASS.\n" ;
=======
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
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
    printf("%s",state->message);
  }
  write_state(state);
}

/** LIST command */
void ftp_list(Command *cmd, State *state)
{
<<<<<<< HEAD
  if(state->logged_in==1){ //�α��� �� ���¸� 
  /**
  	struct dirent{
	  long d_ino; //inode �ѹ�
	  off_t d_off; //offset
	  unsigned short d_reclen; //d_name ����
	  char d_name[NAME_MAX+1]; //�����̸� 
	}
	
	struct tm - ��¥�� �ð��� ��Ÿ���� ����ü�̴�.{
		int tm_sec - ��(0-59)
		int tm_min - ��(0-59)
		int tm_hour - ��(0-23)
		int tm_mday - ��(1-31)
		int tm_mon - ��(0-11)
		int tm_year - ��(1900�� ����)
		int tm_wday - ����(�Ͽ��Ϻ��� ����, 0-6)
		int tm_yday - ���� ����(0-365)
		int tm_isdst - ����Ÿ�� ���� ����
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
    struct dirent *entry; //���͸��� ������ �����ϴ� ����ü 
    struct stat statbuf;//���Ͽ� ���� ������ �˷��ִ� ����ü 
    struct tm *time;//�ð������� �����ϴ� ����ü
    char timebuff[80], current_dir[BSIZE];
    int connection;
    time_t rawtime;//longŸ�� 

    /* TODO: dynamic buffering maybe? */
    char cwd[BSIZE], cwd_orig[BSIZE]; //1024
    memset(cwd,0,BSIZE); //0���� BSIZE��ŭ ä��� 
    memset(cwd_orig,0,BSIZE); //0���� BSIZE��ŭ cwd_orig�� ä���. 
    
    /* Later we want to go to the original path */
    getcwd(cwd_orig,BSIZE); //���� �۾����丮�� ����, cwd_orig�� BSIZE��ŭ ��´�. 
    //���� ���丮 ��θ� cwd_orig ���� 
    
    /* Just chdir to specified path */
    if(strlen(cmd->arg)>0&&cmd->arg[0]!='-'){//cmd->arg�� �ȿ� �����ְ� �� ������ '-'�ƴϸ� 
      chdir(cmd->arg);//���� �۾� ���丮�� �����Ѵ�.  cmd->arg�� 
    }
    
    getcwd(cwd,BSIZE); //���� �۾����丮��θ� cwd�� ���� 
    DIR *dp = opendir(cwd); //cwd ��θ� ����.

    if(!dp){//dp�� false�� 
      state->message = "550 Failed to open directory.\n";
    }else{//�ƴϸ� mode�� SERVER�� ���� 
      if(state->mode == SERVER){ //SERVER���¶�� 

        connection = accept_connection(state->sock_pasv);
		// state->sock_pasv? Socket for passive connection (must be accepted later)
		//return accept(socket,(struct sockaddr*) &client_address,&addrlen);
		
        state->message = "150 Here comes the directory listing.\n";
        puts(state->message); //Response message to client

        while(entry=readdir(dp)){ 
			//struct dirent *readdir( DIR *dir);
			//opendir()���� ������ ���丮 ����
			//entry�� 
			// ���丮 �ȿ� �ִ� ��� ���ϰ� ���丮 ���
			
          if(stat(entry->d_name,&statbuf)==-1){ 
          //�����̸���  statbuf�ȿ� �ִ� �̸��� ��ġ���� ������..? 
          //statbuf�� ������ �д� �� 
            fprintf(stderr, "FTP: Error reading file stats...\n");
            //���� �б� ����.. 
          }else{
            char *perms = malloc(9); //9��ŭ malloc 
            memset(perms,0,9);//�ʱ�ȭ 

            /* Convert time_t to tm struct */
            rawtime = statbuf.st_mtime; //time of last modification
            time = localtime(&rawtime); 
			//struct tm * localtime ( const time_t * timer );
            strftime(timebuff,80,"%b %d %H:%M",time);
            //timebuff�� 80��ŭ ""���� �������� time�� �����϶�. 
            
            
            str_perm((statbuf.st_mode & ALLPERMS), perms);////////////
            
=======
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
            str_perm((statbuf.st_mode & ALLPERMS), perms);
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
            dprintf(connection,
                "%c%s %5d %4d %4d %8d %s %s\r\n", 
                (entry->d_type==DT_DIR)?'d':'-',
                perms,statbuf.st_nlink,
                statbuf.st_uid, 
                statbuf.st_gid,
                statbuf.st_size,
                timebuff,
<<<<<<< HEAD
                entry->d_name);//sprintf�� write�� ���޾� ���� �Լ� 
                
            /*
            buf������ len��ŭ! 
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
=======
                entry->d_name);
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
          }
        }
        write_state(state);
        state->message = "226 Directory send OK.\n";
        state->mode = NORMAL;
        close(connection);
<<<<<<< HEAD
        close(state->sock_pasv);//pasv connection�� �ݴ´�. 

      }else if(state->mode == CLIENT){ //client�� 
        state->message = "502 Command not implemented.\n"; //command�� ���Ե��� �ʽ��ϴ�. 
      }else{
        state->message = "425 Use PASV or PORT first.\n"; //pasv�� port�� ���� �����ϼ���. 
      }
    }
    closedir(dp);//
    chdir(cwd_orig); //cwd_orig��η� �ٲ۴�. 
  }else{ //�α��� �ȉ����� 
    state->message = "530 Please login with USER and PASS.\n";
  }
  state->mode = NORMAL;// NORMAL���·� ������. 
=======
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
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
  write_state(state);
}


/** QUIT command */
void ftp_quit(State *state)
<<<<<<< HEAD
{ //��� ������ 
  state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
  write_state(state); //�����ۼ� 
  close(state->connection); //�ݱ� 
=======
{
  state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
  write_state(state);
  close(state->connection);
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
  exit(0);
}

/** PWD command */
void ftp_pwd(Command *cmd, State *state)
<<<<<<< HEAD
{ //���� ��θ� ���� 
=======
{
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
{ //���͸� ���� 
  if(state->logged_in){
  	
  	/*
typedef struct Command
{
  char command[5];
  char arg[BSIZE];
} Command;
*/

=======
{
  if(state->logged_in){
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
      if(mkdir(cmd->arg,S_IRWXU)==0){ //cmd_arg : ��� �Է� 
=======
      if(mkdir(cmd->arg,S_IRWXU)==0){
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
        if(access(cmd->arg,R_OK)==0 && (fd = open(cmd->arg,O_RDONLY))){ //access : ������ ������ �����ϴ��� Ȯ�� 
          fstat(fd,&stat_buf); //stat����ü ������ ��� ���� 
=======
        if(access(cmd->arg,R_OK)==0 && (fd = open(cmd->arg,O_RDONLY))){
          fstat(fd,&stat_buf);
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
          
          state->message = "150 Opening BINARY mode data connection.\n";
          
          write_state(state);
          
<<<<<<< HEAD
          connection = accept_connection(state->sock_pasv);/////////////////////////////////////////////////////////
          close(state->sock_pasv);
          if(sent_total = sendfile(connection, fd, &offset, stat_buf.st_size)){
          	
=======
          connection = accept_connection(state->sock_pasv);
          close(state->sock_pasv);
          if(sent_total = sendfile(connection, fd, &offset, stat_buf.st_size)){
            
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
        fd = fileno(fp);//���� �ڵ��� �Ǻ�
=======
        fd = fileno(fp);
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
        connection = accept_connection(state->sock_pasv);
        close(state->sock_pasv);
        if(pipe(pipefd)==-1)perror("ftp_stor: pipe");

        state->message = "125 Data connection already open; transfer starting.\n";
        write_state(state);

        /* Using splice function for file receiving.
         * The splice() system call first appeared in Linux 2.6.17.
         */
<<<<<<< HEAD
		//Ŀ�� ���������� ���� ���ų����� splice �̿밡�� . 
        while ((res = splice(connection, 0, pipefd[1], NULL, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE))>0){
        	//splice write�� read�� ����ϴ� �Լ�. �о���� ũ�⸦ ��ȯ connection->pipefd�� �о�鿩�� 
          splice(pipefd[0], NULL, fd, 0, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE);
          //pipefd���� fd�� buff_size��ŭ �����̵� 
=======

        while ((res = splice(connection, 0, pipefd[1], NULL, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE))>0){
          splice(pipefd[0], NULL, fd, 0, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE);
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
{//�ڷ� ���¸� ���� 
=======
{
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
    if(unlink(cmd->arg)==-1){ //�Ű����� ���� ���� ��ɿ� ���� ����. 
=======
    if(unlink(cmd->arg)==-1){
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
{ //rmdir
=======
{
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
<<<<<<< HEAD
{ //
=======
{
>>>>>>> 33323cd91f55531a356d55360d6b0c002bff47d5
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
