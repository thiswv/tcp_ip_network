#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_handling(char *message) 
{
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}

int main(int argc, char *argv[])
{
  int serv_sock;
  int clnt_sock;

  struct sockaddr_in serv_addr;
  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_size;

  char message[] = "hello world!";

  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);  //输入有问题
    exit(1);
  }

  //socket 创建套接字
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (serv_sock == -1) {
    error_handling("socket() error");
  }

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  //bind 分配IP地址和端口号
  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    error_handling("bind() error");
  }

  //listen 将套接字转为可连接
  if (listen(serv_sock, 5) == -1) {
    error_handling("listen() error");
  }

  clnt_addr_size = sizeof(clnt_addr);

  //accept 受理连接请求，如果没有连接请求下调用该函数，则不会返回
  //直到有连接请求为止
  clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
  if (clnt_sock == -1) {
    error_handling("accept() error");
  }

  write(clnt_sock, message, sizeof(message));
  close(clnt_sock);
  close(serv_sock);
  return 0;
}

