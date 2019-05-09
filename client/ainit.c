#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

void main()
{
	char path[1024];
	char orgpath[1024];
	char temp[1024];

	memset(path, '\0', 1024);
	memset(orgpath, '\0', 1024);
	
	getcwd(temp, 1024);
	strcpy(path, getenv("HOME"));
	strcat(path, "/asvn");

	strcpy(orgpath, path);

	//HOME 파일 안에 asvn 안에 생성되도록
	/*asvn path*/
	if(mkdir(path, 0777) == -1){
		perror("asvn creat");
		exit(1);
	}

	/*repos path*/
	strcat(path, "/repos");
	if(creat(path,0644) == -1){
		perror("repos creat");
		exit(1);
	}

	/*exe file*/
	strcpy(path, orgpath);
	strcat(path, "/.asubversion");
	if(mkdir(path, 0777) != 0){
		perror("exe mkdir");
		exit(1);	
	}

	/*user file*/
	strcpy(path, orgpath);
	strcat(path, "/conf");
	if(mkdir(path, 0777) != 0){
		perror("usr mkdir");
		exit(1);
	}

	/*user id*/
	strcpy(path, orgpath);
	strcat(path, "/conf/authz");
	if(creat(path, 0644) == -1){
		perror("id creat");
		exit(1);
	}

	/*user passwd*/
	strcpy(path, orgpath);
	strcat(path, "/conf/passwd");
	if(creat(path, 0644) == -1){
		perror("pas creat");
		exit(1);
	}
}
