#include <sys/wait.h>
#include <time.h> /* rand */
#include <unistd.h>
#define SO_WIDTH 4 /* a tempo di compilazione */
#define SO_HEIGHT 4
#define SO_WIDTH 8 /* a tempo di compilazione */
#define SO_HEIGHT 8
#define MAX_SOURCES (SO_WIDTH * SO_HEIGHT / 3)
#define EXIT_ON_ERROR                                                          \
  if (errno) {                                                                 \
@ -43,33 +43,7 @@ typedef struct {

typedef struct {
  long type;
  Point source;
  Point destination;
} Message;

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

#endif /* __GENERAL_H_ */
