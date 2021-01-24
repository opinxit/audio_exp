// iscale.c from listing 1.6.10
// generate  E.T tables for N-notes to the octave (N <= 24)
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/** usage [-m][-i] iscale N startval [outfile.txt]
  -m: sets format of startval as MIDI note (default: freq, Hz)
	-i: prints the calculated interval as well as the absolute frequency
	outfile: write data to textfile
*/

/**
 * usage [-m][-i] iscale N startval [outfile.txt]
 * -m: sets format of startval as MIDI note (default: freq, Hz)
 * -i: prints the calculated interval as well as the absolute frequency
 * outfile: write data to textfile
 */
int main(int argc, char* argv[])
{
	FILE *fp;
	double startval, basefreq, ratio;
	double intervals[25];
	int notes;
	int ismidi = 0;
	int write_interval = 0;
	int i, err = 0;

	// check first arg for flag option: argc has to be at least 2
	while (argc > 1) {
		if (argv[1][0] == '-') {
			if (argv[1][1] == 'm') {
				ismidi = 1;
			} else if (argv[1][1] == 'i') {
				write_interval = 1;
			} else {
				printf("error: unrecognised flag option %s\n", argv[1]);
				return 1;
			}
			/* step up to next arg */
			argc--;
			argv++;
		} else {
			break;
		}
	}

	if (argc < 3) {
		printf("insufficient arguments\n");
		printf("Usage: iscale [-m] [-i] N startval [outfile.txt]\n");
		return 1;
	}

	// read and check all arguments
  // argv[1] now holds N, argv[2] holds startval

	notes  = atoi(argv[1]);
	if (notes < 1 || notes > 24) {
		printf("error: N out of range. Must be between 1 and 24.\n");
		return 1;
	}

	startval = atof(argv[2]);
	if (ismidi) {
		if (startval > 127.0) {
			printf("error: MIDI startval must be <= 127.\n");
			return 1;
		}

		// startval starts at 0 for MIDI
		if (startval < 0.0) {
		    printf("error: MIDI startval must be >= 0.\n");
			return 1;
		}
	} else { 
    // freq must be positive number
		if (startval <= 0.0) {
			printf("error: frequency startval must be positive.\n");
			return 1;
		}
	}

	// check for optional filename
	fp = NULL;

	if (argc == 4) {
		fp = fopen(argv[3], "w");
		if (fp == NULL) {
			printf("WARNING: unable to create file %s\n", argv[3]);
			perror("");
		}
	}

	// fill array and write to file if created

	// find basefreq, if val is MIDI
	if (ismidi) {
		double c0,c5;
		// find base MIDI note
		ratio = pow(2.0, 1.0 / 12.0);
		c5 = 220.0 * pow(ratio, 3);
		c0 = c5 * pow(0.5, 5);
		basefreq = c0 * pow(ratio, startval);
	} else {
		basefreq = startval;
	}

	// calc ratio from notes, fill array
	ratio = pow(2.0, 1.0/notes);
	for (i=0; i <= notes; i++) {
		intervals[i] = basefreq;
		basefreq *= ratio;
	}

	// read array, write to screen; optionally to file
	for (i=0; i <= notes; i++) {
		double nextfreq = intervals[i];
		double noteratio = pow(ratio, (double) i);

		if (write_interval) {
			printf("%d:\t%f\t%f\n", i, noteratio, nextfreq);
		} else {
			printf("%d:\t%f\n", i, nextfreq);
		}
  
		if (fp) {
			if (write_interval) {
				err =  fprintf(fp, "%d:\t%f\t%f\n", i, noteratio, nextfreq);
			}
			else {
				err = fprintf(fp, "%d:\t%f\n", i, nextfreq);
			}

			if (err < 0) {
				break;
			}
		}
	}

	if (err < 0) {
		perror("There was an error writing the file.\n");
	}

	if (fp) {
		fclose(fp);
	}

	return 0;
}