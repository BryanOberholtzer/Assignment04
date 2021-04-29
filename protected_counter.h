#include <semaphore.h>

#ifndef PROTECTED_COUNTER_H_
#define PROTECTED_COUNTER_H_

#define MAX_SHM_NAME_LEN 32

struct protected_counter {
  char shm_name[MAX_SHM_NAME_LEN];
  int x;
  sem_t lock;
};

struct protected_counter *make_counter(char*);
void inc(struct protected_counter*);
void dec(struct protected_counter*);
void set(struct protected_counter*, int);
int get_value(struct protected_counter*);
void cleanup(struct protected_counter*);

#endif

