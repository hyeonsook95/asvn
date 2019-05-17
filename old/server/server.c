#include "common.h"
/** 
 * Sets up server and handles incoming connections
 * @param port Server port
 */
void server(int port)
{
  int sock = create_socket(port);
  struct sockaddr_in client_address;
  int len = sizeof(client_address);
  int connection, pid, bytes_read;
 char buf[BSIZE];
  while(1){
    connection = accept(sock, (struct sockaddr*) &client_address,&len);
    char buffer[BSIZE];
    Command *cmd = malloc(sizeof(Command));
    State *state = malloc(sizeof(State));
   pid = fork();
   printf("pid : %d\n",pid); 
    memset(buffer,0,BSIZE);
////////////////process error
    if(pid<0){
      fprintf(stderr, "Cannot create child process.");
      exit(EXIT_FAILURE);
    }
///////////////if create chile process
    if(pid==0){
	printf("welcome client!\n");
      close(sock);
      char welcome[BSIZE] = "220 ";
      if(strlen(welcome_message)<BSIZE-4){
	printf("okokok1\n");
        strcat(welcome,welcome_message);
      }else{
	printf("xxxxxx1\n");
        strcat(welcome, "Welcome to nice FTP service.");
      }

      /* Write welcome message */
     strcat(welcome,"\0");
	printf("%s",welcome);
      write(connection, welcome,strlen(welcome));
	printf("write okokok\n");

      /* Read commands from client */
      while (bytes_read = read(connection,buffer,BSIZE)){
       
        signal(SIGCHLD,my_wait);

        if(!(bytes_read>BSIZE)){
//	write(connection,buf,strlen(buf));
          /* TODO: output this to log */
          buffer[BSIZE-1] = '\0';
        printf("User %s sent command: %s\n",(state->username==0)?"unknown":state->username,buffer);
//	strcat(buf,"\0");
//	write(connection,buf,strlen(buf));
          parse_command(buffer,cmd);
        //  printf("User %s sent command: %s\n",(state->username==0)?"unknown":state->username,buffer);
         // parse_command(buffer,cmd);
          state->connection = connection;
          
          /* Ignore non-ascii char. Ignores telnet command */
          if(buffer[0]<=127 || buffer[0]>=0){
            response(cmd,state);
          }
          memset(buffer,0,BSIZE);
          memset(cmd,0,sizeof(cmd));
        }else{
          /* Read error */
          perror("server:read");
        }

sprintf(buf,"User %s sent command: %s\n",(state->username==0)?"unknown":state->username,buffer);
        strcat(buf,"\0");
        write(connection,buf,strlen(buf));


      }
      printf("Client disconnected.\n");
      exit(0);
if(pid<0){
      fprintf(stderr, "Cannot create child process.");
      exit(EXIT_FAILURE);
    }
    }else{
      printf("closing... :(\n");
      close(connection);
    }
  }
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

  /* Server addess */
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

int create_csocket(int port, int cport)
{
int sock;
int reuse = 1;

/*Client address*/
struct sockaddr_in client_address = (struct sockaddr_in){
AF_INET,
htons(cport),
(struct in_addr){INADDR_ANY}
};

  /* Server addess */
  struct sockaddr_in server_address = (struct sockaddr_in){  
     AF_INET,
     htons(port),
     (struct in_addr){inet_addr(port)}
  };
	/* Connect client socket to server address */
  if(connect(sock,(struct sockaddr*) &server_address, sizeof(server_address)) < 0){
    fprintf(stderr, "Cannot bind socket to address");
    exit(EXIT_FAILURE);
  }
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
  const int cmdlist_count = sizeof(cmdlist_str)/sizeof(char *); //(��ɾ� ����Ʈ/�ּҰ�ũ�� )=��ɾ� ����? 
  return lookup(cmd, cmdlist_str, cmdlist_count); //�Ѿ�� ���, ����� ��ɹ迭, ��ɾ�� �ѱ��� 
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
  for(i=0;i<count; i++){ //��ɾ� ������ŭ ����. 
    if(strcmp(needle,haystack[i])==0)return i; //�´� ��ɾ��϶� return! 
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
  sscanf(cmdstring,"%s %s",cmd->command,cmd->arg);
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

main()
{
  server(6096);

  return 0;

}
