/*
  Here is some example code for Program 1.  You don't have to use any of
  it, but if you are having trouble, it might get you started.
  The etype used through this is the template class your need to create
  in this class, etype means Example TYPE
*/
#ifndef QUEUE_ARRAY
#define QUEUE_ARRAY
#include <cstdlib>  //This is for the exit command.
#include <queue>
#include <iostream>

using namespace std;

template <class etype>
class q
{
  public:
    etype get();
    void set(const etype &item);
    int size();
    etype* makeArray(); 
  private:
    queue<etype> storage;
};

template <class etype>
etype q<etype>::get()
{
	etype temp = storage.front();
	storage.pop();
    return temp;
}

template <class etype>
void q<etype>::set(const etype &item)
{
    storage.push(item);
}

template <class etype>
int q<etype>::size()
{
    return storage.size();
}

template <class etype>
etype* q<etype>::makeArray()
{
    if(storage.empty())
    {
        return NULL;
    }
    etype * current = new etype[storage.size()];
    for(int i = 0; i < storage.size(); i++)
    {
        current[i] = storage.front();
		storage.pop();
		storage.push(current[i]);
    }
    return current;
}

template <class etype>
class QueueArray
{
  public:
    QueueArray(int);
    ~QueueArray();
    int Asize(); 
    etype Dequeue();
    int Enqueue(const etype &item,const int index);
    int QAsize(); 
    int Qsize(int index) ;
    etype* Qstate(int index);

  private:
    int size;      //size of the array
    q<etype>* array;   //the array of queues
    int totalItems; //total number of items stored in the queues
};

//Constructor for the queue array.  It sets the default size
//to 10, initializes the private variables size and totalItems
template <class etype>
QueueArray<etype>::QueueArray(int sz = 10):size(sz),totalItems(0)
{
  array = new q<etype>[size];
  if( array == NULL)
  {
    cout << "Not enough memory to create the array" << endl;
    exit(-1);
  }
}
template <class etype>
QueueArray<etype>::~QueueArray()
{
	delete [] array;
}
template <class etype>
int QueueArray<etype>::Asize()
{
    //int Asize() : Returns the size of the array.
    return size;
}

template <class etype>
etype QueueArray<etype>::Dequeue()
{
    /*
    type Dequeue(): Dequeues an item from the first non-empty queue in the array,
    i.e., from the non-empty queue at the lowest numbered index in the array. Returns
    the dequeued item, if there is at least one item in the queue array; 0 otherwise.
    */
    for(int i = 0; i < size; i++)
    {
        if(array[i].size() != 0)
        {
            return array[i].get();
        }            
    }
    return 0;
}

template <class etype>
int QueueArray<etype>::Enqueue(etype const &item,const int index)
{
    /*
    int Enqueue(type item, int index): Enqueues item in the queue of array index
    index. Returns 1 if item is successfully enqueued; -1, if index is out of range; 0,
    otherwise
    */
    if(index<0 || index>size)
    {
        return -1;
    }
    else
    {
        array[index].set(item);
		return 1;
    }
    return 0;
}

template <class etype>
int QueueArray<etype>::QAsize()
{
    //int QAsize(): Returns the total number of items stored in the array of queues
    int sum =0;
    for(int i = 0; i<size; i++)
    {
        sum += array[i].size();
    }
    return sum;
}

template <class etype>
int QueueArray<etype>::Qsize(int index)
{
    /*
    int Qsize(int index): Returns the number of items in the queue at array index
    index; -1, if index is out of range.
    */
    if(index < 0 || index > size-1)
    {
        return -1;
    }
    else
    {
        return array[index].size();
    }
    
}

template <class etype>
etype* QueueArray<etype>::Qstate(int index)
{
    /*
    type* Qstate(int index): Copies all items stored in the queue at array index index
    in an array. Returns a pointer to this array; NULL, if index is out of range.
    */
    return array[index].makeArray();
}

#endif