#include <stdio.h>
#include <math.h>

/**
 * Calculate/display frequency of MIDI note numbers from C0 (from Listing 1.2 in MIT Audio Bk)
 */
int main()
{
	double semitone_ratio;
	double c0;  // MIDI Note 0
	double c5;  // Middle C
	double frequency;
	int midinote;
	const char *notenames[] = {"C", "C# D♭", "D", "D# E♭", "E", "F", "F# G♭", "G",
		"G# A♭", "A", "A# B♭", "B"
	};

	// ratio between semitones in 12-tone equal-tempered scale
	semitone_ratio = pow(2.0, 1.0/12.0); // approx. 1.0594631

	// Middle C is three semitones above low A, which is 220Hz
	c5 = 220.0 * pow(semitone_ratio, 3);

	// MIDI Note 0 is C, 5 octaves below Middle C
	c0 = c5 * pow(0.5, 5.0);

	// calculate a frequency for a given MIDI Note Number */
	for (midinote = 0; midinote <= 127; midinote++) {
		frequency = c0 * pow(semitone_ratio, midinote);
		// print midinote #, notename octave: freqquency
		printf("%3d: %5s%2d: %13f Hz\n", midinote, notenames[midinote % 12], midinote/12, frequency);
	}

	return 0;
}
