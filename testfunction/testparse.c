#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <netinet/in.h>
#include <time.h>
#include <dirent.h>

#ifndef BSIZE
  #define BSIZE 1024
#endif

#ifndef FAUTH
	#define FAUTH 0644
#endif

typedef struct Command
{
  char command[10];
  char arg[BSIZE];
  char comment[BSIZE];
} Command;

