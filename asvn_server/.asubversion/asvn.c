#include "common.h"

void asvn_update(State *state)
{
    dostr(state->sock); //real work
}

void ftp_quit(State *state)
{
    state->message = "221 Goodbye, friend. I never thought I'd die like this.\n";
    exit(0);
}

/* FILE TRANSFER (GIVE to CLIENT)*/
void asvn_retr(int sockid)
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
        printf("server: filename read error :%d\n", errno);
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
                    //n=fread(buf, sizeof(char) * 2, 1, rfp
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
                }

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
                    }
                    printf("send data : %d/%d\n", recv_fsize, fsize);
                    //read recv info
                    sock_len = read(sockid, dataBuf, sizeof(dataBuf));
                    if (sock_len < 0)
                    {
                        printf("error\n");
                        exit(0);
                    }
                    if ((strcmp(dataBuf, "file_recv")) == 0)
                    {
                        printf("file send complate!!\n");
                        fclose(fp);
                        close(sockid);
                        exit(0);
                    }
                }
            }
        }
    }
}

void main(int argc, char *args[])
{
    int len;
    int sockid, newsd, pid, clilen;
    struct sockaddr_in client_addr;

    Command *cmd = malloc(sizeof(Command));
    State *state = malloc(sizeof(State));
    state->logged_in = 1;

    while (1 == 1)
    {
        printf("server: starting accept\n");
        clilen = sizeof(client_addr); //clean accept, before start.-> cause accept error
        if ((newsd = accept(sockid, (struct sockaddr *)&client_addr, &clilen)) < 0)
        {
            printf("server: accept  error :%d\n", errno);
            exit(0);
        } //error처리
        printf("server: return from accept, socket for this ftp: %d\n", newsd);
        if ((pid = fork()) == 0)
        {
            close(sockid); /* child shouldn't do an accept */
            state->sock = newsd;
            dostr(newsd); //real work
            close(newsd);
            exit(0); /* child all done with work */
        }
        close(newsd);
    }
}
