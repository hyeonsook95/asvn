#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 7000

typedef struct test{
	char s[20];
	int num;
}test;

void main()
{
	int s_sck, c_sck;
	struct sockaddr_in c_addr, s_addr;
	int len;
	int n, i;
	
	char temp[50];
	int length;

	s_sck = socket(PF_INET, SOCK_STREAM, 0);
	
	if(bind(s_sck, (struct sockaddr*)&s_addr, sizeof(s_addr))==-1){
		printf("Can not Bind\n");
		exit(1);
	}

	if(listen(s_sck, 2)==-1){
		printf("Listen fail\n");
		exit(1);
	}

	length = 0;
	while(1){
		len = sizeof(c_addr);
		accept(s_sck, (struct sockaddr*)&c_addr, &len);
		
		//recv
		struct test *tt;
		n = recv(c_sck, temp, 50, 0);

		temp[n] = '\0';
		tt = (test*)temp;

		printf("test: (char)%s, (int)%d\n", tt->s, tt->num);
		
		close(c_sck);
	}
	close(s_sck);
}
