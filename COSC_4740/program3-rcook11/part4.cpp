//Ryan Cook
//COSC 4740
//Program 3
//#4

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include "semp.h"


Semaphore mutex, t, call, finish, finish1, wait;
int count = 0; int waiting = 0; bool call0 = false; bool call1 = false; bool sendback;

void * client(void * cID)
{
    int id = *((int *)cID);
    printf("Starting Client %d\n",id);
    while(1)
    {
        int time = (int) rand() %10+1;
        printf("Client %d: is trying to break the machine for %d seconds\n",id,time);
        sleep(time);
        printf("Client %d: broke a part and is calling the help desk\n",id);
        semaphore_up(&call);
        if(id == 0)
        {
            semaphore_down(&mutex);
            if(call1==true)
            {
                sendback = false;
            }
            else
            {
                sendback = true;
            }
            call0 = true;
            semaphore_up(&mutex);
            semaphore_down(&finish);
            semaphore_down(&finish);
            semaphore_down(&finish);
        }
        else
        {
            semaphore_down(&mutex);
            if(call0==true)
            {
                sendback = true;
            }
            else
            {
               sendback = false;
            }
            call1 = true;
            semaphore_up(&mutex);
            semaphore_down(&finish1);
            semaphore_down(&finish1);
            semaphore_down(&finish1);
        }
        
        printf("Client %d: Part no longer broken\n",id);
    }
    
    
}

void * helpDesk(void * noparam)
{
    printf("Starting Help Desk\n");
    while(1)
    {
        semaphore_down(&call);
        semaphore_up(&t);
        semaphore_up(&t);
        semaphore_up(&t);
    }
    
}


void * tech(void * tID)
{
    int id = *((int *)tID);
    printf("Starting tech %d\n",id);
    /*
    bool flag = true;
    while(1)
    {
        int time = (int) rand() %10+5;
        printf("Tech %d: on a %d second coffee break\n",id,time);
        sleep(time);
        semaphore_down(&t);      
        printf("Tech %d: waiting on techs\n",id);
        semaphore_down(&mutex);
        if(count == 3)
        {
            semaphore_up(&mutex);
            semaphore_down(&wait);
        }
        else
        {
            semaphore_up(&mutex);   
        }
        semaphore_down(&mutex);
        count++;
        semaphore_up(&mutex); 
        while(flag)
        {
            semaphore_down(&mutex);
            if(count==3)
            {
                semaphore_up(&mutex);
                flag=false;
            }
            else
            {   
                semaphore_up(&mutex);
            }
        }
        printf("Tech %d: Is on the Job\n",id);
        sleep(5);
        semaphore_up(&finish);
        semaphore_down(&mutex);
        count--;
        semaphore_up(&wait);
        semaphore_up(&mutex);
    }   
    */
    bool flag = true;
    while(1)
    {
        int time = (int) rand() %10+1;
        printf("Tech %d: is on a %d second coffee break\n",id, time);
        sleep(time);
        semaphore_down(&t);
        semaphore_down(&mutex);
        if(count >= 3)
        {
            waiting++;
            semaphore_up(&mutex);
            semaphore_down(&wait);
            semaphore_down(&mutex);
            count++;
            semaphore_up(&mutex);
        }
        else
        {
            count++;
            semaphore_up(&mutex);
        }
        printf("Tech %d: is waiting for more techs\n",id);
        while(flag)
        {
            semaphore_down(&mutex);
            if(count >= 3)
            {
                semaphore_up(&mutex);
                flag = false;
                sleep(1);
            }
            else
            {
                semaphore_up(&mutex);
            }
        }
        printf("Tech %d: is on the job\n",id);
        semaphore_down(&mutex);
        count--;
        if(waiting > 0)
        {
            semaphore_up(&wait);
            waiting--;
        }
        semaphore_up(&mutex);
        semaphore_down(&mutex);
        if(sendback == true)
        {
            semaphore_up(&mutex);
            semaphore_up(&finish);
        }
        else
        {
            semaphore_up(&mutex);
            semaphore_up(&finish1);
        }
        sleep(2);
    }
    
}


int main()
{
    semaphore_init(&call, 0);
    semaphore_init(&finish, 0);
    semaphore_init(&t, 0);
    semaphore_init(&mutex,1);
    semaphore_init(&wait, 0);
    
    pthread_t techs[5];
    pthread_t hd;
    pthread_t c[2];
    for(int i = 0; i < 5; i++)
    {
        if (pthread_create(&techs[i], NULL, tech, &i)) {
            perror("Error creating a Tech thread\n");
            exit(1);
        }
        sleep(1);
    }
    for(int i = 0; i < 2; i++)
    {
        if (pthread_create(&c[i], NULL, client, &i)) {
            perror("Error creating a Client thread\n");
            exit(1);
        }
        sleep(1);
    }
    if (pthread_create(&hd, NULL, helpDesk, NULL)) {
            perror("Error creating Help Desk thread\n");
            exit(1);
    }
    sleep(1);

    for(int i = 0; i < 5; i++)
    {
        pthread_join(techs[i], 0);
    } 
    for(int i=0;i < 2; i++)
    {
        pthread_join(c[i], 0);
    }
    pthread_join(hd, 0);
    return 0;
}