#include "common.h"
/** 
 * Sets up server and handles incoming connections
 * @param port Server port
 */
void server(int port){}
void client(int port)
{
char welcome[BSIZE] ;
int sock = create_csocket(port);
int connection, pid, bytes_read;
char buffer[BSIZE];
int msg;
char * server_addr = "127.0.0.1";

Command *cmd = malloc(sizeof(Command));
State *state = malloc(sizeof(State));
state->logged_in =1; 
memset(buffer,0,BSIZE);
//connection success
if((read(sock,welcome,BSIZE)) <0){
	printf("server :  read error %d \n", errno); 
	exit(0);
	}
	printf("received : %s \n",welcome);

	//throw command to Server
	gets(buffer);
	strcat(buffer,"\0");
	if(write(sock,buffer,BSIZE)<0){
	perror("write to server error\n");
	exit(0);
	}

     /* Read commands-response from server */
      while (bytes_read = read(sock,buffer,BSIZE)){
	printf("received : %s \n",buffer);

	//throw command to Server
	gets(buffer);
	strcat(buffer,"\0");
	if(write(sock,buffer,BSIZE)<0){
	perror("client read error\n");
	exit(1);
	}//if
	}//while 
sprintf(buffer,"Client disconnected.\n");
        write(sock,buffer,BSIZE);
      close(connection);


}

/**
 * Creates socket on specified port and starts listening to this socket
 * @param port Listen on this port
 * @return int File descriptor for new socket
 */

int create_socket(int port)
{
  int sock;
  int reuse = 1;

  /* Server addess*/ 
  struct sockaddr_in server_address = (struct sockaddr_in){  
     AF_INET,
     htons(port),
     (struct in_addr){INADDR_ANY}
  };


  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot open socket");
    exit(EXIT_FAILURE);
  }

  /* Address can be reused instantly after program exits */
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

  /* Bind socket to server address */
  if(bind(sock,(struct sockaddr*) &server_address, sizeof(server_address)) < 0){
    fprintf(stderr, "Cannot bind socket to address");
    exit(EXIT_FAILURE);
  }

  listen(sock,5);
  return sock;
}

int create_csocket(int port)
{
int sock;
int reuse = 1;
char *server_addr = "127.0.0.1";


  /* Server addess */
  struct sockaddr_in server_address = (struct sockaddr_in){
     AF_INET,
     htons(port),
     (struct in_addr){inet_addr(server_addr)}
  };

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot open socket");
    exit(EXIT_FAILURE);
  }

  /* Address can be reused instantly after program exits */
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

       /* Connect client socket to server address */
  if(connect(sock,(struct sockaddr*) &server_address, sizeof(server_address)) < 0){
    fprintf(stderr, "Cannot connect socket to address");
    exit(EXIT_FAILURE);
  }
printf("client connected successed");
return sock;
}


/**
 * Accept connection from client
 * @param socket Server listens this
 * @return int File descriptor to accepted connection
 */
int accept_connection(int socket)
{
  int addrlen = 0;
  struct sockaddr_in client_address;
  addrlen = sizeof(client_address); //Client�� �ּ��� ũ�� ���� 
  return accept(socket,(struct sockaddr*) &client_address,&addrlen); //accept���� 
}

/**
 * Get ip where client connected to
 * @param sock Commander socket connection
 * @param ip Result ip array (length must be 4 or greater)
 * result IP array e.g. {127,0,0,1}
 */
void getip(int sock, int *ip)
{
  socklen_t addr_size = sizeof(struct sockaddr_in);
  struct sockaddr_in addr;
  getsockname(sock, (struct sockaddr *)&addr, &addr_size);//������ ���������ڿ� ���� ������ ��ȯ 
 
  char* host = inet_ntoa(addr.sin_addr);
  sscanf(host,"%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3]);
}

/**
 * Lookup enum value of string
 * @param cmd Command string 
 * @return Enum index if command found otherwise -1
 */

int lookup_cmd(char *cmd){
  const int cmdlist_count = sizeof(cmdlist_str)/sizeof(char *); //(���ɾ� ����Ʈ/�ּҰ�ũ�� )=���ɾ� ����? 
  return lookup(cmd, cmdlist_str, cmdlist_count); //�Ѿ�� ����, ����� ���ɹ迭, ���ɾ�� �ѱ��� 
}

/**
 * General lookup for string arrays
 * It is suitable for smaller arrays, for bigger ones trie is better
 * data structure for instance.
 * @param needle String to lookup
 * @param haystack Strign array
 * @param count Size of haystack
 */
int lookup(char *needle, const char **haystack, int count)
{ 
  int i;
  for(i=0;i<count; i++){ //���ɾ� ������ŭ ����. 
    if(strcmp(needle,haystack[i])==0)return i; //�´� ���ɾ��϶� return! 
  }
  return -1;
}


/** 
 * Writes current state to client
 */
void write_state(State *state)
{
  write(state->connection, state->message, strlen(state->message));
}

/**
 * Generate random port for passive mode
 * @param state Client state
 */
void gen_port(Port *port)
{
  srand(time(NULL)); // �����ð� 
  port->p1 = 128 + (rand() % 64); //
  port->p2 = rand() % 0xff;
}

/**
 * Parses FTP command string into struct
 * @param cmdstring Command string (from ftp client)
 * @param cmd Command struct
 */
void parse_command(char *cmdstring, Command *cmd)
{
printf("%s %s\n",cmdstring, cmd);
  sscanf(cmdstring,"%s %s",cmd->command,cmd->arg);
printf("%s\n ",cmdstring);
}

/**
 * Handles zombies
 * @param signum Signal number
 */
void my_wait(int signum)
{
  int status;
  wait(&status);
}

/* DUE TO THE FACT THAT BUFFER LIMITS IN KERNEL FOR THE SOCKET MAY BE 
   REACHED, IT IS POSSIBLE THAT READ AND WRITE MAY RETURN A POSITIVE VALUE
   LESS THAN THE NUMBER REQUESTED. HENCE WE CALL THE TWO PROCEDURES
   BELOW TO TAKE CARE OF SUCH EXIGENCIES */

int readn(int sd,char *ptr,int size)

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


int writen(int sd,const void *ptr,int size)
{         int no_left,no_written;
 //        const char *ptr =NULL;
//             ptr=ptr;
           no_left = size;
           while (no_left > 0)
                      {
                                                if((no_written = write(sd,ptr,no_left)<=0)){
                        if(no_written <0&& errno==EINTR)
                                                                        no_written =0;
                                                else
                                                        return(-1);
                                                }
                        no_left -= no_written;
                       ptr += no_written;
                     }
           return(no_left);
}
                                                       



main()
{
//  server(6081);
	client(6096);
  return 0;

}


