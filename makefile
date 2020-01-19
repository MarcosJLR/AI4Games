.PHONY: all clean clear

CC = g++
CFLAGS = -O2 -Wall -std=c++14 `sdl2-config --cflags --libs`
LFLAGS = -lSDL2_image
OBJS = vector3D.o position.o texture.o 

all: game clean

game: game.cpp $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $< $(OBJS)

vector3D.o: vector3D.cpp vector3D.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

position.o: position.cpp position.hpp vector3D.o 
	$(CC) $(CFLAGS) -c -o $@ $<

texture.o: texture.cpp texture.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm *.o

clear:
	rm game
	rm *.o
