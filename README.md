# ASVN

## 프로젝트 개요
2016년, 시스템 프로그래밍 학기 과제용으로 만든 팀 프로젝트입니다.

FTP 프로그래밍을 이용하여 서버와 클라이언트 간의 파일 전송을 구현하고, 버전관리를 위한 기능을 구현합니다.

## 구현된 기능
### 네트워크가 필요한 기능
* create
* checkout

### 네트워크가 필요하지 않은 기능
* mkdir
* delete
* pwd
* log

## 참고자료
* subversion api site : 
https://subversion.apache.org/docs/api/latest/modules.html

* subversion source site :
svn protocol 참조 : https://svn.apache.org/repos/asf/subversion/trunk/subversion/libsvn_ra_svn/protocol

* suv 기본 명령어참조 사이트 : http://welchsy.tistory.com/275

* 우리가 공부한 svn 사이트

  http://www.pyrasis.com/main/Subversion-HOWTO -> svn사용방법 간단정리

  https://wiki.kldp.org/wiki.php/SubversionBook -> 전체적으로 svn을 책으로 정리

* https://github.com/Siim/ftp/blob/master/README.md


## 구현하지 못한 기능
### 네트워크가 필요한 기능
* import
* commit
* update

### 네트워크가 필요하지 않은 기능
* mv
* add
* diff
