# ASVN

2016년, 시스템 프로그래밍 학기 과제용으로 만든 팀 프로젝트입니다.

FTP 프로그래밍을 이용하여 서버와 클라이언트 간의 파일 전송하고, 버전관리를 위한 log 기능을 제공합니다.

## 맡은 역할

**제가 맡은 부분의 코드는 asvn_client/ 에서 모두 보실 수 있습니다.**

- Makefile 구현
- 관리 디렉토리 설계, 구현
- 명령어 처리 구조 설계, 구현
    - parse_command(), response(), creat_log(), write_log(), logfile(), lookup_cmd(), lookup()
- 사용자 함수 설계, 구현
    - asvn_remove(), asvn_pwd(), asvn_readlog(), asvn_mkdir(), asvn_creat()

## Project Stack

- C
- gcc
- make
- ftp protocol

## Running

![execution](./img/execution.jpg)

![execution](./img/execution_command-log.jpg)

---
## Reference Source

- https://github.com/Siim/ftp
