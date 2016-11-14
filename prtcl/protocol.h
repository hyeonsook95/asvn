#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef struct create{
	char ord[3]; //명령어 
	int ptr; //로컬경로 길이 
	char pth[200];//로컬경로 
}cr;

typedef struct checkout{
	char ord[3];
	int ath;
	int ptr;
	char pth[200];
}ch;

typedef struct import{
	char ord[3];
	int ath;
	int ptr;
	char pth[200];
	int urr;
	char url[200];
	char msg[200];
	char fnm[30]; //파일이름 
}im;

typedef struct update{
	char ord[3];
	int ath;
	int ptr;
	char pth[200];
	int urr;
	char url[200];
}up;

typedef struct merge{
	char ord[3];
	int ath;
	int ptr;
	char pth[200];
	int urr;
	char url[200];
}me;

typedef struct commit{
	char ord[3];
	int ath;
	int ptr;
	char pth[200];
	int urr;
	char url[200];
};

typedef struct response{
	int result;
	char fnm[30];
}re;

#endif PROTOCOL_H 
