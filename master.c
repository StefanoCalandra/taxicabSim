#include "master.h"

void ;
int ;

int main(int argc, char **argv) 
{
  int shmid;
  key_t shmkey, qkey;
  Message msg;
}

void logmsg(char *message, enum Level l) {
  if (l <= DEBUG) {
    printf("[taxi-%d] %s\n", getpid(), message);
  }
}

void SIGINThandler(int sig) {
  logmsg("Finishing up", DB);
  shmdt(mapptr);
  logmsg("Graceful exit successful", DB);
  exit(0);
}
