
							프로토콜 정의								

	서버와 클라이언트간의 통신이 있는 명령어의 경우 프로토콜을 정의하여 통신한다.						
	asvn은 TCP/IP 소켓을 기반으로 통신하며, 리눅스 기반 환경에서 개발하였으며 리눅스 기반 환경을 전제로 프로그래밍한다.



	프로토콜로 정의하는 명령어 종류
	create. checkout, import, update, merge. commit
	



	[1] create: (명령어:char 저장소위치:char)

	[2] checkout: (명령어:char 인증:int 서버경로:char)

	[3] import: (명령어:char 인증:int 로컬경로:char 서버경로:char 메세지:char 파일:char)

	[4] update: (명령어:char 인증:int 로컬경로:char 서버경로:char)

	[5] merge: (명령어:char 인증:int 로컬경로:char 서버경로:char)
	    merge는 서버와의 merge하는 상황이나 클라이언트와 서버의 merge하는 상황일 경우 사용된다.

	[6] commit: (명령어:char 인증:int 로컬경로:char 서버경로:char)

	[7] response: (에러번호:int 파일:char)
	    에러코드를 정의하여 명령어의 실행결과를 전달받고 결과에 따라 안내메시지를 출력한다.