//Ryan Cook
//Unisex Bathroom Problem

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "semp.h"
#include <time.h>
using namespace std;

Semaphore malesS,femalesS, bathroom,multex;

//count, so we can number the processes
int femalen =1, malen=1;
//variables that hold how many since last of opposite sex
int sessionM=0, sessionF=0;
bool endStarvationF = false;
bool endStarvationM = false;

//count for number in bathroom and number waiting
int fc=0, fwc=0, mc=0, mwc=0;

/*
* instead of passing the id as the parameter, this just uses a global variable
* and a multex 
*/
 
void * male(void * noparam) {
  int i,eid,t;

  //geting an id number
  semaphore_down( &multex );
  eid = malen;
  malen++ ;
  semaphore_up( &multex );
  printf("male %d: Starting \n",eid);

  while (1==1)  {

   //work for a while to create a random linup
    t = (int) rand() % 10; 
    printf("male %d: is working %d seconds\n",eid,t);
    // have the thread sleep while at work
    sleep (t);

    //the thread is ready for the bathroom
    printf("male %d: needs to use the bathroom\n",eid);
    semaphore_down (&bathroom); 
    //determine if the female threads have been waiting too long
    if(endStarvationF == true)
    {
        //add male to the line
        mwc++;
        semaphore_up(&bathroom);
        printf("male %d: is in line\n",eid);
        semaphore_down(&malesS);//the line of males waiting
        
        // allow into the bathroom
        semaphore_down (&bathroom); 
        mc++;
        sessionM++;
        semaphore_up(&bathroom);
        
    }
    else if (fc == 0) {  //bathroom has no females, so no waiting for male
      mc++;
      sessionM++;
      semaphore_up(&bathroom);  
    } 
    else {
      //cannot use the restroom yet add to wait
      mwc++;
      semaphore_up(&bathroom);
      printf("male %d: is in line\n",eid);
      semaphore_down(&malesS);//the line of males waiting
       
      //finally can use the bathroom, mark the count up.
      semaphore_down (&bathroom); 
        mc++;
        sessionM++;
      semaphore_up(&bathroom);
    }


    //use bathroom for random time
    t = (int) rand() % 10; 
    printf("male %d: is using the bathroom for %d seconds\n",eid,t);
    sleep (t);

    //test for starvation of either male or female in line based on percentage of workers
    semaphore_down(&bathroom);
      double test = (double) fwc;
      test = test / (double) femalen;
      if(test >= .25 || sessionM > fwc && fwc != 0) //this test notices starvation starvation
      {
          endStarvationF = true;
      }
      mc--;
      //check for change of who can use the restroom
      if (mc == 0) {
        endStarvationM = false;
        printf("male %d: signaling the line to start\n",eid);
        sessionM=0;
        //allow in all females in line
        for (i=0; i<fwc; i++)
        {
          semaphore_up(&femalesS);        
        }
        fwc =0;
      }
    
    semaphore_up(&bathroom);
    printf("male %d: is done using the bathroom\n",eid);
  } 
}

//create female workers
void * female(void * noparam) {
  int i,t,eid;
  semaphore_down( &multex );
  eid = femalen;
  femalen++ ;
  semaphore_up( &multex );
  printf("female %d: Starting \n",eid);

  //set to infinite for working 24/7
  while (1==1) {
    //allow female to work
    t = (int) rand() % 10;
    printf("female %d: is working for %d seconds\n",eid,t);
    sleep (t);
    //female is ready for restroom
    printf("female %d: needs to use the bathroom\n",eid);
    semaphore_down (&bathroom); 
    //check for male starvation
    if(endStarvationM == true)
    {
        fwc++;
        semaphore_up(&bathroom);
        printf("female %d: is in line\n",eid);
        semaphore_down(&femalesS);//the line of females waiting
        
        semaphore_down (&bathroom); 
        fc++;
        sessionF++;
        semaphore_up(&bathroom);
    }
    else if (mc == 0) {  // no males in restroom, allow female to enter
        fc++;
        sessionF++;
        semaphore_up(&bathroom);
    }
    else {
      //wait for line to allow female to enter
      fwc++;
      semaphore_up(&bathroom);
      printf("female %d: is in line\n",eid);
      semaphore_down(&femalesS);//the line of females waiting

      //finally can use the bathroom, mark the count up.
      semaphore_down (&bathroom); 
         fc++;
         sessionF++;
      semaphore_up(&bathroom);
    }


    //use bathroom
    t = (int) rand() % 10; 
    printf("female %d: is using the bathroom for %d seconds\n",eid,t);
    sleep (t);

    //check for starvation logic
    semaphore_down(&bathroom);
      double test = (double) mwc;
      test = test / (double) malen;
      if(test >= .25 || sessionF > mwc && mwc !=0) //this test notices starvation
      {
          endStarvationM = true;
      }
      //female done in restroom
      fc--;
      if (fc == 0) {
        endStarvationF = false;
        printf("female %d: signaling the line to start\n",eid);
        sessionF=0;
        for (i=0; i<mwc; i++)
        {
          semaphore_up(&malesS);
        }  
        mwc =0;

      }
    
    semaphore_up(&bathroom);
    printf("female %d: is done with the bathroom\n",eid);
    
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
  semaphore_init(&malesS,0);
//init semaphore line to 0, since everyone must wait
  semaphore_init(&femalesS,0);
//init semaphore bathroom to 1, so first person can go in,  
  semaphore_init(&bathroom,1);
//init semaphore multex to 1, so first person can entry critical section
  semaphore_init(&multex,1);

// create threads 
  for (i=0; i<num_m; i++) {
     if (pthread_create(&males[i], 0, male, 0)) {
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
  printf ("all threads created\n");


//wait for threads to finish 
  for(i=0;i<num_m;i++) 
    pthread_join(males[i], 0);

  for(i=0;i<num_f;i++) 
  pthread_join(females[i], 0);

}
