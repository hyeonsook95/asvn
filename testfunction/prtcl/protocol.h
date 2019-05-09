#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef struct create{
	char ord[3]; //��ɾ� 
	int ptr; //���ð�� ���� 
	char pth[200];//���ð�� 
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
	char fnm[30]; //�����̸� 
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
