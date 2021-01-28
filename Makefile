CC = gcc
LIBS = -lm
PROGS = breakdur expbrk expad iscale midi2freq

all: ${PROGS}

clean: 
	rm -f ${PROGS} 

breakdur: breakdur.c
	${CC} ${LIBS} breakdur.c -o breakdur

expbrk: expbrk.c
	${CC} ${LIBS} expbrk.c -o expbrk

expad: expad.c
	${CC} ${LIBS} expad.c -o expad

iscale: iscale.c
	${CC} ${LIBS} iscale.c -o iscale

midi2freq: midi2freq.c
	${CC} ${LIBS} midi2freq.c -o midi2freq
