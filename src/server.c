#include "server.h"
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

int createServer(const char *bindIp, int bindPort) {
  int serverFileDescriptor, connectionFileDescriptor;
  struct sockaddr_in bindAddress;

  serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  assert(serverFileDescriptor != -1);

  memset(&bindAddress, 0, sizeof(bindAddress));
  bindAddress.sin_family = AF_INET;
  assert(inet_aton(bindIp, &bindAddress.sin_addr) != 0);
  bindAddress.sin_port = htons(MY_SOCK_PORT);

  assert(bind(serverFileDescriptor, (struct sockaddr *)&bindAddress,
              sizeof(bindAddress)) != -1);

  assert(listen(serverFileDescriptor, 5) != -1);
  return serverFileDescriptor;
}

void handleConnection(int serverFileDescriptor) {
  socklen_t addressSize;
  struct sockaddr_in remoteAddress;
  addressSize = sizeof(remoteAddress);
  int connectionFileDescriptor = accept(
      serverFileDescriptor, (struct sockaddr *)&remoteAddress, &addressSize);
  assert(connectionFileDescriptor != -1);
  char receiveBuffer[1024] = {};
  recv(connectionFileDescriptor, receiveBuffer, 1024, 0);
  printf("%s\n", receiveBuffer);
  send(connectionFileDescriptor, SERVER_RESPONSE, strlen(SERVER_RESPONSE), 0);
  closeFileDescriptor(connectionFileDescriptor);
}

void closeFileDescriptor(int fileDescriptor) {
  assert(close(fileDescriptor) != -1);
}
