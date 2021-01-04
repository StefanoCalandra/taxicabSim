#include "taxi.h"
#include "general.h"
#include <stdio.h>
#include <unistd.h>

void *mapptr;
Point position;
int *executing;

void SIGINThandler(int sig) { *executing = 0; }
void SIGINThandler(int sig) {
  *executing = 0;
  logmsg("Finishing up");
  shmdt(mapptr);
}

void incTrafficAt(Point p) {

  /*wait mutex*/
  ((Cell(*)[SO_WIDTH][SO_HEIGHT])mapptr)[p.x][p.y]->traffic++;
  logmsg("Incrementato traffico in ");
  printf("\t(%d,%d)\n", p.x, p.y);
  /*signal mutex*/
}

void moveTo(Point p) { /*pathfinding*/
  /* funzione nanosleep permette di aspettare per il tempo indicato da */
  usleep(5000000);
  logmsg("Moving to");
  printf("\t(%d,%d)\n", p.x, p.y);
  incTrafficAt(p);
}

int main(int argc, char **argv) {
@ -22,11 +34,15 @@ int main(int argc, char **argv) {
  key_t shmkey, qkey;
  Message msg;

  logmsg("Inizializzazione");
  sleep(1);

  /************INIT************/
  if ((shmkey = ftok("makefile", 'a')) < 0) {
    EXIT_ON_ERROR
  }

  if ((shmid = shmget(shmkey, sizeof(int), IPC_CREAT | 0644)) < 0) {
  if ((shmid = shmget(shmkey, sizeof(int), 0644)) < 0) {
    EXIT_ON_ERROR
  }

@ -39,7 +55,9 @@ int main(int argc, char **argv) {
    printf("ftok error\n");
    EXIT_ON_ERROR
  }
  shmid = shmget(shmkey, 0, 0644);
  if ((shmid = shmget(shmkey, 0, 0644)) < 0) {
    EXIT_ON_ERROR
  }
  if (shmid < 0) {
    printf("shmget error\n");
    EXIT_ON_ERROR
@ -57,18 +75,16 @@ int main(int argc, char **argv) {
  logmsg("Init position");
  sscanf(argv[1], "%d", &position.x);
  sscanf(argv[2], "%d", &position.y);
  /************END-INIT************/

  incTrafficAt(position);
  sleep(3);

  while (*executing) {
    msgrcv(qid, &msg, sizeof(Point), 0, 0);
    logmsg("Going to Source");
    moveTo(msg.source);
    logmsg("Going to Nearest Source");
    /* moveTo(getNearSource()); */
    logmsg("Going to destination");
    moveTo(msg.destination);
  }

  logmsg("Finishing up");
  shmdt(mapptr);
  exit(0);
}
