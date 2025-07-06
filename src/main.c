#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define MY_SOCK_ADDRESS "127.0.0.1"
#define MY_SOCK_PORT 5432
#define SERVER_RESPONSE                                                        \
  "HTTP/1.0 200 OK\nServer: nginy\nContent-type: text/html; "                  \
  "charset=utf-8\nContent-Length: 0\n"

void handleConnection(int connectionFileDescriptor);

int main() {
  int socketFileDescriptor, connectionFileDescriptor;
  socklen_t addressSize;
  struct sockaddr_in bindAddress, remoteAddress;

  socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  assert(socketFileDescriptor != -1);

  memset(&bindAddress, 0, sizeof(bindAddress));
  bindAddress.sin_family = AF_INET;
  bindAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  bindAddress.sin_port = htons(MY_SOCK_PORT);

  assert(bind(socketFileDescriptor, (struct sockaddr *)&bindAddress,
              sizeof(bindAddress)) != -1);

  assert(listen(socketFileDescriptor, 5) != -1);
  printf("server listening\n");

  /* Now we can accept incoming connections one
     at a time using accept(2). */

  addressSize = sizeof(remoteAddress);
  connectionFileDescriptor = accept(
      socketFileDescriptor, (struct sockaddr *)&remoteAddress, &addressSize);
  assert(connectionFileDescriptor != 1);

  handleConnection(connectionFileDescriptor);

  assert(close(socketFileDescriptor) != -1);

  return 0;
}

void handleConnection(int connectionFileDescriptor) {
  char receiveBuffer[1024] = {};
  recv(connectionFileDescriptor, receiveBuffer, 1024, 0);
  printf("%s\n", receiveBuffer);
  send(connectionFileDescriptor, SERVER_RESPONSE, strlen(SERVER_RESPONSE), 0);
}
