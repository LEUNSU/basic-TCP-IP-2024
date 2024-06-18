# basic-TCP-IP-2024
IoT 개발자 TCP/IP 네트워크 프로그래밍 학습 리포지토리

## 1일차
- TCP/IP - cmd > ipcofig
- 전송방식 : TCP/UDP
- 소(소켓 socket) 말(바인더 bind) 리(리슨 listen) 아(엑셉트 accept)

## 2일차

## 3일차
- Half-Close
- IP주소, 도메인 이름 변환
- 프로세스(ps명령문 : ps au)
    - fork 함수 
- 좀비프로세스 생성

## 4일차
- 좀비프로세스 소멸
    - wait 함수
    - waitpid 함수
- 시그널 핸들링 - 자식 프로세스 종료 인식 부모 프로세스에게 전달
    - sigaction 함수
- 다중접속 서버
- 멀티플렉싱

## 5일차
- send & recv 함수
- readv & writev 함수
- 표준 입출력 함수

## 6일차
- epoll - select 함수의 단점 극복
- 레벨 트리거 | 엣지 트리거
- 쓰레드 
    - 쓰레드 동기화
        - 뮤텍스 - 쓰레드의 동시접근 허용 X
        - 세마포어
