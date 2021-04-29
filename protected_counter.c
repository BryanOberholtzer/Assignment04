/* remember to compile with -lrt -lpthread */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <strings.h>

#include "protected_counter.h"

struct protected_counter *make_shm(char *shm_name);

struct protected_counter *make_counter(char *shm_name) {
  struct protected_counter *counter;

  if ((counter = make_shm(shm_name)) == NULL) {
    fprintf(stderr, "error creating shared int. quitting ...\n");
    exit(EXIT_FAILURE);
  }

  bzero(counter, sizeof(struct protected_counter));
  strncpy(counter -> shm_name, shm_name, MAX_SHM_NAME_LEN);
  if ((sem_init(&(counter -> lock), 1, 1)) == -1) {
    fprintf(stderr, "sem_init failure. quitting ...\n");
    exit(EXIT_FAILURE);
  }
  return counter;
}

void inc(struct protected_counter *c) {
  if (sem_wait(&(c->lock)) == -1) {
    fprintf(stderr, "sem_wait error\n");
    exit(EXIT_FAILURE);
  }
  (c -> x)++;
  if (sem_post(&(c->lock)) == -1) {
    fprintf(stderr, "sem_wait error\n");
    exit(EXIT_FAILURE);
  }
}

void set(struct protected_counter *c, int x) {
  if (sem_wait(&(c->lock)) == -1) {
    fprintf(stderr, "sem_wait error\n");
    exit(EXIT_FAILURE);
  }
  c -> x = x;
  if (sem_post(&(c->lock)) == -1) {
    fprintf(stderr, "sem_wait error\n");
    exit(EXIT_FAILURE);
  }
}

void dec(struct protected_counter *c) {
  if (sem_wait(&(c->lock)) == -1) {
    fprintf(stderr, "sem_wait error\n");
    exit(EXIT_FAILURE);
  }
  (c -> x)--;
  if (sem_post(&(c->lock)) == -1) {
    fprintf(stderr, "sem_wait error\n");
    exit(EXIT_FAILURE);
  }
}

int get_value(struct protected_counter *c) {
  int val;
  
  if (sem_wait(&(c->lock)) == -1) {
    fprintf(stderr, "sem_wait error\n");
    exit(EXIT_FAILURE);
  }
  val = (c -> x);
  if (sem_post(&(c->lock)) == -1) {
    fprintf(stderr, "sem_wait error\n");
    exit(EXIT_FAILURE);
  }
  return val;
}

void cleanup(struct protected_counter *c) {
  char name[MAX_SHM_NAME_LEN];
  strncpy(name, c -> shm_name, MAX_SHM_NAME_LEN);

  if (sem_destroy(&(c -> lock)) == -1) {
    fprintf(stderr, "sem_destroy error\n");
  }
  
  if (shm_unlink(name) == -1) {
    fprintf(stderr, "shm_unlink error\n");
    exit(EXIT_FAILURE);
  }
}

/* creates a shared memory region large enough to
 * hold a struct protected counter */
struct protected_counter *make_shm(char *shm_name) {
  int fd;
  struct protected_counter *counter;
  
  if ((fd = shm_open(shm_name, O_CREAT | O_RDWR, 0600)) == -1) {
    fprintf(stderr, "error opening shared memory\n");
    exit(EXIT_FAILURE);
  }

  if (ftruncate(fd, sizeof(struct protected_counter)) == -1) {
    fprintf(stderr, "error opening shared memory\n");
    exit(EXIT_FAILURE);
  }

  if ((counter = (struct protected_counter*)mmap(NULL, sizeof(struct protected_counter),
                                                 PROT_READ | PROT_WRITE,
                                                 MAP_SHARED, fd, 0)) == MAP_FAILED) {
    fprintf(stderr, "mmap error\n");
    exit(EXIT_FAILURE);
  }

  return counter;
}
