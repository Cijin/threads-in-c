#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_THREADS 10

void *thread_start(void *arg) { return NULL; }

int main(int argc, char *argv[]) {
  pthread_attr_t thread_attr;
  pthread_t thread;
  cpu_set_t cpus;

  pthread_attr_init(&thread_attr);
  CPU_ZERO(&cpus);
  CPU_SET(0, &cpus); // thread only runs on CPU 0

  pthread_attr_setaffinity_np(&thread_attr, sizeof(cpu_set_t), &cpus);

  if (pthread_create(&thread, &thread_attr, thread_start, NULL) != 0) {
    printf("failed to create thread\n");
    return 1;
  }

  if (pthread_join(thread, NULL) != 0) {
    printf("unable to join thread\n");
  };

  pthread_attr_destroy(&thread_attr);

  return 0;
}
