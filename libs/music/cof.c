/* Well tempered scale:
 a   1.000, a#  1.070,  b  1.143,  c  1.218,  c#  1.295,  d  1.375,  d#   1.457,
 e   1.542, f   1.628,  f  1.718,  g  1.810,  g#  1.904,  A   2.000
*/

/**
 * Fundamental freq of this octave.
 * Ohters: 220, 440, 880, 1760
 */
float octave = 440.0;

/**
 * tonne for piezo test
 */
float a0 = octave * 1.000;

/**
 * tonne for piezo test
 */
float b0 = (octave * 1.143);

/**
 * tonne for piezo test
 */
float c  = (octave * 1.218);

/**
 * tonne for piezo test
 */
float d  = (octave * 1.375);

/**
 * tonne for piezo test
 */
float e  = (octave * 1.542);

/**
 * tonne for piezo test
 */
float f  = (octave * 1.628);

/**
 * tonne for piezo test
 */
float g  = (octave * 1.810);

/**
 * tonne for piezo test
 */
float a  = (octave * 2.000);

/**
 * tonne for piezo test
 */
float b  = (octave * 2.0 * 1.143);

/**
 * tonne for piezo test
 */
float c2 = (octave * 2.0 * 1.218);

/**
 * Quarter note
 */
float q = 0.25;

/**
 * Half note
 */
float h = (2.0 * q);

/**
 * Dotted half
 */
float dh = (3.0 * q);

/**
 * Global chooses whether to display note or not
 */
int note_display = 0;

/**
 * Play a note with given frequency & time
 * can print as display with note_display > 0
 */
void note(float freq, float dur)
{
    if(note_display)
        printf("Piezo Test      freq%f\n", freq);

    tone(freq,dur);
}

/**
 * Play a basic music "piezo" for test issue
 */
void cof()
{
    note(c,q);
    note(f,q);
    note(g,q);
    note(a,q);
    note(g,dh);
    note(e,5.0 * q);
    note(c,q);
    note(f,q);
    note(g,q);
    note(a,q);
    note(g,8.0 * q);
    note(c,q);
    note(f,q);
    note(g,q);
    note(a,q);
    note(g,dh);
    note(e,5.0 * q);
    note(g,q);
    note(f,q);
    note(e,q);
    note(c,q);
    note(c,5.0 * q);
    note(c2,q);
    note(b,q);
    note(a,q);
    note(g,q);
    note(b,h);
    note(g,q);
    note(a,h);
    note(f,q);
    note(g,h);
    note(c2,q);
    note(b,q);
    note(a,q);
    note(g,q);
    note(b,8.0 * q);
    note(c2,q);
    note(b,q);
    note(a,q);
    note(g,q);
    note(b,h);
    note(g,q);
    note(a,h);
    note(f,q);
    note(g,h);
    note(g,q);
    note(f,q);
    note(e,q);
    note(c,q);
    note(c,7.0 * q);
}

