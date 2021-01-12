#ifndef __MASTER_H_
#define __MASTER_H_
#include <errno.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> /* rand */
#include <unistd.h>

#ifndef DEBUG    /* DEBUG by setting when compiling -DDEBUG=1 */
#define DEBUG 0
#endif

#define SO_WIDTH 8 /* a tempo di compilazione */
#define SO_HEIGHT 8
#define MAX_SOURCES (SO_WIDTH * SO_HEIGHT / 3)
#define EXIT_ON_ERROR                                                          \
  if (errno) {                                                                 \
    fprintf(stderr, "%d: pid %ld; errno: %d (%s)\n", __LINE__, (long)getpid(), \
            errno, strerror(errno));                                           \
    exit(EXIT_FAILURE);                                                        \
  }

enum type { FREE, SOURCE, HOLE };

enum Level { RUNTIME, DB };

typedef struct {
  int ;
  int ;
  int ;
} Viaggi;

typedef struct {
  int SO_SOURCES, SO_TOP_CELLS;
} Mappa;

typedef struct {
  ;
} Taxi;

#endif /* __MASTER_H_ */
