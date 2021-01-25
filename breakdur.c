#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NPOINTS (64)
// buffer size PER CHANNEL
#define BUFSIZE_CHAN (32768)
#define LINELENGTH (80)

typedef struct breakpoint {
		double time;
		double value;
} BREAKPOINT;

void print_points(const BREAKPOINT* points, unsigned long npoints)
{
	unsigned int i;

	printf("Printing all points:\n");
	for (i = 0; i < npoints; i++) {
		printf("%f: %f\n", points[i].time, points[i].value);
	}
	printf("\n");
}

void scale_values(BREAKPOINT* points, unsigned long npoints, float factor)
{
	unsigned int i;

	printf("Scaling values with factor of %f:\n", factor);
	for (i = 0; i < npoints; i++) {
		points[i].value *= factor;
		printf("%f: %f\n", points[i].time, points[i].value);
	}
	printf("\n");
}

void scale_times(BREAKPOINT* points, unsigned long npoints, float factor)
{
	unsigned int i;

	printf("Scaling times with factor of %f:\n", factor);
	for (i = 0; i < npoints; i++) {
		points[i].time *= factor;
		printf("%f: %f\n", points[i].time, points[i].value);
	}
	printf("\n");
}

// npoints size of BREAKPOINT array
BREAKPOINT maxpoint(const BREAKPOINT* points, unsigned long npoints)
{
	unsigned int i;
	BREAKPOINT point;

	point.time = 0.0;
	point.value = 0.0;

	if (npoints < 2) {
		printf("maxpoint requires more than one breakpoint\n");
		return point;
	}

	for (i = 1; i < npoints; i++) {
		if (point.value < points[i].value){
			point.value = points[i].value;
			point.time = points[i].time;
		}
	}			
	return point;
}

BREAKPOINT* get_breakpoints(FILE* fp, unsigned long* psize)
{
	int got;
	
	unsigned long npoints = 0, size = NPOINTS;
	double lasttime = 0.0;
	BREAKPOINT* points = NULL;	
	char line[LINELENGTH];

	if (fp==NULL) return NULL;

	points = (BREAKPOINT*) malloc(sizeof(BREAKPOINT) * size);
	
	if (points==NULL) return NULL;

	while (fgets(line, 80, fp)) {				
		if ((got = sscanf(line, "%lf%lf", &points[npoints].time, &points[npoints].value)) < 0) {
			continue;
		}
			
		if (got == 0) {
			printf("Line %lu has non-numeric data\n", npoints + 1);
			break;
		}

		if (got == 1) {
			printf("Incomplete breakpoint found at point %lu\n", npoints + 1);
			break;
		}
	
		if (points[npoints].time < lasttime) {
			printf("error in breakpoint data at point %lu: time not increasing\n", npoints + 1);
			break;
		}

		lasttime = points[npoints].time;

		if (++npoints == size) {
			BREAKPOINT* tmp;
			size += NPOINTS;
			tmp = (BREAKPOINT*) realloc(points, sizeof(BREAKPOINT) * size);
			if (tmp == NULL)	{
				// release the memory; return NULL to caller
				npoints = 0;
				free(points);
				points = NULL;
				break;		 
			}

			points = tmp;
		}			
	}
	if (npoints) *psize = npoints;	
	return points;
}

int main(int argc, char* argv[])
{	
	unsigned long	size;
	double dur;
	BREAKPOINT point, *points;
	FILE* fp;

	printf("breakdur: find duration of breakpoint file\n");
	if (argc < 2) {
		printf("usage: breakdur infile.txt \n");
		return 0;
	}

	fp = fopen(argv[1], "r");
	if (fp == NULL) {		
		return 0;
	}

	size = 0;
	points = get_breakpoints(fp, &size);
	if (points==NULL) {
		printf("No breakpoints read.\n");
		fclose(fp);
		return 1;
	}

	if (size < 2) {
		printf("Error: at least two breakpoints required\n");		
		free(points);
		fclose(fp);
		return 1;
	}	

	// breakpoints start from 0
	if (points[0].time != 0.0) {
		printf("Error in breakpoint data: first time must be 0.0\n");		
		free(points);
		fclose(fp);
		return 1;
	}

	printf("read %lu breakpoints\n", size);
	dur = points[size - 1].time;
	printf("duration: %f seconds\n", dur);
	point = maxpoint(points, size);

	printf("maximum value: %f at %f secs\n", point.value, point.time);

	print_points(points, size);

	// TODO: allow passing in normalize and scale factors
	scale_values(points, size, 0.85);
	scale_times(points, size, 2.0);

	free(points);
	fclose(fp);
	return 0;
}
