// local user function
#include "common.h"

/**
 * Parse inputed command from user
 * @type System def
 * @param args[1] = command, args[2] = orgpath, (args[3] = newpath, args[4~] = cmt)
 */
void parse_command(int argc, char **args, Command *cmd)
{
	int n = argc - 2;
	int i = 0;
	
	switch(lookup_cmd(cmd->command)){
		/*Only command*/
		case LOG: case CREAT: case COMMIT:case CHECKOUT: 
		case UPDATE: case PWD: case QUIT: 
			break;
		/*command and path*/
		case MKDIR: case DELETE: case ADD:
			strcpy(cmd->arg, args[2]); n--; break;
		/*command, origin path, new path*/
		case MV: case DIFF: case MERGE :
			strcpy(cmd->arg, args[2]); n -= 2;
			strcat(cmd->arg, " "); strcat(cmd->arg, args[3]);
			break;
		default:
			printf("500 Unknown command\n");
			break;
	}

	memset(cmd->cmt, '\0', sizeof(cmd->cmt));

	/*inputed comment on the command from user*/		
	for(i=n; i<argc-3; i++){
		if(argc != i-1){
			strcat(cmd->cmt, args[i]);
			strcat(cmd->cmt, " ");
		}
		else
			strcat(cmd->cmt, "\n");
	}
	
	return;
}



/**
 * Logging to '/home/username/asvn/repos' file
 * @type System def
 * @param Command = command,arg,cmt ..., State = user's logged_in,username ...
 */
void write_log(Command* cmd, State *state)
{
	char path[200];
	char path[BSIZE];
	char temp[500];
	FILE *fp_log;
	
	memset(logpath, '\0', BSIZE);
	memset(temp, '\0', 500);
	
	/*set .asvn path*/
	logfile(path);

	if((fp_log=fopen(path, "a")) ==NULL){
		perror("fopen");
		exit(1);
	}

	creat_log(cmd, state, temp);

	/*write log in .asvn*/
	if(fwrite(temp, 1, strlen(temp), fp_log) ==-1){
		perror("fwrite");
		exit(1);
	}

//	printf("Success!\n");

	if(fclose(fp_log) ==-1){
		perror("fclose");
		exit(1);
	}
}

/**
 * 
 * @type System def
 * @param cmd = arg of a command inputed from user
 * @return 
 */
int lookup_cmd(char *cmd){
  const int cmdlist_count = sizeof(cmdlist_str)/sizeof(char *);
  int a = lookup(cmd, cmdlist_str, cmdlist_count);
  return a;
}

/**
 * General lookup for string arrays
 * It is suitable for smaller arrays, for bigger ones trie is better
 * data structure for instance.
 * @type
 * @param needle String to lookup
 * @param haystack Strign array
 * @param count Size of haystack
 * @return 
 */
int lookup(char *needle, const char **haystack, int count)
{ 
  int i;
  for(i=0;i<count; i++){ //(enum cmdlist & char* cmflist_str)'s count
   if(strcmp(needle,haystack[i])==0)return i; //needel == haystack[i] -> return i
  }
  return -1;
}

/**
 *
 * @type
 * @param
 * @return 
 */
void logfile(char path[100])
{
	char *asvnpath = NULL;
	char logpath[BSIZE];
	char temp[BSIZE];
	FILE *fp_log;
	
	memset(temp, '\0', BSIZE);
	
	asvnpath=getenv("HOME");
	strcat(asvnpath, "/asvn/repos");
	getcwd(logpath, 1024);
	
	if(!(fp_log=fopen(asvnpath, "r"))){
		perror("repos fpoen");
		exit(1);
	}
	while(fgets(temp, 1024, fp_log)){
		if(strstr(logpath, temp)){
			printf("temp: %s\n", temp);
			strcpy(logpath, temp);
		}
	}

	strcat(logpath, "/.asvn/log");

	strcpy(path, logpath);
}

/**
 * Creat log contents
 * @type System def
 * @param Command Executed command
 * @param State Status of the user who executed the command
 * @param content Log content, Executed result
 */

void creat_log(Command *cmd, State *state, char *content)
{
  time_t rawt;
  struct tm *t;
  char c_time[20];

  memset(content, '\0', BSIZE);
  memset(c_time, '\0', 20);

  time(&rawt);
  t = localtime(&rawt);
  sprintf(c_time, "%4d-%2d-%2d-%2d:%2d:%2d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

  /* set log [date, username, cmd, cmt] */
  sprintf(content, "%s %s %s %s\n", c_time, state->username, cmd->command, cmd->comment, state->message);
}

void main(int argc, char* args[])
{
  Command *cmd = malloc(sizeof(Command));

  State *state = malloc(sizeof(State));
  state->logged_in =1;

  strcpy(cmd->command, args[1]);
  strcpy(cmd->arg, args[2]);
  strcpy(cmd->comment, args[3]);
   
  if(cmd->command[0]<=127 && cmd->command[0]>=0){
     response(cmd);
  }
}
