#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int tcp_sock, udp_sock;
	int sock_type;
	socklen_t optlen;
	int state;

	optlen=sizeof(sock_type);
	tcp_sock=socket(PF_INET, SOCK_STREAM, 0); //TCP 소켓생성
	udp_sock=socket(PF_INET, SOCK_DGRAM, 0);  //UDP 소켓생성
	printf("SOCK_STREAM: %d \n", SOCK_STREAM); //소켓 생성시 인자로 전달하는  상수값 출력
	printf("SOCK_DGRAM: %d \n", SOCK_DGRAM); //소켓 생성시 인자로 전달하는 상수값 출력

	//소켓의 타입 정보 TCP소켓은 SOCK_STREAM의 상수 값 1을 얻게 될것
	state=getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if(state)
		error_handling("getsockopt() error!");
	printf("Socket type one: %d \n", sock_type);

	//UDP 소켓은 SOCK_DGRAM의 상수 값 2를 얻게 될것
	state=getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
	if(state)
		error_handling("getsockopt() error!");
	printf("Socket type two: %d \n", sock_type);

	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
