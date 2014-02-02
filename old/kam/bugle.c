/* Bugle -- Connect bump switches to tones */

/* There are 12 steps in an octave.  Step 0 corresponds to A, 1 to A sharp,
   2 to B, and so on.  A common tuning is to let A = 440 Hz.  To compute the
   frequency of a given note we use: freq = 440 * (2 ^ (s / 12)), where
   s is the step.  Step 0 is the A below middle C, step 12 is the A above
   middle C.
 */

float octave = 440.0;

float  c_note = octave * (2.0 ^  3.0 / 12.0);
float  f_note = octave * (2.0 ^  8.0 / 12.0);
float  a_note = octave * (2.0 ^ 12.0 / 12.0);
float c1_note = octave * (2.0 ^ 15.0 / 12.0);
float f1_note = octave * (2.0 ^ 20.0 / 12.0);
float a1_note = octave * (2.0 ^ 24.0 / 12.0);
float c2_note = octave * (2.0 ^ 27.0 / 12.0);

/* Select an action based on which bumper switch is closed
Note played      -->   C   F   A   C1  F1  A1  C2  none
Switches closed  --> none  B  R&B  R  L&R   L  L&B LRB
bumper() returns -->      100 101 001 011 010  110 111
 */

void select_bumper()
{ int bpr = bumper();		/* Local Var for bumper contents */
  if      (bpr == 0b100)
    set_beeper_pitch(c_note);
  else if (bpr == 0b101)
    set_beeper_pitch(f_note);
  else if (bpr == 0b001)
    set_beeper_pitch(a_note);
  else if (bpr == 0b011)
    set_beeper_pitch(c1_note);
  else if (bpr == 0b010)
    set_beeper_pitch(f1_note);
  else if (bpr == 0b110)
    set_beeper_pitch(a1_note);
  else if (bpr == 0b111)
    set_beeper_pitch(c2_note);

  if (bpr !=0)			/* Turn beeper on if any switch is pressed */
    beeper_on();
  else				/* Turn beeper off otherwise */
    beeper_off();
}

void bugle()
{ printf("Bugle \n");
  while(1)
    { select_bumper();
    }
}

void start_bugle()
{ start_process(bugle());
}
