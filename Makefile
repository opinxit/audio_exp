
INCLUDES 	= -I./include
LIBS 		= -L./lib -lportsf -lm
PROGS = breakdur expbrk expad iscale midi2freq sf2float sfgain sfnorm tforkraw
PORTSF=./lib/libportsf.a
CC = gcc

all: ${PROGS}

$(PORTSF): 
	cd portsf; make; make install

clean: 
	rm -f ${PROGS}
	rm -f *.o

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

sf2float: sf2float.c $(PORTSF)
	$(CC) -o sf2float sf2float.c $(INCLUDES) $(LIBS)

sfgain: sfgain.c $(PORTSF)
	$(CC) -o sfgain sfgain.c $(INCLUDES) $(LIBS)

sfnorm: sfnorm.c $(PORTSF)
	$(CC) -o sfnorm sfnorm.c $(INCLUDES) $(LIBS)

sfpan: sfpan.c  breakpoints.c $(PORTSF)
	$(CC) -o sfpan sfpan.c breakpoints.c $(INCLUDES) $(LIBS)

tforkraw: tforkraw.c
	${CC} ${LIBS} tforkraw.c -o tforkraw

veryclean:	clean
	cd portsf; make veryclean