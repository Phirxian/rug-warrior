/* lib_rw11.c */

/*
    VERSION HISTORY FOR lib_rw11.c

    V2.81	Jun 11 1994			jlj
    Created library for Rug Warrior from an earlier 
    library by jsr and fgm

*/

persistent int test_number;

/*******************/
/* TIME PRIMITIVES */
/*******************/

/***********************************/
/* location of various time stuff: */
/* 0x14: time in milliseconds      */
/***********************************/

void reset_system_time()
{
  pokeword(0x14, 0);
  pokeword(0x12, 0);
}
    
/*  returns valid time from 0 to 32,767 minutes (approx)  */
float seconds()
{
  return ((float) mseconds()) / 1000.;
}

void sleep(float seconds)
{ 
    msleep((long)(seconds * 1000.));
}

void msleep(long msec)
{
    long end_time= mseconds() + msec;

    while (1) {
	/* if the following test doesn't execute at least once a second,
	   msleep may not halt */
	long done= mseconds()-end_time;
	if (done >= 0L && done <= 1000L) break;
    }
}

void beep()
{   tone(500., .3);
  }

/*  1/2 cycle delay in .5us goes in 0x26 and 0x27  */ 
void tone(float frequency, float length)
{
    pokeword(0x26, (int)(1E6 / frequency));
    bit_set(0x1020, 0b00000001);
    sleep(length);
    bit_clear(0x1020, 0b00000001);

    /*  following is important to reduce # of interrupts
	when tone is off  */
    pokeword(0x26, 0);

    bit_clear(0x1000, 8);
}

void beeper_on()
{
    bit_set(0x1020, 0b00000001);
    bit_set(0x1022, 0b00001000);
}

void beeper_off()
{
    bit_clear(0x1022, 0b00001000);
    bit_clear(0x1020, 0b00000001);
    pokeword(0x26, 0);
    bit_clear(0x1000, 0b00001000);	/* turn power to spkr off */
}

void set_beeper_pitch(float frequency)
{
    pokeword(0x26, (int)(1E6 / frequency));
}    

long timer_create_mseconds(long timeout)
{
    return mseconds() + timeout;
}

long timer_create_seconds(float timeout)
{
    return mseconds() + (long) (timeout * 1000.);
}

int timer_done(long timer)
{
    return timer < mseconds();
}

/* Rug Warrior's analog ports 6 and 7 are unassigned */

int analog(int port)
{   poke(0x1039, 0b10000000);
    poke(0x1030, port);
    return peek(0x1031);
}

/*****************************/
/*** Multi-Tasking Support ***/
/*****************************/

/*  gives process that calls it 256 ticks (over 1/4 sec)
    more to run before being swapped out

    call repeatedly to hog processor indefinitely  */
void hog_processor()
{
    poke(0x0a, 0);
}

/* Debugging utilities */

/* Dump 8 bytes to the LCD screen, starting at addr */
void dump(int addr)
{ printf("%x: %x %x ", addr, peekword(addr), peekword(addr + 2));
  printf("      %x %x\n",  peekword(addr + 4), peekword(addr + 6));
}

/*****************************************************************************/
/* Rug Warrior specific functions and constants                              */
/*****************************************************************************/

/* Digital ports 1 and 2 are unassigned */

int digital(int port)		/* Return 1 bit from PA1 or PA2 (or PA0) */
{ return 1 & (peek(0x1000) >> (port & 3));
}

/* Indices for accessing sensors connected to the A/D converter.
   e.g. to read value of right photo cells use analog(photo_right) */

int photo_right = 0;
int photo_left = 1;
int microphone = 2;
int pyro = 5;

/******************************************************************************/
/*
/* Motor Control Primitives       
/*
/*    init_motors()		- Must be called to enable motors
/*    motor(index, speed)	- Control velocity of motor 0 (left) or 1 (right)
/*    drive(trans_vel, rot_vel)	- Control robot translation and rotation
/*    stop()			- Stop both motors
 */

/* Setup two PWM channels for motor control */

/*                 Left        Right            */
int TOCx[2]     = {0x1018,     0x101A};		/* Index for timer register */
int dir_mask[2] = {0b00010000, 0b00100000};	/* Port D direction bits */
int pwm_mask[2] = {0b01000000, 0b00100000};	/* Port A PWM bits */

int init_motors()
{ bit_set(0x1009,0b110000);	/* Set PD4,5 as outputs for motor direction */
  poke(0x100C,0b01100000);	/* OC1M Output compare 1 affects PA5,6 */
  bit_set(  0x1020,0b10100000);	/* TCTL1 OC3 turns off PA5, OC2 PA6 */
  bit_clear(0x1020,0b01010000);	/* Use set and clear to avoid other bits */
  pokeword(0x1018,0);		/* When TCNT = 0, OC1 fires */
}

/* Make sure init_motors is called after a reset */

int init_motors_dummy = init_motors();

void stop()			/* Stop both drive motors */
{ bit_clear(0x100D,pwm_mask[0]);	/* Let OC1 turn off motors rather */
  bit_clear(0x100D,pwm_mask[1]);	/* than turn them on */
}

/* Vel is in the range [-100, +100], index = 0 => Left, = 1 => Right */

void motor(int index, float vel)
{ float avel	;	/* Absolute value of velocity */
  if (vel > 0.0)
    { bit_set(0x1008, dir_mask[index]);	/* Forward rotation */
      avel = vel; }
  else
    { bit_clear(0x1008, dir_mask[index]); /* Backward rotation */
      avel = (- vel); }
  if (avel < 1.0)		/* If we are going real slow */
    bit_clear(0x100D,pwm_mask[index]);	/* then just have OC1 turn off the motor */
  else
    bit_set(0x100D,pwm_mask[index]);	/* Otherwise let OC1 control the motors */
  if (avel > 99.0)		/* If we are gong real fast */
    avel = 99.0;		/* don't let the following multiply overflow */
  pokeword(TOCx[index], (int) (655.56 * avel));	/* Compute TOCx value */
}

/* Use drive to control motion of the robot.  A positive rot_vel makes the robot
   turn left. */

void drive(float trans_vel, float rot_vel)
{ motor(0,trans_vel - rot_vel);
  motor(1,trans_vel + rot_vel);
}

/* Enable and activate debugging LEDs. */
void leds(int val)
{ poke(0x1009,0b111100);	/* Set port D for output */
  poke(0x1008,val << 2);	/* Shift number over */
}

/* Return a 3-bit value representing which of the bumper switches are closed */
int bumper()
{ int bmpr;
  bmpr = analog(3);			/* Switch closure: */
  if      (bmpr <  11) return 0b000;	/* none */
  else if (bmpr <  32) return 0b001;	/* A   */
  else if (bmpr <  53) return 0b010;	/*  B  */
  else if (bmpr <  74) return 0b011;	/* AB  */
  else if (bmpr <  96) return 0b100;	/*  C  */
  else if (bmpr < 117) return 0b101;	/* A C */
  else if (bmpr < 132) return 0b110;	/*  BC */
  else                 return 0b111;	/* ABC - (mechanically impossible) */
}

/* ir_detect returns:
     0b00 => no reflection, 0b01 => reflection on right,
     0b10 => reflection on left, 0b11 => reflection on both sides */

int ir_detect()
{ int val1, val2, val3;
  val1 = peek(0x100A) & 0b10000;	/* IR Detector connected to PE4 */
  bit_set(0x1008,0b1000);		/* Turn on Left emitter, PD3 */
  msleep(1L);				/* Wait 1 millisecond */
  val2 = peek(0x100A) & 0b10000;	/* Should be Low if signal detected */
  bit_clear(0x1008,0b1000);		/* Turn off Left emitter */
  bit_set(0x1008,0b0100);		/* Turn on Right emitter, PD2 */
  msleep(1L);				/* Wait 1 millisecond */
  val3 = peek(0x100A) & 0b10000;
  bit_clear(0x1008,0b1100);		/* Turn emitters off */
  /* For detection, detector must be high when emitter is off, low when on */
  return ((val1 & ~val2) >> 3)	| ((val1 & ~val3) >> 4); /* HI -> LOW */
}


