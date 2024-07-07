#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_THREADS 10

#define handle_error_en(en, msg)                                               \
  do {                                                                         \
    errno = en;                                                                \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

void display_attr(pthread_attr_t *attr, char *prefix) {
  int s, i;
  size_t v;
  void *stkaddr;
  struct sched_param sp;

  s = pthread_attr_getdetachstate(attr, &i);
  if (s != 0)
    handle_error_en(s, "pthread_attr_getdetachstate");
  printf("%sDetach state        = %s\n", prefix,
         (i == PTHREAD_CREATE_DETACHED)   ? "PTHREAD_CREATE_DETACHED"
         : (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE"
                                          : "???");

  s = pthread_attr_getscope(attr, &i);
  if (s != 0)
    handle_error_en(s, "pthread_attr_getscope");
  printf("%sScope               = %s\n", prefix,
         (i == PTHREAD_SCOPE_SYSTEM)    ? "PTHREAD_SCOPE_SYSTEM"
         : (i == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS"
                                        : "???");

  s = pthread_attr_getinheritsched(attr, &i);
  if (s != 0)
    handle_error_en(s, "pthread_attr_getinheritsched");
  printf("%sInherit scheduler   = %s\n", prefix,
         (i == PTHREAD_INHERIT_SCHED)    ? "PTHREAD_INHERIT_SCHED"
         : (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED"
                                         : "???");

  s = pthread_attr_getschedpolicy(attr, &i);
  if (s != 0)
    handle_error_en(s, "pthread_attr_getschedpolicy");
  printf("%sScheduling policy   = %s\n", prefix,
         (i == SCHED_OTHER)  ? "SCHED_OTHER"
         : (i == SCHED_FIFO) ? "SCHED_FIFO"
         : (i == SCHED_RR)   ? "SCHED_RR"
                             : "???");

  s = pthread_attr_getschedparam(attr, &sp);
  if (s != 0)
    handle_error_en(s, "pthread_attr_getschedparam");
  printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);

  s = pthread_attr_getguardsize(attr, &v);
  if (s != 0)
    handle_error_en(s, "pthread_attr_getguardsize");
  printf("%sGuard size          = %zu bytes\n", prefix, v);

  s = pthread_attr_getstack(attr, &stkaddr, &v);
  if (s != 0)
    handle_error_en(s, "pthread_attr_getstack");
  printf("%sStack address       = %p\n", prefix, stkaddr);
  printf("%sStack size          = %#zx bytes\n", prefix, v);
}

void *thread_start(void *arg) {
  pthread_attr_t attr;

  pthread_getattr_np(pthread_self(), &attr);

  printf("Thread attributes: \n");
  display_attr(&attr, "\t");
  return NULL;
}

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
