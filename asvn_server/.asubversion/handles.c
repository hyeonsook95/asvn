#include "common.h"

/**
 * 네트워크와 관련없는 시스템 함수들 
 * creat_log, write_log, logfile
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

/**
 * 네트워크 시스템 함수들 
 * sock, docftp, doingstr, doingfile, doftp
**/

void docftp(char *filename, int len, int socketDisc)
{ //doftp를 실행한다.

    int i, fsize, recv_fsize, fd, msg_ok, size;
    int fail = FILENAMEOK;
    int fail1, req, c, ack, file_info;
    int no_read, num_blks, num_blks1, num_last_blk, num_last_blk1, tmp;
    char fname[MAXLINE];
    char out_buf[MAXSIZE];
    FILE *fp;
    char text[MAXLINE];
    char name[MAXLINE];
    int sendnum = 0;

    printf("filename : %s\n", filename);
    printf("len : %d\n", len);

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
    printf("%d\n\n\n\n", sock_len);
    if (sock_len < 0)
    {
        perror("sock_len error");
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
    if (strcmp(dataBuf, buf) == 0)
    {
        printf("send file name : %s!!\n", fname);
        //write name
        sock_len = write(socketDisc, fname, sizeof(fname));
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

    if (fsize != 0)
    {
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
        recv_fsize += num_last_blk;
        size = fwrite(dataBuf, sizeof(char), num_last_blk, fp);

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

/*create server socket*/
int create_socket(int port)
{
    int sock;
    int reuse = 1;
    /* Server addess */
    struct sockaddr_in server_address = (struct sockaddr_in){
        AF_INET,
        htons(port),
        (struct in_addr){INADDR_ANY}};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Cannot open socket");
        exit(EXIT_FAILURE);
    }
    /* Address can be reused instantly after program exits */
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);
    /* Bind socket to server address */
    if (bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        fprintf(stderr, "Cannot bind socket to address");
        exit(EXIT_FAILURE);
    }
    listen(sock, 5);
    return sock;
}

void dofile(int newsd, char *filename) //file
{
    int fail, req;
    char fname[MAXLINE];
    char name[MAXLINE];
    char buf[MAXSIZE];
    int len;
    int msg_2;
    int start_xfer;
    char *path = getcwd(buf, MAXSIZE);
    char *filepwd = getcwd(buf, MAXSIZE);

    strcat(filepwd, "/");
    strcat(filepwd, filename);
    if (0 == access(filepwd, R_OK))
    {
        printf("파일이 존재합니다.\n");
        printf("path : %s\n", filepwd);
        fail = FILENAMEOK; //400
        printf("file exit? : %d", fail);
        if (chdir(path) == 0)
        {
            printf("정상적으로 경로를 이동하였습니다.\n");
            asvn_retr(newsd); //real work
        }
        else
        {
            printf("정상적으로 경로를 이동하지 못하였습니다.\n");
            exit(0);
        }
    }
    else
    {
        printf("파일이 존재하지 않습니다.\n");
        fail = BADFILENAME; //200
    }

    printf("file exit? : %d", fail);
}

void dostr(int newsd) //str
{
    char fname[MAXLINE]; //filename
    char name[MAXLINE];  //return
    char name2[MAXLINE]; //return
    char com[MAXLINE];   //command
    int len1, len2, len3;

    len1 = rnum(newsd, len1); //번호읽기
    printf("len1 : %d\n", len1);
    memset(com, '\0', MAXLINE);

    asvn_retr(newsd);
    printf("파일전송완료\n");
}

int rnum(int sock, int no)
{ //소켓넘버,받으려는 수
    no = 0;
    //read
    sock_len = read(sock, dataBuf, 512);
    if (sock_len < 0)
    {
        printf("read error : %d\n", errno);
        exit(0);
    }
    sprintf(dataBuf, "%d", FILENAMEOK);
    //write
    sock_len = write(sock, dataBuf, strlen(dataBuf));
    if (sock_len < 0)
    {
        printf("file Ok Recomand Fail %d\n", errno);
    }
    printf("data : %s\n", dataBuf);
    //char to int
    no = atoi(dataBuf);
    return no;
}

char *rstr(int sock, char fname[], int len)
{ //socketNumber, str, strlen
    char name[len];
    memset(name, '\0', len);
    if ((read(sock, fname, len)) < 0)
    {
        printf("server: filename read error :%d\n", errno);
        printf("now im in read error\n");
    }

    return fname;
}

int wnum(int sock, int setnum)
{ //socketNUM, WRITING NUM
    int sendnum;
    sendnum = setnum;
    sendnum = htons(sendnum);
    if ((writen(sock, (char *)&sendnum, sizeof(sendnum))) < 0)
    {
        printf("write  error :%d\n", errno);
        exit(0);
    }

    return sendnum;
}

char *wstr(int sock, char fname[], int len)
{
    if ((writen(sock, fname, len) < 0))
    {
        printf("client :  write error %d\n", errno);
        exit(0);
    }

    return fname;
}

int readn(int sd, const char *ptr, int size)
{
    int no_left, no_read;
    no_left = size;
    while (no_left > 0)
    {
        no_read = read(sd, ptr, no_left);
        if (no_read < 0)
            return (no_read);
        if (no_read == 0)
            break;
        no_left -= no_read;
        ptr += no_read;
    }
    return (size - no_left);
}

int writen(int sd, const char *ptr, int size)
{
    int no_left, no_written;
    no_left = size;
    while (no_left > 0)
    {
        no_written = write(sd, ptr, no_left);
        if (no_written <= 0)
            return (no_written);
        no_left -= no_written;
        ptr += no_written;
    }
    return (size - no_left);
}