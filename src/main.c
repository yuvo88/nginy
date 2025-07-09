#include "server.h"
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define MY_SOCK_ADDRESS "127.0.0.1"
#define MY_SOCK_PORT 5432
#define SERVER_RESPONSE                                                        \
  "HTTP/1.0 200 OK\nServer: nginy\nContent-type: text/html; "                  \
  "charset=utf-8\nContent-Length: 0\n"

const char *handlerFunction(HttpRequest httpRequest) {
  printf("%s\n", httpRequest.requestLine.uriPath);
  return SERVER_RESPONSE;
}

int main() {
  int serverFileDescriptor = createServer(MY_SOCK_ADDRESS, MY_SOCK_PORT);

  while (1) {
    handleConnection(serverFileDescriptor, handlerFunction);
  }
  closeFileDescriptor(serverFileDescriptor);

  return 0;
}
