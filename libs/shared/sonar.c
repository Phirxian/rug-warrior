/* RugBat Software Driver

    RugBat uses line PA3 in two different ways.  Normally PA3 drives the
    piezo buzzer -- commands such as beep(); place a square wave of a
    certain frequency on PA3 to make the spaker sound.  RugBat's scanning
    servo uses PA3 to move the servo back and forth.  Normally, to use
    sonar you disable the beeper, to use the beeper you disable the sonar.

    Only one RugBat unit can be connected to Rug Warrior at a time because
    RugBat uses the PA1 and PA2 input capture lines.

    RugBat_addr:
      Bit 0: 1 Initiate sonar ping, 0 Reset sonar
      Bit 1: 1 Disable speaker, 0, Enable speaker
      Bit 2: 1 Disable PWM, 0, Enable PWM
*/

/*  CONSTANTS  */

int RugBat_jumper = 0;	/* Edit to select alternate jumper [0 - 3] */
int RugBat_addr = 0x4000 + (RugBat_jumper * 0x1000);

/*  Addresses of MC68HC11 internal registers used by RugBat */

int _tctl1 = 0x1020;
int _tctl2 = 0x1021;
int _tflg1 = 0x1023;
int _oc1m  = 0x100C;
int _oc1d  = 0x100D;
int _toc1  = 0x1016;
int _toc5  = 0x101E;
int _tic1  = 0x1010;
int _tic2  = 0x1012;

/*  SONAR  */

/*  Enable input capture of rising edge waveforms on lines PA1 and PA2.
 *  init_sonar() must be called before ping(). */

void init_sonar()			
{ bit_set(_tctl2,0b010100);	/* Use bit_set and bit_clear rather than */
  bit_clear(_tctl2,0b101000);	/* poke to avoid changing other tctl2 bits */
}

/*  Initiate a sonar pulse.  init_sonar must be called before a ping can 
 *  be sent.
 */

void ping()
{ poke(_tflg1,0b10);		/* Writing 1 clears echo received flag */
  bit_set(RugBat_addr,0b001);	/* Initiate a PING */
  sleep(0.030);			/* Wait 30 milliseconds for an echo */
  bit_clear(RugBat_addr,0b001);	/* Clear the echo line */
}

/*  Compute range in feet.  range() returns a meaningful result 
 *  only when called after ping().  If range() returns -1.0 then
 *  no valid echo was received.
 */

float range()
{ if ( (peek(_tflg1) & 0b10) == 0 )  /* If IC2 didn't see echo */
    return -1.0;	/* Let calling routine decide which flag */
  else			/*  indicates no echo */
    return 
       ((float) ((peekword(_tic2) - peekword(_tic1)) >> 1) * 0.000569); 
}

/* ************************************************************************
 * The following code is optional.  Copy this code only if you will use a
 * servo to position RugBat.
 * ************************************************************************
 */

/*  SERVO  */
				  
/* Routines to run the optional scanning servo */

int servo_pos = 0;           /* Global, current commanded servo pos */
float scan_delay = 0.01;     /* Wait between commanded positions */

/*  Make it possible for the servo to run.
 */

void enable_servo()
{  poke(RugBat_addr,0b010);	/* Speaker off, Sonar PWM on */
}

/*  Terminate servo action
 */

void disable_servo()
{  poke(RugBat_addr,0b100);	/* Speaker on, Sonar PWM off */
}

/*  Setup PA3 to produce the PWM signal for the servo.
 */

void init_servo()
{ enable_servo();
  bit_set(_tctl1,0b11);          /* OC5 sets pin PA3 */
  bit_set(_oc1m,0b1000);         /* OC1 affects PA3 */
  bit_clear(_oc1d,0b1000);       /* The OC1 effect is to clear PA3 */
  pokeword(_toc1,0x0000);        /* OC1 fires on TCNT overflow */
  pokeword(_toc5,0xFFFF);        /* On for maximum time */
}  

/*  Turn the servo off.
 */

void servo_off()
{ disable_servo();
  bit_clear(_tctl1,0b11);        /* OC5 won't bother PA3 */
  bit_clear(_oc1m,0b1000);       /* Neither will OC1 */
}


/* Empirical data for a Futaba S5102 servo: Min = 500 uSec, Max = 2450 */
int servo_center = 1475;
int servo_gain = 10;


/*  Set servo pules length to usec.
 */

void pwm_servo (int usec)
{ pokeword(_toc5,usec<<1);   /* Convert 1/2 uSec ticks to 1 uSec units */
}


/*  Set the servo to a position between -100 and +100.  Zero points forward.
 */

void set_servo (int pos)
{ pwm_servo(pos * servo_gain + servo_center);
}


/*  Make the servo scan back and forth between low_pos and high_pos
 *  Delay tells the servo how long to wait between steps
 */

void scan(int low_pos, int high_pos)
{
  int step = 1;    /* Standard step size */
  servo_pos = (low_pos + high_pos) / 2; /* Start can fail otherwise*/
  while (1)
    { if ((servo_pos < low_pos) || (servo_pos > high_pos))
        { sleep (3.0 * scan_delay);     /* Don't reverse instantly */
          step = (- step); }            
      servo_pos = servo_pos + step;
      set_servo(servo_pos);
      sleep(scan_delay);
    }}

/* AJOUT GUILHEM */
void main() {
	init_servo();
	
	printf("A1");
	scan(-100,100);
	printf("A2");
	sleep(5.0);
	
	
}
