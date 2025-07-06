#ifndef __server_h
#define __server_h
void handleConnection(int serverFileDescriptor);
int createServer(const char *bindIp, int bindPort);
void closeFileDescriptor(int fileDescriptor);
#endif
