#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

sem_t sem;

void *publisher() {
  printf("Doing some work...\n");
  sleep(2);
  printf("Data published!\n");
  sem_post(&sem);
  return NULL;
}

void *subscriber() {
  sem_wait(&sem);
  printf("Data recieved!\n");
  return NULL;
}

int main() {
  pthread_t pub, sub;
  sem_init(&sem, 0, 0);

  pthread_create(&pub, NULL, publisher, NULL);
  pthread_create(&sub, NULL, subscriber, NULL);

  pthread_join(pub, NULL);
  pthread_join(sub, NULL);

  sem_destroy(&sem);

  return 0;
}
