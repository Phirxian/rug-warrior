/* Theramin */

/* Compute the beeper frequency from the differene of the photo cells */

float freq(int left, int right)
{ int delta;
  float frq = 100.0;
  delta = left - right;
  frq = 2500.0 * (1.0 + ((float) delta) / ((float) max(left, right)));
  return frq;
}

float period_gain = 0.0005;

/* Compute the length of the pause between beeps from the sum of the
   photo cells */

float period(int left, int right)
  /* Protect against negative periods */
{ return (period_gain * (float) (max(0, (512 - (left + right)))));
}

void theremin()
{
  int left = 0;
  int right = 0;
  while (1)
    {
      left  = analog(photo_left);
      right = analog(photo_right);
      tone(freq(left, right),0.1);  /* Play a tone of a certain freq */
      sleep(period(left, right));   /*  then wait a while */
  }
}

void start_theremin()
{ printf("Theremin\n");
  start_process(theremin());
}
