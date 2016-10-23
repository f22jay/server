#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
// gcc -g -o simple_client simple_client.c
int main(int argc,char *argv[])
{
  char ip[20]={"10.128.144.17"};
  char data[20]={"good luck!\n"};
  int port=10086;
  int server_fd;
  int connect_num = atoi(argv[1]);
  int* connect_fd = (int*)malloc(connect_num * sizeof(int));
  struct sockaddr_in server_listen_addr;
  bzero(&server_listen_addr,sizeof(server_listen_addr));
  server_listen_addr.sin_family=AF_INET;
  server_listen_addr.sin_port=htons(port);
  inet_pton(AF_INET,"10.128.144.17",(void*)&server_listen_addr.sin_addr);
  int i = 0;
  for (i; i < connect_num; i++) {
    connect_fd[i]=socket(AF_INET,SOCK_STREAM,0);
    int ret=connect(connect_fd[i],(const struct sockaddr *)&server_listen_addr,
                    sizeof(struct sockaddr));
    if(ret<0)
    {
      perror("error: socket connect!");
      exit(1);
    }
  }
  i = 0;
  while (i++ < 100) {
    int j = 0;
    for (j; j < connect_num; j++) {
      int num=send(connect_fd[j],(void*)data,strlen(data),0);
      printf("send bytes[%d][%s]\n",num,data);
    }
    usleep(10000);
  }
  usleep(10000000);
  int j = 0;
  for (j; j < connect_num; j++) {
    close(connect_fd[j]);
  }
  return 1;
}
