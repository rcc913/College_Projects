#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "semp.h"
#include <time.h>
using namespace std;

int max_num = 20;
Semaphore full,empty,multex;
int buffer[10], bufferc = 0;
 
void *produc(void * mid) {
  int i,eid,id,t;

  //geting an id number
  id =  *((int *)mid);

  printf("producer %d:  Starting \n",id);

// the producer will produce 10 items for this one, otherwise, we could just go infinite.

  for (int x=0; x<10;x++)  {

    t = (int) rand() % 10; 
    printf("Producer %d:  producing for %d seconds\n",id,t);
    sleep (t);

    //don't add to buffer if full    
    
    semaphore_down( &empty );
 
    //criticial section
    semaphore_down( &multex );
      buffer[bufferc] = id; 
      bufferc++;
    semaphore_up( &multex );

    printf("Producer %d:  has placed an item number %d in the buffer\n",id,x);
    semaphore_up( &full );
  } 
}

void *consum(void * mid) {
  int i,t,eid,id,item;

  //geting an id number
  id =  *((int *) mid);

  printf("Consumer %d:  Starting \n",id);

  //initial set to infinite for testing
  for ( ;1<2;) {

    printf("Consumer %d: is ready to consume an item\n",id);
    semaphore_down( &full );

    //enter critical section
    semaphore_down( &multex );
      bufferc--;
      item = buffer[bufferc]; 
    semaphore_up( &multex );

    printf("consumer %d:  has a new item %d\n",id,item);
    semaphore_up( &empty);
    t = (int) rand() % 30;
    printf("Consumer %d: consuming for  %d seconds\n",id,t);
    sleep (t);
  } 
}

main(int argc, char* argv[]) {
  int i;

int consumern =1, producern=1;

  int num_p = 1; 
  int num_c=1;
  if (argc ==3) {
    num_p = atoi( argv[1] );
    num_c = atoi( argv[2] );
  } 
  printf("p is %d, c is %d\n", num_p, num_c);

  pthread_t cons[num_c];
  pthread_t prod[num_p];


  srand((unsigned)time(0));

//init semaphore full to 0 since nothing in queue yet
  semaphore_init(&full,0);
//init semaphore empty to 10, since we can produce 10 items.
  semaphore_init(&empty,10);
//init semaphore multex to 1, so first person can entry critical section
  semaphore_init(&multex,1);

// create threads 
  for (i=0; i<num_c; i++) {
     if (pthread_create(&cons[i], NULL, consum, &consumern)) {
       perror("error creating the thread");
       exit(1);
     }
     sleep(1); //the thread creation is so fast, the all param are the last one... duh!
     ++consumern;
  }
  printf ("consmer threads created\n");

  for (i=0; i<num_p; i++) {
    if (pthread_create(&prod[i], NULL, produc, &producern)) {
      perror("error creating the thread");
      exit(1);
    }
   sleep(1); //the thread creation is so fast, the all param are the last one... duh!
   producern++;
  }
  printf ("threads created\n");


//wait for threads to finish 
  for(i=0;i<num_c;i++) 
    pthread_join(cons[i], 0);

  for(i=0;i<num_p;i++) 
  pthread_join(prod[i], 0);

}
