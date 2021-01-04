@ -1,10 +1,29 @@
#include "generator.h"
#include "general.h"
#include <stdio.h>
#include <unistd.h>

int *executing;
Point Sources[MAX_SOURCES];

void logmsg(char *message) {
  int pid;
  pid = getpid();
  printf("[generator-%d] %s\n", pid, message);
}

/*  Signal Handlers  */
void SIGINThandler(int sig) {
  printf("=============== Received SIGINT ==============\n");
  *executing = 0;
}

void ALARMhandler(int sig) {
  signal(SIGINT, SIGINThandler);
  printf("=============== Received ALARM ===============\n");
  kill(0, SIGINT);
}

/*
 * Parses the file taxicab.conf in the source directory and populates the Config
 * struct
@ -59,8 +78,9 @@ int checkNoAdiacentHoles(Cell (*matrix)[SO_WIDTH][SO_HEIGHT], int x, int y) {
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      if ((x + i - 1) >= 0 && (x + i - 1) <= SO_WIDTH && (y + j - 1) >= 0 &&
          (y + j - 1) <= SO_HEIGHT && !(i == 1 && j == 1)) {
        b = b && (matrix[x + i - 1][y + j - 1]->state != HOLE);
          (y + j - 1) <= SO_HEIGHT &&
          matrix[x + i - 1][y + j - 1]->state == HOLE) {
        b = 1;
      }
    }
  }
@ -95,15 +115,14 @@ void generateMap(Cell (*matrix)[SO_WIDTH][SO_HEIGHT], Config *conf) {
      i++;
    }
  }
  for (i = 0; i > conf->SO_SOURCES; i++) {
  for (i = 0; i < conf->SO_SOURCES; i++) {
    x = rand() % SO_WIDTH;
    y = rand() % SO_HEIGHT;

    if (matrix[x][y]->state != HOLE && matrix[x][y]->state != SOURCE) {
    if (matrix[x][y]->state == FREE) {
      matrix[x][y]->state = SOURCE;
      p.x = x;
      p.y = y;
      Sources[i] = p;
      Sources[i].x = x;
      Sources[i].y = y;
    } else {
      i--;
    }
@ -128,12 +147,29 @@ int isFree(Cell (*map)[SO_WIDTH][SO_HEIGHT], Point p) {
  return r;
}

/*  Signal Handlers  */
void SIGINThandler(int sig) { *executing = 0; }

void ALARMhandler(int sig) {
  signal(SIGINT, SIGINThandler);
  kill(0, SIGINT);
/*
 * Print on stdout the map in a readable format:
 *     FREE Cells are printed as   [ ]
 *     SOURCE Cells are printed as [S]
 *     HOLE Cells are printed as   [H]
 */
void printMap(Cell (*map)[SO_WIDTH][SO_HEIGHT]) {
  int x, y;
  for (y = 0; y < SO_HEIGHT; y++) {
    for (x = 0; x < SO_WIDTH; x++) {
      switch (map[x][y]->state) {
      case FREE:
        printf("[ ]");
        break;
      case SOURCE:
        printf("[S]");
        break;
      case HOLE:
        printf("[X]");
      }
    }
    printf("\n");
  }
}

int main(int argc, char **argv) {
@ -143,10 +179,11 @@ int main(int argc, char **argv) {
  int found = 0;
  key_t shmkey, qkey;
  void *mapptr;
  Message msg;
  char xArgBuffer[20], yArgBuffer[20];
  char *args[4];
  char *envp[1];

  /************ INIT ************/
  if ((shmkey = ftok("makefile", 'a')) < 0) {
    EXIT_ON_ERROR
  }
@ -158,8 +195,6 @@ int main(int argc, char **argv) {
  if ((executing = shmat(shmid, NULL, 0)) < (int *)0) {
    EXIT_ON_ERROR
  }
  *executing = 1;
  /* Genero chiave unica per tutti i processi */
  if ((shmkey = ftok("makefile", 'd')) < 0) {
    EXIT_ON_ERROR
  }
@ -179,14 +214,20 @@ int main(int argc, char **argv) {
  if ((qid = msgget(qkey, IPC_CREAT | 0644)) < 0) {
    EXIT_ON_ERROR
  }

  *executing = 1;
  parseConf(&conf);
  generateMap(mapptr, &conf);
  printMap(mapptr);

  signal(SIGINT, SIGINThandler);
  signal(SIGALRM, ALARMhandler);
  /************ END-INIT ************/

  printMap(mapptr);
  sleep(1);

  logmsg("Creo taxi:");
  for (i = 0; i < conf.SO_TAXI; i++) {
    printf("\t%d\n", i);
    sleep(1);
    switch (fork()) {
    case -1:
      EXIT_ON_ERROR
@ -206,27 +247,26 @@ int main(int argc, char **argv) {
    }
  }

  logmsg("Creo processi sorgenti");

  logmsg("Creo processi sorgenti:");
  usleep(2000000);
  for (i = 0; i < conf.SO_SOURCES; i++) {
    printf("\t%d\n", i);
    switch (fork()) {
    case -1:
      EXIT_ON_ERROR
    case 0:
      printf("[Source-%d] Initialization\n", getpid());
      msg.type = getpid();
      msg.source = Sources[i];
      while (*executing) {
        while (!found) {
          msg.destination.x = (rand() % SO_WIDTH);
          msg.destination.y = (rand() % SO_HEIGHT);
          if (isFree(mapptr, msg.destination)) {
            found = 1;
          }
        }
        msgsnd(qid, &msg, sizeof(Point), 0);
      }
      exit(0);
      xArg = (rand() % SO_WIDTH);
      yArg = (rand() % SO_HEIGHT);
      snprintf(xArgBuffer, 20, "%d", xArg);
      snprintf(yArgBuffer, 20, "%d", yArg);
      args[0] = "source";
      args[1] = xArgBuffer;
      args[2] = yArgBuffer;
      args[3] = NULL;
      envp[0] = NULL;
      execve("source", args, envp);
      /* here execv failed */
      EXIT_ON_ERROR
    }
  }
