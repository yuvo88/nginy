#include "server.h"
#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define MY_SOCK_ADDRESS "127.0.0.1"
#define MY_SOCK_PORT 5432
#define SERVER_RESPONSE                                                        \
  "HTTP/1.0 200 OK\nServer: nginy\nContent-type: text/html; "                  \
  "charset=utf-8\nContent-Length: 0\n"
#define MAX_METHOD_LEN 10
#define RECEIEVE_BUFFER_LENGTH 1024

HttpRequestLine parseRequestLine(char *requestLineString);
HttpRequest parseHttpRequest(char *receiveBuffer);
HttpHeader parseHttpHeader(char *httpHeaderString);
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

void handleConnection(int serverFileDescriptor, void *handlerFunction) {
  socklen_t addressSize;
  struct sockaddr_in remoteAddress;
  char receiveBuffer[RECEIEVE_BUFFER_LENGTH] = {};

  addressSize = sizeof(remoteAddress);
  int connectionFileDescriptor = accept(
      serverFileDescriptor, (struct sockaddr *)&remoteAddress, &addressSize);
  assert(connectionFileDescriptor != -1);

  recv(connectionFileDescriptor, receiveBuffer, RECEIEVE_BUFFER_LENGTH, 0);
  HttpRequest parsedRequest = parseHttpRequest(receiveBuffer);
  char *(*callableHandlerFunction)(HttpRequest httpRequest) = handlerFunction;
  char *returnValue = callableHandlerFunction(parsedRequest);
  send(connectionFileDescriptor, returnValue, strlen(returnValue), 0);
  closeFileDescriptor(connectionFileDescriptor);
}

void closeFileDescriptor(int fileDescriptor) {
  assert(close(fileDescriptor) != -1);
}

HttpRequest parseHttpRequest(char *receiveBuffer) {
  HttpRequest httpRequest;
  char *splitBuffer = strndup(receiveBuffer, RECEIEVE_BUFFER_LENGTH);
  char *runner = splitBuffer;
  char *header;
  char *requestLineString = strsep(&runner, "\n");

  httpRequest.requestLine = parseRequestLine(requestLineString);
  header = strsep(&runner, "\n");

  for (int i = 0; header[0] != '\r' && i < HEADER_ARRAY_LENGTH; i++) {
    HttpHeader httpHeader = parseHttpHeader(header);
    httpRequest.headers.headers[i] = httpHeader;
    httpRequest.headers.length = i + 1;
    header = strsep(&runner, "\n");
  }

  free(splitBuffer);
  return httpRequest;
}

HttpRequestLine parseRequestLine(char *requestLineString) {
  HttpRequestLine httpRequestLine;
  char *splitBuffer = strndup(requestLineString, RECEIEVE_BUFFER_LENGTH);
  char *runner = splitBuffer;
  char *method = strsep(&runner, " ");

  if (strncmp(method, "GET", MAX_METHOD_LEN) != 0) {
    httpRequestLine.method = UNKNOWN;
  } else {
    httpRequestLine.method = GET;
  }

  char *uri = strsep(&runner, " ");
  strncpy(httpRequestLine.uriPath, uri, URI_LENGTH);

  free(splitBuffer);
  return httpRequestLine;
}

HttpHeader parseHttpHeader(char *httpHeaderString) {
  HttpHeader httpHeader;
  char *splitBuffer = strndup(httpHeaderString, RECEIEVE_BUFFER_LENGTH);
  char *runner = splitBuffer;
  char *headerKey = strsep(&runner, ":");

  strncpy(httpHeader.headerKey, headerKey, HEADER_KEY_SIZE);
  char *headerValue = strsep(&runner, ":");
  strncpy(httpHeader.headerValue, headerValue + 1, HEADER_VALUE_SIZE);

  return httpHeader;
}

HttpUri parseUri(char *uri) {
  HttpUri httpUri;
  char *splitBuffer = strndup(uri, RECEIEVE_BUFFER_LENGTH);
  char *runner = splitBuffer;
  char *path = strsep(&runner, "?");
  strncpy(httpUri.path, path, URI_LENGTH);
  char *query = strsep(&runner, "?");
}
