//Ryan Cook
//COSC 4740
//Program 2
//Process Manager
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include "QueueArray.h"
#include <vector>
using namespace std;

struct pcb
{
    //every process keeps track of these values
    int pid, priority, value, start_time, run_time, cpu_time, end_time, quantum;
};

void printProcess(pcb c)
{
    //format print for blocked processes and running process
    cout << c.pid << "\t" <<  c.priority << "\t\t" << c.value << "\t" << c.start_time << "\t\t" << c.cpu_time << endl;
}

void printReady(pcb c)
{
    //format print for ready state
    cout << c.pid << "\t" << c.value << "\t" << c.start_time << "\t\t" << c.cpu_time << endl;
}

bool timeUpdate(pcb &c, int &Time)
{
    //update time
    Time = Time+1;
    //update RunningState cpu time
    c.cpu_time = c.cpu_time + 1;
    //update of RunningState quantum
    if(c.quantum == 1)
    {
        //quantum expired so change quantum and priority by 0:1->1:2->2:4->3:8
        switch (c.priority)
        {
            case 0: 
                c.priority = 1;
                c.quantum = 2;
                break;
            case 1:
                c.priority = 2;
                c.quantum = 4;
                break;
            case 2:
                c.priority = 3;
                c.quantum = 8;
                break;
            case 3:
                c.priority = 3;
                c.quantum = 8;
                break;
            default:
                c.priority = 0;
                c.quantum = 1;
                break;
        }
        //saying quantum expired
        return true;
    }
    else
    {
        //quantum did not expire so update 
        c.quantum = c.quantum - 1;
        return false;
    }
}

int main(int argc, char *argv[])
{
    //cout<< "I am Process Manager" << endl;
    int mypipe[2];
    int Time=0;
    vector<pcb> pcb_table(100);  
    QueueArray <int> ReadyState(4);
    QueueArray <int> BlockedState0(4), BlockedState1(4), BlockedState2(4);
    int RunningState;
    bool rEmpty = true;
    //set up of pipes
    mypipe[0] = atoi(argv[1]);
    mypipe[1] = atoi(argv[2]);
    char action;
    int num;
    //Read first action
    read(mypipe[0], &action, sizeof(char));
    while(action != 'T')
    {
        if(action == 'S')
        {
            //creating a pcb for entry to table
            pcb temp;
            read(mypipe[0], (int *)&num, sizeof(num));
            temp.pid = num;
            read(mypipe[0], (int *)&num, sizeof(num));
            temp.value = num;
            read(mypipe[0], (int *)&num, sizeof(num));
            temp.run_time = num;
            temp.priority = 0;
            temp.quantum = 1;
            temp.start_time = Time;
            temp.end_time = -1;
            pcb_table[temp.pid] = temp;
            if(rEmpty == true)
            {
                //checking if cpu is being used
                RunningState = temp.pid;
                rEmpty = false;
            }
            else
            {
                //placing new process in ReadyState
                ReadyState.Enqueue(temp.pid, temp.priority);
            }
        }
        else if (action == 'B')
        {
            //read int from pipe
            read(mypipe[0], (int *)&num, sizeof(num));
            int rid = num;
            if(rid==0)
            {
                //blocking for resource 0 and replacing with next ReadyState process
                BlockedState0.Enqueue(RunningState, pcb_table[RunningState].priority);
                RunningState = ReadyState.Dequeue();
            }
            else if(rid==1)
            {
                //blocking for resource 1 and replacing with next ReadyState process
                BlockedState1.Enqueue(RunningState, pcb_table[RunningState].priority);
                RunningState = ReadyState.Dequeue();
            }
            else if(rid==2)
            {
                //blocking for resource 2 and replacing with next ReadyState process
                BlockedState2.Enqueue(RunningState, pcb_table[RunningState].priority);
                RunningState = ReadyState.Dequeue();
            }
        }
        else if (action == 'U')
        {
            //read int from pipe
            read(mypipe[0], (int *)&num, sizeof(num));
            int rid = num;
            if(rid==0)
            {
                //unblocking resource 0 and placing in ReadyState
                int temp = BlockedState0.Dequeue();
                ReadyState.Enqueue(temp, pcb_table[temp].priority);
            }
            else if(rid==1)
            {
                //unblocking resource 1 and placing in ReadyState
                int temp = BlockedState1.Dequeue();
                ReadyState.Enqueue(temp, pcb_table[temp].priority);
            }
            else
            {
                //unblocking resource 2 and placing in ReadyState
                int temp = BlockedState2.Dequeue();
                ReadyState.Enqueue(temp, pcb_table[temp].priority);
            }
        }
        else if (action == 'Q')
        {
            //calling time update function
            bool change = timeUpdate(pcb_table[RunningState], Time);
            if(pcb_table[RunningState].run_time == pcb_table[RunningState].cpu_time)
            {
                //checking if the process is finished running and updating end time
                pcb_table[RunningState].end_time = Time;
                if(ReadyState.QAsize() == 0)
                {
                    //determining that cpu is empty and no processes to run
                    rEmpty = true;
                    RunningState = 0;
                }
                else
                {
                    //loading next process to RunningState
                    RunningState = ReadyState.Dequeue();
                }
            }
            else if (change == true)
            {
               //grab next off ReadyState because quantum expired
               ReadyState.Enqueue(RunningState, pcb_table[RunningState].priority);
               RunningState = ReadyState.Dequeue();
            }
        }
        else if (action == 'C')
        {
            //read char and int from pipe
            read(mypipe[0], &action, sizeof(char));
            read(mypipe[0], (int *)&num, sizeof(num));
            if(action == 'A')
            {
                //addition of value
                pcb_table[RunningState].value = pcb_table[RunningState].value + num;
            }
            else if(action == 'S')
            {
                //subtraction of value
                pcb_table[RunningState].value = pcb_table[RunningState].value - num;
            }
            else if(action == 'M')
            {
                //multiplication of value
                pcb_table[RunningState].value = pcb_table[RunningState].value * num;
            }
            else if(action == 'D')
            {
                //division of value
                pcb_table[RunningState].value = pcb_table[RunningState].value / num;
            }
            else
            {
                //Error check for improper command
                cout << "Improper command in action C, closing program" << endl;
                close(mypipe[0]);
                close(mypipe[1]);
                exit(1);
            }
            //calling time update function
            bool change = timeUpdate(pcb_table[RunningState], Time);
            if(pcb_table[RunningState].run_time == pcb_table[RunningState].cpu_time)
            {
                //checking if the process is finished running and updating end time
                pcb_table[RunningState].end_time = Time;
                if(ReadyState.QAsize() == 0)
                {
                    //CPU is waiting for next process
                    rEmpty = true;
                    RunningState = 0;
                }
                else
                {
                    //upload next process from ReadyState to RunningState
                    RunningState = ReadyState.Dequeue();
                }
            }
            else if (change == true)
            {
               //grab next off ReadyState because quantum expired 
               ReadyState.Enqueue(RunningState, pcb_table[RunningState].priority);
               RunningState = ReadyState.Dequeue();
            }
        }
        else if (action == 'P')
        {
            //Format for printing 
            int temp=0;
            int *current;
            cout << "****************************************************************" << endl;
            cout << "The current system state is as follows:" << endl;
            cout << "****************************************************************" << endl << endl;
            cout << "CURRENT TIME: " << Time <<  endl << endl;
            cout << "RUNNING PROCESS: " << endl;
            cout << "PID\tPRIORITY\tVALUE\tSTART TIME\tTOTAL CPU TIME" << endl;
            printProcess(pcb_table[RunningState]);
            cout << "BLOCKED PROCESSES: " << endl;
            cout << "Queue of processes blocked for resource 0: " << endl;
            temp = BlockedState0.QAsize();
            if(temp == 0)
            {
                //checks if QueueArray is empty and displays
                cout << "Is Empty" << endl;
            }
            else
            {
                cout << "PID\tPRIORITY\tVALUE\tSTART TIME\tTOTAL CPU TIME" << endl;
                for(int j = 0; j < 4; j++)
                {
                    //works through the Blocked states priorities to print
                    current = BlockedState0.Qstate(j);
                    for(int i =0; i < BlockedState0.Qsize(j); i ++)
                    {
                        printProcess(pcb_table[current[i]]);
                    }
                }
            }
            cout << "Queue of processes blocked for resource 1: " << endl;
            temp = BlockedState1.QAsize();
            //checks if QueueArray is empty
            if(temp == 0)
            {
                cout << "Is Empty" << endl;
            }
            else
            {
                cout << "PID\tPRIORITY\tVALUE\tSTART TIME\tTOTAL CPU TIME" << endl;
                for(int j = 0; j < 4; j++)
                {
                    //Works through BlockedState1 priorities to print
                    current = BlockedState1.Qstate(j);
                    for(int i =0; i < BlockedState1.Qsize(j); i ++)
                    {
                        printProcess(pcb_table[current[i]]);
                    }
                }
            }
            cout << "Queue of processes blocked for resource 2: " << endl;
            temp = BlockedState2.QAsize();
            //checks if QueueArray is empty
            if(temp == 0)
            {
                cout << "Is Empty" << endl;
            }
            else
            {
                cout << "PID\tPRIORITY\tVALUE\tSTART TIME\tTOTAL CPU TIME" << endl;
                for(int j = 0; j < 4; j++)
                {
                    //works through BlockedState2 priorities to print
                    current = BlockedState2.Qstate(j);
                    for(int i =0; i < BlockedState2.Qsize(j); i ++)
                    {
                        printProcess(pcb_table[current[i]]);
                    }
                }
            }
            cout << endl;
            cout << "PROCESSES READY TO EXECUTE: " << endl;
            cout << "Queue of processes with priority 0: " << endl;
            //checks if ReadyState Priority 0 is empty
            temp = ReadyState.Qsize(0);
            if(temp == 0)
            {
                cout << "Is Empty" << endl;
            }
            else
            {
                cout << "PID\tVALUE\tSTART TIME\tTOTAL CPU TIME" << endl;
                current = ReadyState.Qstate(0);
                //print all values in priority 0
                for(int i = 0; i < temp; i++)
                {
                    printReady(pcb_table[current[i]]);
                }
            }
            cout << "Queue of processes with priority 1: " << endl;
            temp = ReadyState.Qsize(1);
            //checks if priority 1 is empty
            if(temp == 0)
            {
                cout << "Is Empty" << endl;
            }
            else
            {
                cout << "PID\tVALUE\tSTART TIME\tTOTAL CPU TIME" << endl;
                current = ReadyState.Qstate(1);
                //print all values in priority 1
                for(int i = 0; i < temp; i++)
                {
                    printReady(pcb_table[current[i]]);
                }
            }
            cout << "Queue of processes with priority 2: " << endl;
            temp = ReadyState.Qsize(2);
            //checks if priority 2 is empty
            if(temp == 0)
            {
                cout << "Is Empty" << endl;
            }
            else
            {
                cout << "PID\tVALUE\tSTART TIME\tTOTAL CPU TIME" << endl;
                current = ReadyState.Qstate(2);
                //prints all values in priority 2
                for(int i = 0; i < temp; i++)
                {
                    printReady(pcb_table[current[i]]);
                }
            }
            cout << "Queue of processes with priority 3: " << endl;
            temp = ReadyState.Qsize(3);
            //checks if priority 3 is empty
            if(temp == 0)
            {
                cout << "Is Empty" << endl;
            }
            else
            {
                cout << "PID\tVALUE\tSTART TIME\tTOTAL CPU TIME" << endl;
                current = ReadyState.Qstate(3);
                //prints all values in priority 3
                for(int i = 0; i < temp; i++)
                {
                    printReady(pcb_table[current[i]]);
                }
            }
            cout << endl;
            cout << "****************************************************************" << endl;
            cout << endl;
            
        }
        else
        {
            //Shows that the pipe had a character failure
            cout << "Pipe is corrupted" << endl;
            close(mypipe[0]);
            close(mypipe[1]);
            exit(1);
        }
        //checks next character in pipe
        read(mypipe[0], &action, sizeof(char)); 
    }
    //close pipes and kill child
    close(mypipe[0]);
    close(mypipe[1]);
    //cout << "Closing Child" << endl;
    exit(0);
}