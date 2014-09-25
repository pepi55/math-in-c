OBJS = main.c src/sdlFunc.c src/gameFunc.c
OBJ_NAME = testProgram

CC = gcc
COMPILER_FLAGS = -std=c99 -O2 -pedantic -W -Wall
LINKER_FLAGS = -lSDL2 -lSDL2_image -lm

all: $(OBJS)
	$(CC) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(OBJS) -o $(OBJ_NAME)
