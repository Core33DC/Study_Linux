#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>

#define BUF_SIZE 1024
#define IMG_BUF_SIZE 500000

void error_handling(char* message);

int main(int argc, char* argv[])
{
   int serv_sock, clnt_sock;
   struct sockaddr_in serv_adr, clnt_adr;
   int clnt_adr_size;
   char buf[BUF_SIZE];

   int fdimg;
   char img_buf[IMG_BUF_SIZE];

   char webpage[] = "HTTP/1.1 200 OK\r\n"
      "Server:Linux Web Server\r\n"
      "Content-Type: text/html; charset=UTF-8\r\n\r\n"
      "<!DOCTYPE html>\r\n"
      "<html><head><title> My Web Page </title>\r\n"
      "<style>body {background-color: #FFFFFF }</style></head>\r\n"
      "<body><center><h1><font size = ""7"">You know you want me</font></h1><br>\r\n"
      "<img src=\"tango.jpg\"></center></body></html>\r\n";

   if(argc !=2)
   {
      printf("Usage : %s <port> \n", argv[0]);
      exit(1);
   }

   serv_sock = socket(PF_INET, SOCK_STREAM, 0);
   memset(&serv_adr, 0, sizeof(serv_adr));
   serv_adr.sin_family = AF_INET;
   serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_adr.sin_port = htons(atoi(argv[1]));

   if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
      error_handling("bind() error");
   if(listen(serv_sock, 20) == -1)
      error_handling("listen() error");

   while(1)
   {
      clnt_adr_size = sizeof(clnt_adr);
      clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_size);
      puts("New client connection");

      read(clnt_sock, buf, sizeof(buf));
      //printf("%s \n", buf);

      if(!strncmp(buf, "GET / HTTP/1.1", 14))
      {
         puts("   Send Hypertext to client");
         write(clnt_sock, webpage, sizeof(webpage)-1);
      }
      else if(!strncmp(buf, "GET /tango.jpg", 14))
      {
         fdimg = open("tango.jpg", O_RDONLY);
         read(fdimg, img_buf, sizeof(img_buf));

         puts("   Send Image to client");
         write(clnt_sock, img_buf, sizeof(img_buf));
      }
      puts("closing");
      close(clnt_sock);
   }
   close(serv_sock);
   return 0;
}

void error_handling(char* message)
{
   fputs(message, stderr);
   fputc('\n', stderr);
   exit(1);
}
