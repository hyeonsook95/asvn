// local에서 실행될 main program, client program이라고도 할 수 있음, 항상 실행되고 있지는 않음 1회성
#include "common.h"

main(int argc, char* args[])
{
  Command *cmd = malloc(sizeof(Command));
  State *state = malloc(sizeof(State));

  init(cmd, state);
  set_command(cmd, argc, args);
   
  if(cmd->command[0]<=127 && cmd->command[0]>=0){
     response(cmd);
  }
}
