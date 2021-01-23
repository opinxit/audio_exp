CC = gcc
LIBS = -lm
PROGS = midi2freq
BUILD =./build/

all: ${PROGS}

midi2freq: midi2freq.c
	${CC} ${LIBS} midi2freq.c -o ${BUILD}midi2freq
