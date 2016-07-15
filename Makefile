#!/bin/bash

CC=g++
CFLAGS= -ansi -O5 -Wall -std=c++14 -Wno-unused-result
LDFLAGS= -ansi -lm -Wall
EXEC=caveman
OBJ1= caveman_generator.o graph.o

all: $(EXEC)

caveman : $(OBJ1) main_caveman.o
	$(CC) -o $@ $^ $(LDFLAGS)


##########################################
# Generic rules
##########################################

%.o: %.cpp %.h
	$(CC) -o $@ -c $< $(CFLAGS)

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -f *.o *~ $(EXEC)
