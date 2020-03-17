#include "common.h"

/**
 * 명령어를 처리하기 위한 함수들
 * parse_command, response
 **/

/* 사용자에게 입력받은 값을 잘라서, 명령어와 인자로 나누어 변수에 저장하는 함수 */
void parse_command(int argc, char **args, Command *cmd)
{

	int n = argc - 2;
	int i = 0;
	int sockid;

	switch (lookup_cmd(cmd->command))
	{
	/* 명령어만 입력받았을 때 */
	case LOG:
	case CREAT:
	case COMMIT:
	case PWD:
	case QUIT:
		break;
	case CHECKOUT:
		printf("socket staring\n");
		sockid = sock();
		//서버로 부터 파일을 받아올 때
		docftp(args[2], sockid, strlen(args[2]));
		break;
	case UPDATE:
		printf("socket staring\n");
		sockid = sock();
		//클라이언트로부터 파일을 보낼 때
		doftp(sockid);
		break;
	/* 명령어와 경로를 입력받았을 때 */
	case MKDIR:
	case DELETE:
	case ADD:
		strcpy(cmd->arg, args[2]);
		n--;
		break;
	/* 명령어, 이전 경로, 새로운 경로를 입력받았을 때 */
	case MV:
	case DIFF:
		break;
	case MERGE:
		strcpy(cmd->arg, args[2]);
		n -= 2;
		strcat(cmd->arg, " ");
		strcat(cmd->arg, args[3]);
		break;
	default:
		printf("500 Unknown command\n");
		break;
	}

	memset(cmd->cmt, '\0', sizeof(cmd->cmt));

	/* 사용자로부터 받은 명령어에 입력된 comment를 변수 cmt에 저장 */
	for (i = n; i < argc - 3; i++)
	{
		if (argc != i - 1)
		{
			strcat(cmd->cmt, args[i]);
			strcat(cmd->cmt, " ");
		}
		else
			strcat(cmd->cmt, "\n");
	}

	return;
}

/** 변수에 입력된 명령어에 따라 알맞은 함수로 handle을 넘겨주는 함수 **/
void response(Command *cmd, State *state)
{
	switch (lookup_cmd(cmd->command))
	{ //user function -> command's enum number
	case CREAT:
		asvn_creat(cmd, state);
		break;
	case UP:
		asvn_update();
		break;
	case LOG:
		asvn_readlog(cmd, state);
		break;
	case MKDIR:
		asvn_mkdir(cmd, state);
		break;
	case DELETE:
		asvn_remove(cmd, state);
		break;
	case PWD:
		asvn_pwd(cmd, state);
		break;
	case QUIT:
		ftp_quit(state);
		break;
	default:
		state->message = "500 Unknown command\n";
		break;
	}
}

/**
 * 네트워크와 관련없는 시스템 함수들 
 * creat_log, write_log, logfile, lookup_cmd, lookup  
**/

/* 형식에 맞게 로그를 생헝하는 함수 */
void creat_log(Command *cmd, State *state, char temp[500])
{
	time_t timer = time(NULL);
	struct tm *t;

	char current_time[20];

	memset(temp, '\0', BSIZE);

	/*set current_time*/
	t = localtime(&timer);
	sprintf(current_time, "%d-%d-%d-%d:%d:%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

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

/* 생성한 함수를 지정된 log 파일에 기록하는 함수 */
void write_log(Command *cmd, State *state)
{
	char path[200];
	char logpath[BSIZE];
	char temp[500];
	FILE *fp_log;

	memset(logpath, '\0', BSIZE);
	memset(temp, '\0', 500);

	/*set .asvn path*/
	logfile(path);

	if ((fp_log = fopen(path, "a")) == NULL)
	{
		perror("fopen");
		exit(1);
	}

	creat_log(cmd, state, temp);

	/*write log in .asvn*/
	if (fwrite(temp, 1, strlen(temp), fp_log) == -1)
	{
		perror("fwrite");
		exit(1);
	}

	if (fclose(fp_log) == -1)
	{
		perror("fclose");
		exit(1);
	}
}

/* 현재 저장소의 log 파일이 어디있는지 알려주는 함수 */
void logfile(char path[100])
{
	char *asvnpath = NULL;
	char logpath[BSIZE];
	char temp[BSIZE];
	FILE *fp_log;

	memset(temp, '\0', BSIZE);

	asvnpath = getenv("HOME");
	strcat(asvnpath, "/asvn/repos");
	getcwd(logpath, 1024);

	if (!(fp_log = fopen(asvnpath, "r")))
	{
		perror("repos fpoen");
		exit(1);
	}
	while (fgets(temp, 1024, fp_log))
	{
		if (strstr(logpath, temp))
		{
			printf("temp: %s\n", temp);
			strcpy(logpath, temp);
		}
	}

	strcat(logpath, "/.asvn/log");

	strcpy(path, logpath);
}

/** 입력받은 명령어를 처리하기 쉽도록 정수형으로 변환하는 함수 **/
int lookup_cmd(char *cmd)
{
	const int cmdlist_count = sizeof(cmdlist_str) / sizeof(char *);
	return lookup(cmd, cmdlist_str, cmdlist_count);
}

int lookup(char *needle, const char **haystack, int count)
{
	int i;
	for (i = 0; i < count; i++)
	{ //(enum cmdlist & char* cmflist_str)'s count
		if (strcmp(needle, haystack[i]) == 0)
			return i; //needel == haystack[i] -> return i
	}
	return -1;
}

/**
 * 네트워크 시스템 함수들 
 * sock, docftp, doingstr, doingfile, doftp
**/

int sock()
{
	int sockid;
	struct sockaddr_in my_addr, server_addr;

	printf("client: creating socket\n");
	if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("client: socket error : %d\n", errno);
		exit(0);
	}

	printf("client: binding my local socket\n");
	bzero((char *)&my_addr, sizeof(my_addr));

	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	my_addr.sin_port = htons(CLIENT_PORT_ID);
	if (bind(sockid, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0)
	{
		printf("client: bind  error :%d\n", errno);
		exit(0);
	}
	printf("client: starting connect\n");
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST_ADDR);
	server_addr.sin_port = htons(SERVER_PORT_ID);
	if (connect(sockid, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("client: connect  error :%d\n", errno);
		exit(0);
	}
	return sockid;
}

void docftp(char *filename, int socketDisc, int len)
{ //doftp를 실행한다.
	int i, fsize, recv_fsize, fd, msg_ok, size;
	int fail = FILENAMEOK;
	int fail1, req, c, ack, file_info;
	int no_read, num_blks, num_blks1, num_last_blk, num_last_blk1, tmp;
	char fname[MAXLINE + 1];
	char out_buf[MAXSIZE];
	FILE *fp;
	char text[MAXLINE];
	char name[MAXLINE];
	int sendnum = 0;

	no_read = 0;
	num_blks = 0;
	num_last_blk = 0;

	memset(fname, '\0', MAXLINE);
	strncpy(fname, filename, len);

	printf("filename : %s\n", fname);

	sprintf(dataBuf, "%d", len);
	printf("sizeof : %d strlen : %d\n", sizeof(dataBuf), strlen(dataBuf));

	//write
	sock_len = write(socketDisc, dataBuf, strlen(dataBuf));
	if (sock_len < 0)
	{
		printf("write  error :%d\n", errno);
		exit(0);
	}
	printf("dataBuf : %s\n", dataBuf);
	//read
	sock_len = read(socketDisc, dataBuf, sizeof(buf));
	if (sock_len < 0)
	{
		printf("recv Fail!");
	}
	sprintf(buf, "%d", FILENAMEOK);
	printf("data Buf : %s buf : %s \n", dataBuf, buf);
	if ((strcmp(dataBuf, buf)) == 0)
	{
		printf("send file name : %s!!\n", fname);
		//write name
		sock_len = write(socketDisc, fname, len);
		if (sock_len < 0)
		{
			printf("write file name send fail %d\n", errno);
		}
		//read file Send Ok or No
		sock_len = read(socketDisc, dataBuf, sizeof(dataBuf));
		if (sock_len < 0)
		{
			printf("file send info fail %d\n", errno);
		}
	}
	//file info
	file_info = atoi(dataBuf);
	if (file_info == STARTTRANSFER)
	{
		sprintf(dataBuf, "start");
		printf("send command start?? : %s\n", dataBuf);
		//write recv data from server start!!!!
		sock_len = write(socketDisc, dataBuf, sizeof(dataBuf));
	}
	else
	{
		printf("file send info fail !!");
		exit(0);
	}

	/* IF SERVER CANT OPEN FILE THEN INFORM CLIENT OF THIS AND TERMINATE */
	if ((fp = fopen(fname, "w")) == NULL)
	{ /*cant open file*/
		perror("fopen error: \n");
	}
	//read 1. read file all size
	sock_len = read(socketDisc, dataBuf, sizeof(dataBuf));
	fsize = atoi(dataBuf);
	//printf("atoi : %d",atoi(dataBuf));
	if (fsize != 0)
	{
		//printf("here come ");
		//fsize = atoi(dataBuf);//num_blks , num_blks1,num_last_blk
		//write ok
		sock_len = write(socketDisc, "size_ok", sizeof("size_ok"));
		//read 2. num_blks_ok
		sock_len = read(socketDisc, dataBuf, sizeof(dataBuf));
		if (sock_len < 0)
		{
			printf("num_blcks_no recv!! error\n");
			exit(0);
		}
		if (atoi(dataBuf) != 0)
		{
			num_blks = atoi(dataBuf);
			//write ok
			sock_len = write(socketDisc, "num_blks_ok", sizeof("num_blks_ok"));
			//read 3. num_last_blk
			sock_len = read(socketDisc, dataBuf, sizeof(dataBuf));
			if (sock_len < 0)
			{
				printf("num_last_blk no recv!! error\n");
				exit(0);
			}
			if (strcmp(dataBuf, "none") == 0)
			{
				num_last_blk = 0;
				//write
				sock_len = write(socketDisc, "num_last_ok", sizeof("num_last_ok"));
			}
			else if (atoi(dataBuf) != 0)
			{
				num_last_blk = atoi(dataBuf);
				//write
				sock_len = write(socketDisc, "num_last_ok", sizeof("num_last_ok"));
				printf("send ready \nsize : %d\nnum_blks : %d\nnum_last_blk %d\n", fsize, num_blks, num_last_blk);
			}
		}
	}
	for (i = 0; i < num_blks; i++)
	{
		//read file data

		sock_len = read(socketDisc, dataBuf, sizeof(dataBuf));
		printf("read\n\n\n%s\n", dataBuf);
		if (sock_len < 0)
		{
			printf("error\n");
			exit(0);
		}
		size = fwrite(dataBuf, sizeof(char), MAXSIZE, fp);
		if (size < 0)
		{
			printf("error\n");
			exit(0);
		}
		recv_fsize = MAXSIZE * (i + 1);
		printf("recv data : %d/%d\n", recv_fsize, fsize);
	}
	if (num_last_blk != 0)
	{
		//read last data
		sock_len = read(socketDisc, dataBuf, num_last_blk);
		//dataBuf[num_last_blk]='\0';
		recv_fsize += num_last_blk;
		size = fwrite(dataBuf, sizeof(char), num_last_blk, fp);
		//printf("recv data : %d/%d\n",recv_fsize,fsize);
		if (fsize == recv_fsize)
		{
			printf("All file recv\n");
		}
		//write file recv ok
	}
	fclose(fp);
	sock_len = write(socketDisc, "file_recv", sizeof("file_recv"));
	close(socketDisc);
}

void doingstr(int sockid, char com[])
{ ////num&str
	int len, len2, len3;
	char fname[MAXLINE];
	char name[MAXLINE];
	char *filename = "sun.jpeg";
}

void doingfile(int sockid, char name[])
{ ///file
	int req;
	int fail;
	int no_read, tmp, len;
	char fname[MAXLINE];
	fail = FILENAMEOK;

	len = strlen(name); //len, 들어온 이름값의 길이확인
	printf("filelen : %d\n", len);

	strncpy(fname, name, len); //초기화한 fname에 name붙이기

	docftp(name, sockid, len);

	printf("server: FILE TRANSFER COMPLETE on socket %d\n", sockid);
	close(sockid);
}

void doftp(int sockid)
{ //doftp를 실행한다.
	int newsockid, i, getfile, ack, msg, msg_2, c;
	int no_writen, start_xfer, fsize = 0, recv_fsize, num_blks, num_last_blk;
	struct sockaddr_in my_addr, server_addr;
	FILE *fp;
	int string_len, size;
	char t_buf[512];
	char in_buf[MAXSIZE];
	char fname[MAXLINE];
	int file_info;
	no_writen = 0;
	num_blks = 0;
	num_last_blk = 0;
	int req = 0;

	memset(fname, '\0', MAXLINE);
	req = rnum(sockid, req); //번호읽기
	printf("get num : %d\n", req);
	//read file name
	sock_len = read(sockid, fname, 512);

	if (sock_len < 0)
	{
		printf("client: filename read error :%d\n", errno);
		printf("now im in read error\n");
		file_info = BADFILENAME; //fail to 200 is read error
	}
	printf("fname : %s\n\n\n", fname);

	if (sock_len >= 0)
	{
		if ((fp = fopen(fname, "rb")) == NULL)
		{
			printf(" client: local open file error \n");
			exit(0);
		}
		//file send ready!!! 500 is read error
		file_info = STARTTRANSFER;
		printf("server read file start !!\n");
	}

	sprintf(dataBuf, "%d", file_info);
	//write file send OK!!
	sock_len = write(sockid, dataBuf, sizeof(dataBuf));
	if (sock_len < 0)
	{
		printf("write file send ok error\n");
		exit(0);
	}
	//read file trans info
	sock_len = read(sockid, dataBuf, sizeof(dataBuf));
	if (strcmp(dataBuf, "start") == 0)
	{
		printf("start file send!!\n");
		//write file size
	}
	while ((c = fgetc(fp)) != EOF)
	{
		fsize++;
	}

	fp = fopen(fname, "rb");
	printf("fffsize : %d\n\n\n\n", fsize);
	num_blks = fsize / MAXSIZE;

	num_last_blk = fsize % MAXSIZE;

	//write 1. send fileSize
	sprintf(dataBuf, "%d", fsize);
	sock_len = write(sockid, dataBuf, sizeof(dataBuf));
	printf("size send OK %s\n", dataBuf);
	//read 1. read fileSize Ok
	sock_len = read(sockid, dataBuf, sizeof(dataBuf));

	if ((strcmp(dataBuf, "size_ok")) == 0)
	{
		//write num_blks
		sprintf(dataBuf, "%d", num_blks);
		sock_len = write(sockid, dataBuf, sizeof(dataBuf));
		printf("num_blks send OK temp_buf%s\n", dataBuf);
		//read 2. read num_blks Ok
		sock_len = read(sockid, dataBuf, sizeof(dataBuf));
		if ((strcmp(dataBuf, "num_blks_ok")) == 0)
		{
			//write num_last_blk
			sprintf(dataBuf, "%d", num_last_blk);
			sock_len = write(sockid, dataBuf, sizeof(dataBuf));
			printf("num_last_blk send OK %s\n", dataBuf);
			//read 3. read num_last_blk Ok
			sock_len = read(sockid, dataBuf, sizeof(dataBuf));
			if ((strcmp(dataBuf, "num_last_ok")) == 0)
			{
				printf("All recv Ok!!\n");

				for (i = 0; i < num_blks; i++)
				{
					size = fread(temp_buf, sizeof(char), 512, fp);
					if (size < 0)
					{
						printf("error\n");
						exit(0);
					}
					else
					{
						//write data
						recv_fsize = MAXSIZE * (i + 1);
						sock_len = write(sockid, temp_buf, sizeof(temp_buf));
					}
				} //for

				if (num_last_blk != 0)
				{
					size = fread(temp_buf, sizeof(char), num_last_blk, fp);
					if (size < 0)
					{
						printf("error\n");
						exit(0);
					}

					recv_fsize += num_last_blk;
					printf("send last : %s\n\n\n\n\n\n", temp_buf);
					//write last data
					sock_len = write(sockid, temp_buf, num_last_blk);
					if (sock_len < 0)
					{
						printf("error\n");
						exit(0);
					} // if(sock_len<0)
					printf("send data : %d/%d\n", recv_fsize, fsize);
					//read recv info
					sock_len = read(sockid, dataBuf, sizeof(dataBuf));
					if (sock_len < 0)
					{
						printf("error\n");
						exit(0);
					} // if(sock_len<0)
					if ((strcmp(dataBuf, "file_recv")) == 0)
					{
						printf("file send complate!!\n");
						fclose(fp);
						close(sockid);
						exit(0);
					} //if((strcmp(dataBuf,"file_recv"))==0){
				}	 //if(num_last_blk!=0){
			}		  // if((strcmp(dataBuf,"num_last_ok"))==0){
		}			  // if((strcmp(dataBuf,"size_ok"))==0){
	}
}
