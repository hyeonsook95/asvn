//System functions
#include "common.h"

/**
 * pwd
 * @param pwd path
 * @return erro number
 */
int c_pwd(char *p)
{
  char path[BSIZE];
  memset(path, '\0', BSIZE);

  if(getcwd(path, BSIZE) == NULL){
    fprintf(stderr, "Current working directory get error: %s\n", strerror(errno));
    exit(0);
  {

  strcpy(p, path);
}

/**
 * thread for scan dir list
 * @param struct Path{ conf: .asvn/info/struct path: opendir path }
 */
void* scan_thread(void *path)
{
  Path *p = path;

  int thr_id;
  int status;
  pthread_t p_thread[1];

  DIR *dir_ptr = NULL;
  struct dirent *file = NULL;
  struct stat buf;
  char *ext;
  char filename[BSIZE];

  memset(filename, '\0', BSIZE);
  strcpy(filename, p->path);

  if((dir_ptr = opendir(filename)) == NULL){
    exit(0);
  }

  while((file = readdir(dir_ptr)) != NULL){

    if(strcmp(file->d_name, ".") == 0){
      continue;
    }
    else if(strcmp(file->d_name, "..") == 0){
      continue;
    }

    sprintf(p->path, "%s/%s", filename, file->d_name);

    if(stat(p->path, &buf) == -1){
      continue;
    }

    if(S_ISREG(buf.st_mode)){
      write_fp(p->conf, p->path);
    }
    else if(S_ISDIR(buf.st_mode)){
      thr_id = pthread_create(&p_thread[0], NULL, scan_thread, (Path *)p);

      pthread_join(p_thread[0], (void **) &status);
    }
  }
  closedir(dir_ptr);
}

/**
 * scan dir list
 * 파일의 추가 삭제시 변경된 파일의 내용을 기록하기 위한 함수
 */
void scan_dir()
{
  Path *p = malloc(sizeof(Path));

  find_info(p->conf);

  strcpy(p->path, p->conf);
  strcat(p->conf, "/.asvn/info/struct");

  scan_thread(p);
  free(p);
}

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


int write_fp(char *path, char *temp)
{
  FILE *fp;

  if((fp = fopen(path, "a")) == NULL){
    fprintf(stderr, "%s file open error: %s\n", path, strerror(errno));
    return -1;
  }

  if(fwrite(temp, strlen(temp), 1, fp) <= 0){
    fprintf(stderr, "%s error: %s\n", temp, strerror(errno));
    return -1;
  }

  fclose(fp);       

  return 0;
}

int read_fp(char *path, char *temp)
{
  int size;
  int cnt;

  FILE *fp;

  if((fp = fopen(path, "r")) == NULL){
    fprintf(stderr, "%s file open error: %s\n", path, strerror(errno));
    return -1;
  }

  fseek(fp, 0, SEEK_END);
  size = ftell(fp);

  temp = malloc(size+1);
  memset(temp, 0, size + 1);

  fseek(fp, 0, SEEK_SET);
  cnt = fread(temp, size, 1, fp);

  fclose(fp);

  return 0;
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
 * Logging to 'local_ref/.asvn/logs/LOG' file
 * @type System def
 * @param Command = command,arg,cmt ..., State = user's logged_in,username ...
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

  creat_log(cmd, state, temp)

  write_fp(path, temp)   
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
 * To initialize Command and State ??서버아니면 필요없을듯
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
    case create: asvn_create(cmd,state); break;
/*    case log: asvn_readlog(cmd,state); break;
    case mkdir: asvn_mkdir(cmd,state); break;
    case delete: asvn_remove(cmd,state); break;
    case pwd: asvn_pwd(cmd, state); break;
    case quit: ftp_quit(state); break;*/
    default:
      state->message = "500 Unknown command\n";
      //write_state(state);
      break;
  }
}
