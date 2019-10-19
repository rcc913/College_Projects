The following code is used the pthread lecture.
semp.h is library of semaphore subroutines included by the following 
three programs.
oddevens.cpp creates two threads and uses semaphores
   oddevens, makes sure that odd and even always alternate back and forth

buffer.cpp creates 1 thread each for a producer and a consumer
buffern.cpp creates n threads for the producer consumer problem, where they are from the command line
buffernc.cpp, the same as buffern.cpp, but see comment at the top of the file for the difference

unisex1.cpp is solution for the unisex bathroom, that almost works
unisex.cpp is solution for the unisex bathroom, that works, but haves starvation issues

When looking at the code, 
semaphore_down is just like Wait()
semaphore_up is similar to Signal(), except wakes all blocked threads.


to compile this on cosc linux 
g++ oddevens.cpp -lpthread -o oddevens
g++ buffer.cpp -lpthread -o buffer
g++ buffern.cpp -lpthread -o buffern
g++ buffernc.cpp -lpthread -o buffernc
g++ unisex.cpp -lpthread -o unisex
g++ unisex1.cpp -lpthread -o unisex1

