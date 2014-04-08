/* Sonic commander -- steer the robot with sound */

/* The value for snc_sound_delta must be chosen carefully because the microphone
   tends to hear the sound of the motors. */

int snc_sound_delta = 80;		/* Pick a large number, then make a LOUD sound */
long snc_hold_time = 100L;		/* Numer of milliseconds to hold high time */
int snc_loud_p = 0;			/* 0 => quite, 1 => loud */
int snc_sound_count = 0;

int snc_bmpr = 0;			/* Escape process/analog conflict problem */

void snc_sample_sound()
{
  int s_level = 0;		/* Instantaneous sound level */
  long go_low_time = 0L;
  long current_time = 0L;
  while (1)
    { snc_bmpr = bumper();		/* Must have all analog calls in one process */
      current_time = mseconds();
      s_level = abs(analog(microphone) - 128);	/* Abs difference from 128 */\
      if ( s_level > snc_sound_delta)
	go_low_time = current_time + snc_hold_time;	/* Compute new go low time */
      if (current_time > go_low_time)
	snc_loud_p = 0;
      else
	{ if (snc_loud_p == 0)	/* Was 0, now 1 => add to transition count */
	    snc_sound_count++;
	  snc_loud_p = 1; }
    }}


/* Count the number of low high transitions during some interval.
   Start a timer at the first low high transition, then keep checking for
   more low high transitions until time runs out */

int sonic_cmd = 0;
float snc_cmd_period = 1.2;	/* This is how long we get to gather the sound */

void snc_capture_command()
{ int old_count = 0;
  while(1)
    { if (old_count != snc_sound_count)
	{ sonic_cmd = 1;	/* Stop immediately, while waiting for next cmd */
	  sleep(snc_cmd_period);	/* Wait for the sounds to happen */
	  sonic_cmd = snc_sound_count;
	  snc_sound_count = 0; }
    }}

/* Obey the sonic commands:
     1 - Stop
     2 - Turn
     3 - Forward 
   Stop in the event of a collision */

void sonic_control()
{ while(1)
    { if (snc_bmpr & 0b011)	/* Bump L or R */
	{ drive(-75,0);		/* Backup */
	  sleep(0.5);
	  sonic_cmd = 0; }
      else if (sonic_cmd == 2)	/* Spin in place */
	drive(0,75);
      else if (sonic_cmd == 3)	/* Forward */
	drive(75,0);
      else
	drive(0,0);		/* Stop by default */
    }}

/* 0123456789ABCDEF
   Sonic Commander 
     Cmd: Forward @
          Turn   
	  Stop   */

void snc_rpt()
{ while(1) /* 0123456789ABCDEF */
    { printf("Sonic Commander Cmd: ");
      if (snc_bmpr & 0b011)
        printf("Backup");
      else if (sonic_cmd == 2)
        printf("Turn  ");
      else if (sonic_cmd == 3)
        printf("Forward");
      else
        printf("Stop  ");
      printf("\n");
      sleep(0.5);
    }}

void start_sonic()
{ start_process(snc_sample_sound());
  start_process(snc_capture_command(),1);
  start_process(sonic_control(),1);
  start_process(snc_rpt(),1);
}
