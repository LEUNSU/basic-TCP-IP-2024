// title : hello_client.c
// date : 2024-06-12
// desc : 클라이언트 프로그램 복습

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[30];
	int str_len;

	if(argc != 3)
	{
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);	// 서버 접속을 위한 소켓 생성
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));	// 구조체 변수 serv_addr에 IP와 PORT정보 초기화
	serv_addr.sin_family = AF_INET;	// 서버의 주소체계는 IPv4
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);	// 프로그램 실행시 전달하는 IP 주소값 전달
	serv_addr.sin_port = htons(atoi(argv[2]));	// 프로그램 실행시 전달하는 포트번호

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1 )	// connect 함수를 통해 서버로 연결요청 전달
		error_handling("connect() error!");

	str_len = read(sock, message, sizeof(message)-1);	// 연결 요청 성공 후 데이터를 수신받음
	if(str_len == -1)
		error_handling("read() error!");

	printf("Message from server : %s \n", message);
	close(sock);	// 소켓을 닫으면서 서버와의 연결 종료
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
