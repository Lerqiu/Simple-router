#ifndef MY_ROUTING
#define MY_ROUTING

void Routing_receive(int sockfd);
void Routing_removeOld();
void Routing_age();

void Routing_PrepareToRemove(Record *record);
void Routing_mergeRecord(Repository *repo, Record *record);
void Routing_mergeSource(Repository *repo, Record *record);

#endif