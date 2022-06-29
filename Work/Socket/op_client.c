#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024 //피연산자의 바이트 수를 상수화
#define RLT_SIZE 4    //연산결과의 바이트 수를 상수화
#define OPSZ 4
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int sock;
	char opmsg[BUF_SIZE]; //데이터의 송수신을 위한 메모리 공간은 배열을 기반으로 생성
	int result, opnd_cnt, i;
	struct sockaddr_in serv_adr;
	if(argc!=3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1)
		error_handling("connect() error!");
	else
		puts("Connected.........");

	fputs("Operand count: ", stdout);
	scanf("%d", &opnd_cnt); //개수정보(opnd_cnt)를 입력받은 후
	opmsg[0]=(char)opnd_cnt; //이를 배열 opmsg에 저장
                            //여기서 중요한건 개수정보를 1바이트 정수형태로 전달한다.
                           //그래서 1바이트로 표현 불가능한 범위의 정수가 입력되면 안된다.
	for(i=0;i<opnd_cnt;i++) 
	{
		printf("Operand %d: ", i+1);
		scanf("%d", (int*)&opmsg[i*OPSZ+1]);
	} //정수를 입력받아서 배열 opmsg에 이어서 저장하고 있다.
	  //char형 배열에 4바이트 int형 정수를 저장해야 하기 때문에
	  //int형 포인터로 형변환을 하고있다.
	fgetc(stdin); //53행에서 문자를 입력받아야 하는데, 이에 앞서
	// 버퍼에 남아있는 \n 문자의 삭제를 위해 fgetc 함수가 호출되고 있다
	fputs("Operator: ", stdout);
	scanf("%c", &opmsg[opnd_cnt*OPSZ+1]); //연산자 정보를 입력받아서
	                                      // 배열에 저장
	write(sock, opmsg, opnd_cnt*OPSZ+2); //write 함수호출을 통해 opmsg에
	//저장되어 있는 연산과 관련된 정보를 한방에 전송하고 있다.
	//한번에 묶어서 보내고 되고, 여러번 나눠서 write 함수호출을 통해 나눠서 보내도 됨.
	read(sock, &result, RLT_SIZE); //서버가 전송해주는 연산결과의 저장과정.
	//수신할 데이터의 크기가 4바이트이기 때문에 이렇게 한번의 read 함수 
	//호출로 충분히 수신 가능하다.

	printf("Operation result: %d \n", result);
	close(sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
