#ifndef MY_ROUTING
#define MY_ROUTING

void Routing_receive(int sockfd);
void Routing_removeOld();
void Routing_age();
void Routing_notifyAvailable(Record * record);

#endif