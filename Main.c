#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "Repository.h"
#include "Output.h"
#include "Main.h"
#include "Socket.h"
#include "Routing.h"

int main(int argc, char *argv[])
{
  Repository_Init();
  Output_all();

  int sockfd = Socket_create();
  int a = 0;
  while (true)
  {
    printf("Wpis: %d\n", a++);
    Socket_listening(sockfd, MAXTIMEOUT);
    Socket_send(sockfd);
    Routing_removeOld();
    Routing_age();
  }

  Repository_Free();

  return EXIT_SUCCESS;
}