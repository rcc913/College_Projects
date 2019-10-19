#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <unistd.h>
#include "semp.h"

using namespace std;

/*
 * we want the odd and even to alternate, so use two semphores
 */

Semaphore m;
Semaphore n;

void *odd(void *mid) {
  int i;
  int *id = (int *) mid;
  printf ("my id is %d\n",*id);

  for (i =0; i<10; i++) {
    semaphore_down( &n );
    printf("odd \n");
   semaphore_up( &m );
  }

}

void *even(void *mid) {
  int i;
  int *id = (int *) mid;
  printf ("my id is %d\n",*id);

  for (i =0; i<10; i++) {
    semaphore_down( &m );
    printf("even \n");
    semaphore_up( &n );
  } 
}

main()
{
  int i=1;
  pthread_t th1, th2;

//init semaphore m to 0  m will always go second
  semaphore_init(&m,0);
//init semaphore n to 1  n will always go first
  semaphore_init(&n,1);


// create thread 1
  if (pthread_create(&th1, NULL, odd, &i)) {
    perror("error creating the first thread");
    exit(1);
  }
  printf("created the first thread\n");
  sleep(1);
  i++;
// create thread 2
  if (pthread_create(&th2, 0, even, &i)) {
    perror("error creating the second thread");
    exit(1);
  }
  printf("created the second thread\n");

//wait for threads to finish
  pthread_join(th1, 0);
  pthread_join(th2, 0);
}
