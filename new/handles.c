//System functions
#include "common.h"

/**
 * Lookup enum value of string
 * @type
 * @param cmd Command string 
 * @return Enum index if command found otherwise -1
 */
int lookup(char *cmd, const char **haystack, int count)
{
  int i;
  for(i=0; i<count; i++){
    if(strcmp(cmd, haystack[i]) == 0) return i;
  }
  return -1;
}

/**
 * General lookup for string arrays
 * It is suitable for smaller arrays, for bigger ones trie is better
 * data structure for instance.
 * @type
 * @param needle String to lookup
 * @param haystack Strign array
 * @param count Size of haystack
 * @return lookup
 */
int lookup_cmd(char *cmd)
{
  const int cmdlist_count = sizeof(cmdlist_str) / sizeof(char *);

  return lookup(cmd, cmdlist_str, cmdlist_count);
}


void response(Command *cmd, State *state)
{
  switch(lookup_cmd(cmd->command)){ //user function -> command's enum number
    case create: asvn_creat(cmd,state); break;
    case log: asvn_readlog(cmd,state); break;
    case mkdir: asvn_mkdir(cmd,state); break;
    case delete: asvn_remove(cmd,state); break;
    case pwd: asvn_pwd(cmd, state); break;
    case quit: ftp_quit(state); break;
    default:
      state->message = "500 Unknown command\n";
      break;
  }
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
  sprintf(content, "%s %s %s %s %s\n", c_time, state->username, cmd->command, cmd->comment, state->message);
}

/**
 * Logging to 'localref/.asvn/logs/LOG' file
 * @type System def
 * @param Command = command,arg,cmt ..., State = user's logged_in,username ...
 */
/*
지역 repository의 log file을 찾는다.
log file을 연다.

log content를 생성한다.

file에 log content를 기록한다.
*/
int save_log(Command* cmd, State *state)
{
  char path[BSIZE]; //log file
  char temp[BSIZE]; //log
  FILE *fp_log;
	
  memset(path, '\0', 1024);
  memset(temp, '\0', 1024);
	
  getcwd(path, sizeof(path));
  strcat(path, "/.asvn/logs/LOG");


  if((fp_log = fopen(path, "w")) == NULL){
    fprintf(stderr, "%s file open error: %s\n", path, strerror(errno));
    return -1;
  }

  if(fwrite(temp, strlen(temp), 1, fp_log) <= 0){
    fprintf(stderr, "%s logging error: %s\n", temp, strerror(errno));
    return -1;
  }

  fclose(fp_log);       
}

/**
 * For variable command input
 * @param Command struct Command
 * @param argc Number of inputed variables
 * @param args Inputed command from user
 */
void set_command(Command *cmd, int argc, char* args[])
{
  
  switch(argc){
    case 3: strcpy(cmd->command, args[1]);
            strcpy(cmd->arg, args[2]); break;
    case 4: strcpy(cmd->command, args[1]);
            strcpy(cmd->arg, args[2]);
            strcpy(cmd->comment, args[3]); break;
    default: strcpy(cmd->command, args[1]); break;
  }
}

/**
 * To initialize Command and State
 * @param Command struct Command
 * @param argc Number of inputed variables
 * @param args Inputed command from user
 */
void init(Command *cmd, State* state)
{
  Command cm = {.command = "\0", .arg = "\0", .comment = "\0"};
  cmd = &cm;  

  state->username = "\0";
  state->message = "Have a nice day!";
}

void response(Command *cmd, State *state)
{
  switch(lookup_cmd(cmd->command)){ //user function -> command's enum number
    case create: asvn_creat(cmd,state); break;
    case log: asvn_readlog(cmd,state); break;
    case mkdir: asvn_mkdir(cmd,state); break;
    case delete: asvn_remove(cmd,state); break;
    case pwd: asvn_pwd(cmd, state); break;
    case quit: ftp_quit(state); break;
    default:
      state->message = "500 Unknown command\n";
      //write_state(state);
      break;
  }
}
