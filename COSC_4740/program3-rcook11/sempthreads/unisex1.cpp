#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "semp.h"
#include <time.h>
using namespace std;


Semaphore line, bathroom,multex;

//count, so we can number the processes
int femalen =1, malen=1;

//count for number in bathroom
int fc=0, mc=0, wc=0;  //count for num in bathroom.

/*
* instead of passing the id as the parameter, this just uses a global variable
* and a multex
*/

 
void *male(void * noparam) {
  int i,eid,t;
  bool canuse = false;

  //geting an id number
  semaphore_down( &multex );
  eid = malen;
  malen++ ;
  semaphore_up( &multex );
  printf("male %d:  Starting \n",eid);

  while (1==1)  {

   //work for a while
    t = (int) rand() % 10; 
    printf("male %d:  is working %d seconds\n",eid,t);
    sleep (t);

    printf("male %d: needs to use the bathrooms\n",eid);
    semaphore_down (&bathroom); 
    if (fc == 0 && mc==0) {  //bathroom is empty, so no waiting
      mc++; 
      semaphore_up(&bathroom);  
    } else if (mc>0) { //already males in the bathroom, so no waiting
      mc++; 
      semaphore_up(&bathroom);  
    } else {
      wc++;
      semaphore_up(&bathroom);
      printf("male %d: is line  \n",eid);
      semaphore_down(&line);//the line of people waiting
       
      //finally can use the bathroom, mark the count up.
      // note must do this outside of the bathroom, CR or deadlock will happen
      semaphore_down (&bathroom); 
         mc++;
      semaphore_up(&bathroom);
    }


    //use bathroom
    t = (int) rand() % 10; 
    printf("male %d:  is using the bathroom for %d seconds\n",eid,t);
    sleep (t);

    semaphore_down(&bathroom);
      mc--;
      if (mc == 0) {
        printf("male %d:  signaling the line to start\n",eid);
        for (i=0; i<wc; i++) 
          semaphore_up(&line);
        wc =0;
      }
    semaphore_up(&bathroom);
    printf("male %d:  is done using the bathroom\n",eid);
  } 
}

void * female(void * noparam) {
  int i,t,eid;
  semaphore_down( &multex );
  eid = femalen;
  femalen++ ;
  semaphore_up( &multex );
  printf("female %d:  Starting \n",eid);

  //initial set to infinite for testing
  while (1==1) {

    t = (int) rand() % 10;
    printf("female %d: is working for %d seconds\n",eid,t);
    sleep (t);

    printf("female %d: needs to use the bathrooms\n",eid,t);
    semaphore_down (&bathroom); 
    if (fc==0 &&mc == 0) {  //empty
      fc++;
      semaphore_up(&bathroom);
    } else if (fc>0) { //already feamles, so no waiting.
      fc++;
      semaphore_up(&bathroom);
    }else {
      wc++;
      semaphore_up(&bathroom);
      printf("female %d: is line  \n",eid,t);
      semaphore_down(&line);//the line of people waiting

      //finally can use the bathroom, mark the count up.
      // note must do this outside of the bathroom, CR or deadlock will happen
      semaphore_down (&bathroom); 
         fc++;
      semaphore_up(&bathroom);
    }


    //use bathroom
    t = (int) rand() % 10; 
    printf("female %d:  is using the bathroom for %d seconds\n",eid,t);
    sleep (t);

    //done
    semaphore_down(&bathroom);
      fc--;
      if (fc == 0) {
        printf("female %d:  signaling the line to start\n",eid,t);
        for (i=0; i<wc; i++)
          semaphore_up(&line);
        wc =0;
      }
    semaphore_up(&bathroom);
    printf("female %d:  is done with the bathroom\n",eid,t);
  } 
}

main(int argc, char* argv[]) {
  int i;
  //defaults
  int num_m = 1; 
  int num_f = 1;

  if (argc ==3) {
    num_m = atoi( argv[1] );
    num_f = atoi( argv[2] );
  } 
  printf("m is %d, f is %d\n", num_m, num_f);

  pthread_t males[num_m];
  pthread_t females[num_f];


  srand((unsigned)time(0));

//init semaphore line to 0, since everyone must wait
  semaphore_init(&line,0);
//init semaphore bathroom to 1, so first person can go in
  semaphore_init(&bathroom,1);
//init semaphore multex to 1, so first person can entry critical section
  semaphore_init(&multex,1);

// create threads 
  for (i=0; i<num_m; i++) {
     if (pthread_create(&males[i], NULL, male, 0)) {
       perror("error creating the thread");
       exit(1);
     }
  }

  printf ("male threads created\n");

  for (i=0; i<num_f; i++) {
    if (pthread_create(&females[i], NULL, female, 0)) {
      perror("error creating the thread");
      exit(1);
    }
  }
  printf ("threads created\n");


//wait for threads to finish 
  for(i=0;i<num_m;i++) 
    pthread_join(males[i], 0);

  for(i=0;i<num_f;i++) 
  pthread_join(females[i], 0);

}
