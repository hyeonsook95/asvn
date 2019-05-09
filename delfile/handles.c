//ftp_handles.c
#include "common.h"
#ifndef sendfile
#define BUF_SIZE 8192
<<<<<<< HEAD
ssize_t sendfile(int out_fd, int in_fd, off_t * offset, size_t count )
{//off_t longÇü Á¤¼ö
//ÆÄÀÏÀ» º¸³»´Â ÇÔ ¼ö
 
=======


//////////////////////////////////////////////////////////
//.asvnì„ í˜„ìž¬ ìž‘ì—… ë””ë ‰í† ë¦¬ì— ìƒì„±. ->trunkì¼ ë•Œ, í•¨ê»˜ ìƒì„±. trunk ìƒì„±ì„ ë¡œê·¸ì— ê¸°ë¡
void hs_cret(Command *cmd, State *state)
{
	char dirName[200];
	char *path
	int fp;

	//í˜„ìž¬ ìœ„ì¹˜ì— .asvn ìƒì„±
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
        //offset !=null , ÆÄÀÏÀÌ ÀÖÀ» ¶§ 
        orig = lseek(in_fd, 0, SEEK_CUR);//in_fd¸¦ 0¸¸Å­ ÇöÀçÄ¿¼­¿¡¼­ ÀÐ¾î¿Â´Ù. 
        if (orig == -1)//¿¡·¯Ã³¸® 
            return -1;
        if (lseek(in_fd, *offset, SEEK_SET) == -1)//¿©±â¿¡¼­ offsetÀÌ º¯°æµÇ¹Ç·Î 
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
        toRead = count<BUF_SIZE ? count : BUF_SIZE;//ÆÄÀÏÀÇ Å©±â°¡ ¹öÆÛ»çÀÌÁîº¸´Ù ÀÛÀ¸¸é count ¾Æ´Ï¸é ¹öÆÛ»çÀÌÁî¸¦ ÀÐ´Â´Ù. 

        numRead = read(in_fd, buf, toRead);//buf·Î toRead¸¸Å­in_fd¿¡ ÀÐ¾îµéÀÎ´Ù. 
		if (numRead == -1)//¿À·ù 
            return -1;
        if (numRead == 0)//ÀÐÀ»°Ô ¾ø´Ù. 
            break;                      /* EOF */

        numSent = write(out_fd, buf, numRead);//ÀÐ¾îµéÀÎ ¸¸Å­ buf·Î ÀÐ¾îµé¿©¼­ out_fd·Î ¾´´Ù. 
        if (numSent == -1)//¿¡·¯ 
            return -1;
        if (numSent == 0) {//¾µ°Ô ¾ø´Ù.       /* Should never happen */
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
        count -= numSent; //count´Â ¾´¸¸Å­ -- 
        totSent += numSent;//totSent´Â º¸³½¸¸Å­ °ªÀ» ++ 
    }

    if (offset != NULL) { //offsetÀ» ÃÊ±âÈ­ 
        /* Return updated file offset in '*offset', and reset the file offset
           to the value it had when we were called. */
        *offset = lseek(in_fd, 0, SEEK_CUR); //in_fd¿¡¼­ 0ºÎÅÍ ÇöÀç±îÁö ÀÐ¾îµéÀÓ. 
        if (*offset == -1)
            return -1;
        if (lseek(in_fd, orig, SEEK_SET) == -1)//origºÎÅÍ Ã³À½±îÁö in_fd ÀúÀå, ¿À·ù Ã³¸® 
            return -1;
    }
    return totSent;//ÃÑ º¸³½ °ªÀ» ÀúÀå 
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
{ //ÀÔ·ÂÇÑ ¸í·É¾î¸¦ ÆÇ´ÜÇÏ´Â ÇÔ¼ö 
  switch(lookup_cmd(cmd->command)){//¸í·É¾î¸¦ µé°í¿Í¼­ ¾î¶² ¸í·É¾î¿Í ÀÏÄ¡ÇÏ´ÂÁö ºñ±³ 
  //enumÀÌ¶ó¼­ ¼ýÀÚ·Î ºñ±³°¡´É! 
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
      if(state->logged_in){//·Î±×ÀÎ »óÅÂ¸é 
        state->message = "200 Nice to NOOP you!\n";
      }else{ //µî·ÏµÇÁö ¾ÊÀº »ç¶÷ÀÓÀ» ¾Ë·ÁÁÜ 
        state->message = "530 NOOB hehe.\n";
      }
      write_state(state);//ÇöÀç»óÅÂ¸¦ ±â·Ï 
      break;
    default: // ¾î¶² ¸í·É¾î¿¡µµ Æ÷ÇÔµÇÁö ¾Ê´Â °æ¿ì 
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
{ //»ç¿ëÀÚ ·Î±×ÀÎ 
  const int total_usernames = sizeof(usernames)/sizeof(char *);//À¯ÀúÀÌ¸§ÀÌ¶ó°í µî·ÏµÈ ÃÑ ±æÀÌ¸¦ °è»ê 
  if(lookup(cmd->arg,usernames,total_usernames)>=0){
    state->username = malloc(32); //usernameÅ©±â¸¦ Àâ´Â´Ù. 
    memset(state->username,0,32); //usernameÀ» 0À¸·Î 32¸¸Å­ ÃÊ±âÈ­ 
    strcpy(state->username,cmd->arg);//arg·Î µé¾î¿Í ÀÖ´Â °ªÀ» username¿¡ ³Ö´Â´Ù. 
    state->username_ok = 1;//»ç¿ëÀÚÀÇ ÀÌ¸§Ã¼Å©¸¦ 1·Î ¼³Á¤
    state->message = "331 User name okay, need password\n";//username Åë°ú, 
  }else{ //µî·ÏÇÒ ¼ö ¾ø´Â usernameÀÓ! 
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
  if(state->username_ok==1){//username µî·ÏÀÌ Åë°úµÇ¾úÀ¸¸é 
    state->logged_in = 1; // logged_in»óÅÂ¸¦ 1·Î º¯°æ 
    state->message = "230 Login successful\n"; // login ¼º°ø 
  }else{ //usernameÀÌ Åë°ú ¾ÈµÇ¾úÀ¸¸é ¾Ë¸ÂÁö ¾ÊÀº »ç¿ëÀÚÀÓÀ» ¾Ë¸² 
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
    int ip[4];//IP°¡ 127.0.0.1Ã³·³ ³× µîºÐÀ¸·Î µé¾î°¡´Ï±î! 
    char buff[255];
    char *response = "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\n";
    /*
	typedef struct Port
	{
	  int p1;
	  int p2;
	} Port;
	*/
    Port *port = malloc(sizeof(Port)); //PortÀÔ·Â¹ÞÀº Å©±â ¸¸Å­À» ÀúÀå 
    gen_port(port);//·£´ýÇÏ°Ô p1, p2ÀÇ °ªÀ» ±¸ÇÔ. 
    getip(state->connection,ip);//getip(int sock, int *ip) 

    /* Close previous passive socket? */
    close(state->sock_pasv); //ÀÌÀü¿¡ ¿­¸° ¼ÒÄÏ ´Ý±â 

    /* Start listening here, but don't accept the connection */
    state->sock_pasv = create_socket((256*port->p1)+port->p2); //¼ÒÄÏ»ý¼º 
    printf("port: %d\n",256*port->p1+port->p2);
    sprintf(buff,response,ip[0],ip[1],ip[2],ip[3],port->p1,port->p2);///////////±â·ÏÇÏ´Â°Ç°¡..? 
    state->message = buff; //Client¿¡ º¸³¾ ¸Þ¼¼Áö¿¡ buff ³»¿ëÀ» Ã¤¿î´Ù. 
    state->mode = SERVER; //Connection mode: NORMAL, SERVER, CLIENT ¸¦ SERVER·Î ¼³Á¤. 
    puts(state->message); //state->message¸¦ Ãâ·Â

  }else{ //login ÇØÁÖ¼¼¿ä. 
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
  if(state->logged_in==1){ //·Î±×ÀÎ µÈ »óÅÂ¸é 
  /**
  	struct dirent{
	  long d_ino; //inode ³Ñ¹ö
	  off_t d_off; //offset
	  unsigned short d_reclen; //d_name ±æÀÌ
	  char d_name[NAME_MAX+1]; //ÆÄÀÏÀÌ¸§ 
	}
	
	struct tm - ³¯Â¥¿Í ½Ã°£À» ³ªÅ¸³»´Â ±¸Á¶Ã¼ÀÌ´Ù.{
		int tm_sec - ÃÊ(0-59)
		int tm_min - ºÐ(0-59)
		int tm_hour - ½Ã(0-23)
		int tm_mday - ÀÏ(1-31)
		int tm_mon - ¿ù(0-11)
		int tm_year - ³â(1900³â ±âÁØ)
		int tm_wday - ¿äÀÏ(ÀÏ¿äÀÏºÎÅÍ ½ÃÀÛ, 0-6)
		int tm_yday - ¿¬Áß ÀÏÀÚ(0-365)
		int tm_isdst - ¼­¸ÓÅ¸ÀÓ ¼³Á¤ ¿©ºÎ
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
    struct dirent *entry; //µð·ºÅÍ¸®ÀÇ Á¤º¸¸¦ ÀúÀåÇÏ´Â ±¸Á¶Ã¼ 
    struct stat statbuf;//ÆÄÀÏ¿¡ ´ëÇÑ Á¤º¸¸¦ ¾Ë·ÁÁÖ´Â ±¸Á¶Ã¼ 
    struct tm *time;//½Ã°£Á¤º¸¸¦ ÀúÀåÇÏ´Â ±¸Á¶Ã¼
    char timebuff[80], current_dir[BSIZE];
    int connection;
    time_t rawtime;//longÅ¸ÀÔ 

    /* TODO: dynamic buffering maybe? */
    char cwd[BSIZE], cwd_orig[BSIZE]; //1024
    memset(cwd,0,BSIZE); //0À¸·Î BSIZE¸¸Å­ Ã¤¿ì±â 
    memset(cwd_orig,0,BSIZE); //0À¸·Î BSIZE¸¸Å­ cwd_orig¸¦ Ã¤¿î´Ù. 
    
    /* Later we want to go to the original path */
    getcwd(cwd_orig,BSIZE); //ÇöÀç ÀÛ¾÷µð·ºÅä¸®¸¦ ÀúÀå, cwd_orig¿¡ BSIZE¸¸Å­ ´ã´Â´Ù. 
    //ÇöÀç µð·ºÅä¸® °æ·Î¸¦ cwd_orig ÀúÀå 
    
    /* Just chdir to specified path */
    if(strlen(cmd->arg)>0&&cmd->arg[0]!='-'){//cmd->arg°¡ ¾È¿¡ ³»¿ëÀÖ°í µé¾î°£ ³»¿ëÀÌ '-'¾Æ´Ï¸é 
      chdir(cmd->arg);//ÇöÀç ÀÛ¾÷ µð·ºÅä¸®¸¦ º¯°æÇÑ´Ù.  cmd->arg·Î 
    }
    
    getcwd(cwd,BSIZE); //ÇöÀç ÀÛ¾÷µð·ºÅä¸®°æ·Î¸¦ cwd¿¡ ÀúÀå 
    DIR *dp = opendir(cwd); //cwd °æ·Î¸¦ ¿¬´Ù.

    if(!dp){//dp°¡ false¸é 
      state->message = "550 Failed to open directory.\n";
    }else{//¾Æ´Ï¸é mode¸¦ SERVER·Î Á¢¼Ó 
      if(state->mode == SERVER){ //SERVER»óÅÂ¶ó¸é 

        connection = accept_connection(state->sock_pasv);
		// state->sock_pasv? Socket for passive connection (must be accepted later)
		//return accept(socket,(struct sockaddr*) &client_address,&addrlen);
		
        state->message = "150 Here comes the directory listing.\n";
        puts(state->message); //Response message to client

        while(entry=readdir(dp)){ 
			//struct dirent *readdir( DIR *dir);
			//opendir()¿¡¼­ ¿­±âÇÑ µð·ºÅä¸® Á¤º¸
			//entry´Â 
			// µð·ºÅä¸® ¾È¿¡ ÀÖ´Â ¸ðµç ÆÄÀÏ°ú µð·ºÅä¸® Ãâ·Â
			
          if(stat(entry->d_name,&statbuf)==-1){ 
          //ÆÄÀÏÀÌ¸§ÀÌ  statbuf¾È¿¡ ÀÖ´Â ÀÌ¸§°ú ÀÏÄ¡ÇÏÁö ¾ÊÀ¸¸é..? 
          //statbuf¸¦ °¡Áö°í ÀÐ´Â µí 
            fprintf(stderr, "FTP: Error reading file stats...\n");
            //ÆÄÀÏ ÀÐ±â ½ÇÆÐ.. 
          }else{
            char *perms = malloc(9); //9¸¸Å­ malloc 
            memset(perms,0,9);//ÃÊ±âÈ­ 

            /* Convert time_t to tm struct */
            rawtime = statbuf.st_mtime; //time of last modification
            time = localtime(&rawtime); 
			//struct tm * localtime ( const time_t * timer );
            strftime(timebuff,80,"%b %d %H:%M",time);
            //timebuff¿¡ 80¸¸Å­ ""°°Àº Çü½ÄÀ¸·Î timeÀ» ÀúÀåÇÏ¶ó. 
            
            
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
                entry->d_name);//sprintf¿Í write¸¦ ¿¬´Þ¾Æ ¾²´Â ÇÔ¼ö 
                
            /*
            buf³»¿ëÀ» len¸¸Å­! 
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
        close(state->sock_pasv);//pasv connectionÀ» ´Ý´Â´Ù. 

      }else if(state->mode == CLIENT){ //client¸é 
        state->message = "502 Command not implemented.\n"; //command°¡ Æ÷ÇÔµÇÁö ¾Ê½À´Ï´Ù. 
      }else{
        state->message = "425 Use PASV or PORT first.\n"; //pasv³ª port¸¦ ¸ÕÀú ¼³Á¤ÇÏ¼¼¿ä. 
      }
    }
    closedir(dp);//
    chdir(cwd_orig); //cwd_orig°æ·Î·Î ¹Ù²Û´Ù. 
  }else{ //·Î±×ÀÎ ¾È‰çÀ¸¸é 
    state->message = "530 Please login with USER and PASS.\n";
  }
  state->mode = NORMAL;// NORMAL»óÅÂ·Î µ¹¸°´Ù. 
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
{ //Åë½Å ³¡³»±â 
  state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
  write_state(state); //»óÅÂÀÛ¼º 
  close(state->connection); //´Ý±â 
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
{ //ÇöÀç °æ·Î¸¦ ÀúÀå 
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
{ //µð·ºÅÍ¸® º¯°æ 
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
      if(mkdir(cmd->arg,S_IRWXU)==0){ //cmd_arg : °æ·Î ÀÔ·Â 
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
        if(access(cmd->arg,R_OK)==0 && (fd = open(cmd->arg,O_RDONLY))){ //access : ÁöÁ¤µÈ ÆÄÀÏÀÌ Á¸ÀçÇÏ´ÂÁö È®ÀÎ 
          fstat(fd,&stat_buf); //stat±¸Á¶Ã¼ Á¤º¸¸¦ ¸ðµÎ ÀúÀå 
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
        fd = fileno(fp);//ÆÄÀÏ ÇÚµéÀ» ÆÇº°
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
		//Ä¿³Î °ø°£³»¿¡¼­ ÆÄÀÏ ¼ö½Å³»¿¡¼­ splice ÀÌ¿ë°¡´É . 
        while ((res = splice(connection, 0, pipefd[1], NULL, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE))>0){
        	//splice write¿Í read¸¦ ´ë½ÅÇÏ´Â ÇÔ¼ö. ÀÐ¾îµéÀÎ Å©±â¸¦ ¹ÝÈ¯ connection->pipefd¿¡ ÀÐ¾îµé¿©¿È 
          splice(pipefd[0], NULL, fd, 0, buff_size, SPLICE_F_MORE | SPLICE_F_MOVE);
          //pipefd¿¡¼­ fd·Î buff_size¸¸Å­ ³»¿ëÀÌµ¿ 
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
{//ÀÚ·á »óÅÂ¸¦ ¼³Á¤ 
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
    if(unlink(cmd->arg)==-1){ //¸Å°³º¯¼ö ¹ÞÀº °ÍÀÇ ¸í·É¿¡ ´ëÇØ »èÁ¦. 
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
