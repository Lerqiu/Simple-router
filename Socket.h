#ifndef MY_SOCKET
#define MY_SOCKET

int Socket_create();
void Socket_listening(int sockfd, unsigned long time);
void Socket_send(int sockfd);

#endif