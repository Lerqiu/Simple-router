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
  fprintf(stderr, "Start\n");
  Repository_Init();
  fprintf(stderr, "Repository init\n");
  Output_all();
  fprintf(stderr, "Start phase end\n");

  int sockfd = Socket_create();
  fprintf(stderr, "Socket created\n");

  while (true)
  {
    fprintf(stderr, "Pętla\n");
    Socket_listening(sockfd, MAXTIMEOUT);
    Socket_send(sockfd);
    Routing_removeOld();
    Routing_age();
  }

  Repository_Free();

  return EXIT_SUCCESS;
}