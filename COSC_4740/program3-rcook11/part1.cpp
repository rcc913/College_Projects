//Ryan Cook
//COSC 4740
//Program 3
//#1

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "semp.h"

Semaphore p[5], mutex;
bool forks[5];

void think(int id)
{
    int t = (int) rand() %10+1;
    printf("Philosopher %d: Is pondering thoughts for %d seconds\n", id,t);
    sleep(t);
    printf("Philosopher %d: Is now hungry\n",id); 
}

void take_forks(int id)
{
    int left = ((id+4)%5);
    int right = ((id+1)%5);
    semaphore_down(&mutex);
    //run a test here
    if(forks[left] == true && forks[right]==true)
    {
        printf("Philosopher %d: has taken forks\n",id);
        forks[id] = false;
        semaphore_up(&mutex);
    }
    else
    {
        printf("Philosopher %d: is waiting for Philosophers %d and %d\n",id,left,right);
        bool flag = true;
        while(flag)
        {
            semaphore_up(&mutex);
            semaphore_down(&p[left]);
            semaphore_down(&mutex);
            if(forks[left] == true)
            {
                if(forks[right] == true)
                {
                    forks[id] = false;
                    semaphore_up(&mutex);
                    flag = false;
                }
            }
        }
    }
    
}

void put_forks(int id)
{ 
    semaphore_down(&mutex);
    forks[id] = true;
    printf("Philosopher %d: has returned forks\n",id);
    semaphore_up(&mutex);
    semaphore_up(&p[id]);
}

void eat(int id)
{
    int t = (int) rand() %10+1;
    printf("Philosopher %d: Eating for %d seconds\n", id,t);
    sleep(t);
}

void * philStuff(void * pID)
{
    int id = *((int *)pID);
    printf("Hello from Philosopher %d\n",id);
    while(1)
    {
        think(id);
        take_forks(id);
        eat(id);
        put_forks(id);
    }
    
}

int main()
{
    semaphore_init(&mutex, 1);
    for(int i = 0; i < 5; i++)
    {
        semaphore_init(&p[i], 1);
        forks[i] = true;
    }
    pthread_t phil[5];
    int pnum=0;
    for(int i = 0; i < 5; i++)
    {
        if (pthread_create(&phil[i], NULL, philStuff, &pnum)) {
            perror("Error creating a Philosopher thread\n");
            exit(1);
        }
        sleep(1);
        ++pnum;
    }

    for(int i=0;i < 5; i++)
    {
        pthread_join(phil[i], 0);
    }        
    
    return 0;
}