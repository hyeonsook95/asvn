//mkdir test 함수
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{
	char dirName[200];
	char* path;
	int pt;
	
	//현재 위치를 구하고 현재 위치에 .asvn을 생성

	//현재 위치 구하기
	getcwd(dirName, 200);
	printf("dirName: %s\n", dirName);

	//현재 위치에 .asvn 생성

	path = dirName;
	strcat(path, "/.asvn");
	printf("path: %s\n", path);

	if(mkdir(path, 0777) ==-1){
		perror("mkdir");
		exit(1);
	}
	mkdir("test");

	strcat(path, "/log");
	pt = creat(path, 0644);

	char *temp = "20161201 user1 mkdir trunk\n";
	write(pt, temp, strlen(temp));
}
