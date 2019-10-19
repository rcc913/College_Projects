//Ryan Cook
//COSC 4740
//Program 2
//Commander
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
using namespace std;

int main()
{
    //cout << "I am Commander" << endl;
    int mypipe[2], status, child, num;
    char p0[10], p1[10], action;
    
    if (child = pipe(mypipe)) 
    {
        perror("Pipe Failed");
        exit(1);
    }
    //creating strings to send as pipe address
    sprintf(p0, "%d", mypipe[0]);
    sprintf(p1, "%d", mypipe[1]);
    
    if ((child = fork()) == -1) 
    {
        perror("Fork Failed");
        exit(1);
    }
    else if (child == 0)
    {
        //exec to process_manager
        execl("process_manager", "process_manager", p0, p1, NULL);
        exit(0);
    }
    else
    {
        //close read side of pipe
        close(mypipe[0]);
        cin >> action;
        while(action != 'T')
        {
            if(action == 'S')
            {
                //send char
                write(mypipe[1], (void *)&action, sizeof(char));
                //get 3 numbers and send 3 numbers
                cin >> num;
                write(mypipe[1], (void *)&num, sizeof(int));
                cin >> num;
                write(mypipe[1], (void *)&num, sizeof(int));
                cin >> num;
                write(mypipe[1], (void *)&num, sizeof(int));
            }
            else if (action == 'B')
            {
                //write char
                write(mypipe[1], (void *)&action, sizeof(char));
                //get number and send number
                cin >> num;
                write(mypipe[1], (void *)&num, sizeof(int));
            }
            else if (action == 'U')
            {
                //write char
                write(mypipe[1], (void *)&action, sizeof(char));
                //get number and send number
                cin >> num;
                write(mypipe[1], (void *)&num, sizeof(int));
            }
            else if (action == 'Q')
            {
                //write char
                write(mypipe[1], (void *)&action, sizeof(char));
            }
            else if (action == 'C')
            {
                //write char
                write(mypipe[1], (void *)&action, sizeof(char));
                //get 1 char 1 number send both
                cin >> action;
                write(mypipe[1], (void *)&action, sizeof(char));
                cin >> num;
                write(mypipe[1], (void *)&num, sizeof(int));
            }
            else if (action == 'P')
            {
                //send char
                write(mypipe[1], (void *)&action, sizeof(char));
            }
            else
            {
                //error for invalid char
                cout << "Error in entry, closing program" << endl;
                exit(1);
            }
            //sleep for 2 seconds
            sleep (2);
            //get next action
            cin >> action;
        }
        //write the last action
        write(mypipe[1], (void *)&action, sizeof(char));
        //close pipe and wait then exit
        close(mypipe[1]);
        wait(&status);
        //cout << "Parent closing" << endl;
        exit(0);
    }
    exit(0);
}