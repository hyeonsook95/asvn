#include "common.h"
/* arg로 입력받은 디렉토리를 삭제하고 log에 기록하는 사용자 함수 */
void asvn_remove(Command *cmd, State *state)
{
    if (remove(cmd->arg) == -1)
    {
        perror("remove");
        exit(1);
    }

    write_log(cmd, state);
}

/* 현재 디렉토리의 위치를 출력해주는 사용자 함수 */
void asvn_pwd(Command *cmd, State *state)
{
    char currentpath[BSIZE];

    memset(currentpath, '\0', BSIZE);

    getcwd(currentpath, BSIZE);
    printf("%s\n", currentpath);
}

/* 현재 저장소의 log 기록을 출력해주는 사용자 함수 */
void asvn_readlog(Command *cmd, State *state)
{
    char logpath[BSIZE];
    char temp[BSIZE];
    char *path = NULL;
    FILE *fp_log;

    memset(logpath, '\0', BSIZE);
    memset(temp, '\0', BSIZE);
    logfile(logpath);

    if (!(fp_log = fopen(logpath, "r")))
    {
        perror("fopen");
        exit(1);
    }

    printf("---------------------------------------------------------------\n");
    while (fgets(temp, 1024, fp_log))
    {
        printf("%s", temp);
    }
    printf("---------------------------------------------------------------\n");

    fclose(fp_log);
}

/* arg로 입력받은 경오 상에 입력받은 값으로 디렉토리를 생성하고 log에 기록하는 사용자 함수 */
void asvn_mkdir(Command *cmd, State *state)
{
    char *dirpath = NULL;
    int fd = 0;

    if (mkdir(cmd->arg, 0755))
    {
        perror("mkdir");
        exit(1);
    }

    write_log(cmd, state);
}

/* 현재 위치에 저장소를 생성하고, 해당 저장소의 log 관리 디렉토리를 생성하는 함수 */
void asvn_creat(Command *cmd, State *state) //enum 23
{
    char *asvnpath = NULL;
    int fd = 0;

    asvnpath = getenv("HOME");
    strcat(asvnpath, "/asvn/repos");

    /*write asvn/repos*/
    if ((fd = open(asvnpath, O_WRONLY, 0777)) == -1)
    {
        perror("open");
        exit(1);
    }

    if (write(fd, asvnpath, strlen(asvnpath)) == -1)
    {
        perror("write");
        exit(1);
    }

    /*create .asvn & log file*/
    if (mkdir("./.asvn", 0777) == -1)
    {
        perror("mkdir");
        exit(1);
    }

    if ((fd = creat("./.asvn/log", FAUTH)) == -1)
    {
        perror("creat");
        exit(1);
    }

    char *temp = "# date username cmd cmt\n";

    if (write(fd, temp, strlen(temp)) == -1)
    {
        perror("write");
        exit(1);
    }
}

void asvn_update(State *state)
{
    int sockid;
    printf("socket staring\n");
    sockid = sock();
    //클라이언트로부터 파일을 보낼 때
    doftp(sockid);
    printf("asvn is updatign .. \n");
}

void ftp_quit(State *state)
{
    state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
    exit(0);
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

void main(int argc, char *args[])
{
    char *fname[2];
    int sockid, i = 0;
    fname[0] = "text.txt";
    fname[1] = "sun.jpeg";
    printf("1: %s\n", fname[0]);
    printf("2: %s\n", fname[1]);
    Command *cmd = malloc(sizeof(Command));
    State *state = malloc(sizeof(State));
    state->logged_in = 1;

    strcpy(cmd->command, args[1]);
    parse_command(argc, args, cmd);

    if (cmd->command[0] <= 127 || cmd->command[0] >= 0)
    {                         //is command ASKII code
        response(cmd, state); //user function
    }
}
