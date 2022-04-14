#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "Repository.h"
#include "Output.h"
#include "Main.h"
#include "Socket.h"

int main(int argc, char *argv[])
{
  Repository_init();
  Output_all();

  int sockfd = Socket_create();

  while (true)
  {
    Socket_listening(sockfd, MAXTIMEOUT_us);
    Socket_send(sockfd);
  }

  Repository_free();

  return EXIT_SUCCESS;
}