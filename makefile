.PHONY: all clean clear

CC = g++
CFLAGS = -O2 -Wall -std=c++17 `sdl2-config --cflags --libs`
LFLAGS = -lSDL2_image
OBJS = vector3D.o position.o kinematic.o steering.o texture.o collision.o character.o graph.o 

all: aifg clean

aifg: main.cpp game.o
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $< $(OBJS) game.o

vector3D.o: vector3D.cpp vector3D.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

position.o: position.cpp position.hpp vector3D.o 
	$(CC) $(CFLAGS) -c -o $@ $<

kinematic.o: kinematic.cpp kinematic.hpp position.o 
	$(CC) $(CFLAGS) -c -o $@ $<	

steering.o: steering.cpp steering.hpp position.o
	$(CC) $(CFLAGS) -c -o $@ $<		

texture.o: texture.cpp texture.hpp
	$(CC) $(CFLAGS) -c -o $@ $<

collision.o: collision.cpp collision.hpp vector3D.o 
	$(CC) $(CFLAGS) -c -o $@ $<

character.o: character.cpp character.hpp position.o texture.o steering.o graph.o collision.o 
	$(CC) $(CFLAGS) -c -o $@ $< 

game.o: game.cpp game.hpp $(OBJS)
	$(CC) $(CFLAGS) -c -o $@ $< 

graph.o: graph.cpp graph.hpp vector3D.o 
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm *.o

clear:
	rm game
	rm *.o
