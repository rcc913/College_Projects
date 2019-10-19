#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "semp.h"
//#include <time.h>
using namespace std;


Semaphore full,empty,multex;
int buffer[10], bufferc = 0;
 
void * prod(void *mid) {
  int i,eid,t;
  //get the id number
  eid=*((int*)mid);

  printf("producer %d:  Starting \n",eid);

// the producer will produce 10 items for this one, otherwise, we could just go infinite.

  for (int x=0; x<10;x++)  {

    t = (int) rand() % 10; 
    printf("Producer %d:  producing for %d seconds\n",eid,t);
    sleep (t);

    //don't add to buffer if full    
    semaphore_down( &empty );
 
    //criticial section
    semaphore_down( &multex );
      buffer[bufferc] = eid; 
      bufferc++;
    semaphore_up( &multex );

    printf("Producer %d:  has placed an item %d in the buffer\n",eid,x);
    semaphore_up( &full );
  } 
}

void * consum(void *mid) {
  int i,t,eid,item;
  eid = *((int *) mid);
  printf("Consumer %d:  Starting \n",eid);

  //initial set to infinite for testing
  for ( ;1<2;) {

    printf("Consumer %d:  is ready to consume an item\n",eid);
    semaphore_down( &full );

    //enter critical section
    semaphore_down( &multex );
      bufferc--;
      item = buffer[bufferc]; 
    semaphore_up( &multex );

    printf("consumer %d:  has a new item %d\n",eid,item);
    semaphore_up( &empty);
    t = (int) rand() % 30;
    printf("Consumer %d: consuming for  %d seconds\n",eid,t);
    sleep (t);
  } 
}

main()
{
  int i;
  pthread_t con1;
  pthread_t prod1;

  int consumern =1, producern=1;

  srand((unsigned)time(0));


//init semaphore full to 0 since nothing in queue yet
  semaphore_init(&full,0);
//init semaphore empty to 10, since we can produce 10 items.
  semaphore_init(&empty,10);
//init semaphore multex to 1, so first person can entry critical section
  semaphore_init(&multex,1);

// create threads 
  if (pthread_create(&con1, 0, consum, &consumern)) {
    perror("error creating the thread");
    exit(1);
  }
  if (pthread_create(&prod1, 0, prod, &producern)) {
    perror("error creating the thread");
    exit(1);
  }

//wait for threads to finish
  pthread_join(con1, 0);
  pthread_join(prod1, 0);


}
