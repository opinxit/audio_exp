#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <portsf.h>
#include <breakpoints.h>
#include <wave.h>

#ifndef M_PI
#define M_PI (3.1415926535897932)
#endif
#define TWOPI (2.0 * M_PI)

// double oscil(double frequency);

void oscil_init(OSCIL* osc, unsigned long srate)
{
  osc->twopiovrsr = TWOPI / (double) srate;
  osc->curfreq = 0.0;
  osc->curphase = 0.0;
  osc->incr = 0.0;
}

OSCIL* oscil(void)
{
  OSCIL* osc = (OSCIL*) malloc(sizeof(OSCIL));
  if (osc == NULL) return NULL;

  return osc;
}

// local typing-saving macro for tick functions
#define OSC_WRAPPHASE  if(p_osc->curphase > TWOPI) p_osc->curphase -= TWOPI;	\
						if(p_osc->curphase < 0.0) p_osc->curphase += TWOPI


OSCIL* new_oscil(double srate)
{
  OSCIL* p_osc;

  p_osc = (OSCIL*) malloc(sizeof(OSCIL));
  if (p_osc == NULL) return NULL;

  p_osc->twopiovrsr = TWOPI / (double) srate;
  p_osc->curfreq = 0.0;
  p_osc->curphase = 0.0;
  p_osc->incr = 0.0;

  return p_osc;
}

double sinetick(OSCIL* p_osc, double freq)
{
  double val;

  val = sin(p_osc->curphase);

  if (p_osc->curfreq != freq) {
    p_osc->curfreq = freq;
    p_osc->incr = p_osc->twopiovrsr * freq;
  }

  p_osc->curphase += p_osc->incr;

  if (p_osc->curphase >= TWOPI) p_osc->curphase -= TWOPI;
  if (p_osc->curphase < 0.0) p_osc->curphase += TWOPI;

  return val;
}

// SQUARE
double sqtick(OSCIL* p_osc, double freq)
{
	double val;

	if (p_osc->curfreq != freq) {
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}

	if (p_osc->curphase <= M_PI) {
    val = 1.0;
  }	else {
    val = -1;
  }
		
	p_osc->curphase += p_osc->incr;
	OSC_WRAPPHASE;
	return val;
}

// SAWTOOTH DOWNWARDS
double sawdtick(OSCIL* p_osc, double freq)
{
	double val;

	if (p_osc->curfreq != freq) {
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}

	val =  1.0 - 2.0 * (p_osc->curphase * (1.0 / TWOPI) );
	p_osc->curphase += p_osc->incr;
	OSC_WRAPPHASE;
	return val;
}

// SAWTOOTH UPWARDS
double sawutick(OSCIL* p_osc, double freq)
{
	double val;

	if (p_osc->curfreq != freq) {
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}

	val =  (2.0 * (p_osc->curphase * (1.0 / TWOPI) )) - 1.0;
	p_osc->curphase += p_osc->incr;
	OSC_WRAPPHASE;
	return val;
}

// TRIANGLE
double tritick(OSCIL* p_osc, double freq)
{
	double val;

	if (p_osc->curfreq != freq) {
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}

	if (p_osc->curphase <= M_PI)	{
		val = (4.0 * (p_osc->curphase * (1.0 / TWOPI) )) - 1.0;
	}	else {
		val =  3.0 - 4.0 * (p_osc->curphase * (1.0 / TWOPI) );
	}

	p_osc->curphase += p_osc->incr;
	OSC_WRAPPHASE;
	return val;
}
