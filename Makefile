CC = gcc
LIBS = -lm
PROGS = breakdur iscale midi2freq

all: ${PROGS}

clean: 
	rm -f ${PROGS} 

midi2freq: midi2freq.c
	${CC} ${LIBS} midi2freq.c -o midi2freq

iscale: iscale.c
	${CC} ${LIBS} iscale.c -o iscale
