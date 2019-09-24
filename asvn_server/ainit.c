#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

void main()
{
	/*repos path*/
	if(creat("./repos",0644) == -1){
		perror("open");
		exit(1);
	}
	/*exe file*/
	if(mkdir("./.asubversion", 0777) != 0){
		perror("mkdir");
		exit(1);	
	}
	/*user file*/
	if(mkdir("./conf", 0777) != 0){
		perror("mkdir");
		exit(1);
	}

	/*user id*/
	if(creat("./conf/authz",0644) == -1){
		perror("open");
		exit(1);
	}

	/*user passwd*/
	if(creat("./conf/passwd",0644) == -1){
		perror("open");
		exit(1);
	}

}
