CC=g++
#CFLAGS = -ggdb -Wall
CFLAGS = 

all: helloworld 

#put all the file needed, like .h files as well
#note, you need a tab, not spaces.

hellworld.o: helloworld.cpp
	$(CC) $(CFLAGS) -c helloworld.cpp

hellworld: hellworld.o
	$(CC) $(CFLAGS) -o hellworld helloworld.o 

clean: 
	rm -f helloworld.o helloworld

