//Ryan Cook
//COSC 4740
//Program 3
//#2

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "semp.h"

#define MAX_BOWL 25
Semaphore empty, notempty, mutex;
int bowl = MAX_BOWL;

void * savageStuff(void * sID)
{
    int hungryfor=0;
    int t=0;
    int id;
    id = *((int *)sID);
    printf("My Name Savage %d\n",id);
    while(1)
    {
        t = (int) rand() %10+1; //will do savage stuff for 1 to 10 seconds
        sleep(t);
        hungryfor = (int) rand() % 7+1; //can eat 1 to 7 portions
        printf("Savage %d hungry, need %d servings\n",id,hungryfor);
        
        //critical section
        semaphore_down(&mutex);
        if((bowl - hungryfor) < 0)
        {
            printf("Savage %d eat %d servings, bowl now %d servings\n",id,bowl,0);
            hungryfor = -1*(bowl-hungryfor);
            bowl = 0;
            printf("Savage %d need cook now\n",id);
            semaphore_down(&notempty);
            //cook is refilling the bowl
            //sleep(1);
            semaphore_up(&empty);
            sleep(1); // allows cook to get out of the way before eating again
            bowl -= hungryfor;
            printf("Savage %d eat %d servings, bowl now %d servings\n",id,hungryfor,bowl);
            
            semaphore_up(&mutex);
        }
        else
        {
            bowl -= hungryfor;
            printf("Savage %d eat %d servings, bowl now %d servings\n",id,hungryfor,bowl);
            semaphore_up(&mutex);
        }        
    }    
    
}

void * cookStuff(void * noparam)
{
    while(1)
    {
        semaphore_down(&empty);
        sleep(5); //preparing savages meal
        printf("Cook fill pot\n");
        bowl = MAX_BOWL;
        semaphore_up(&notempty);
    }
}

int main()
{
    semaphore_init(&mutex, 1);
    semaphore_init(&empty, 1);
    semaphore_init(&notempty, 0);
    pthread_t savage[10];
    int savnum=1;
    for(int i = 0; i < 10; i++)
    {
        if (pthread_create(&savage[i], NULL, savageStuff, &savnum)) {
            perror("Error creating a savage thread\n");
            exit(1);
        }
        sleep(1);
        ++savnum;
    }
    
    pthread_t cook;
    if (pthread_create(&cook, NULL, cookStuff, NULL)) {
        perror("Error creating the cook thread\n");
        exit(1);
    }
    
    for(int i=0;i < 10; i++)
    {
        pthread_join(savage[i], 0);
    }        
    pthread_join(cook, 0);
    
    return 0;
}