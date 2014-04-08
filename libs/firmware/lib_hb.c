/*  for Handy Board v1.2 */

/* 
**  Fred Martin
**  Wed Dec  6 23:49:04 1995
**
*/


/*******************************************/
/*** lib_r22.c                           ***/
/*** Fred Martin, Randy Sargent 1991-95  ***/
/*******************************************/

/*

    VERSION HISTORY

    V2.52
    many sbot primitives installed
    
    V2.60	Fri Oct 11 1991		fgm
    wrote "*_ir_distance" routines (in conjunction with sb_ir.icb)

    V2.60-B	Mon Oct 14 12:04:56 1991	fgm
    modified "*_ir_distance" routines such that maximum repetition
    rate is 50 Hz.  Should fix reports that they didn't work.
    
    V2.60-C	Wed Oct 16 15:36:30 1991    rs
    added left_pyro, right_pyro
    
    V2.60-D	Thu Oct 17 04:11:42 1991    rs
    added ir_distance_strength

    V2.60-E	Fri Oct 18 14:20:58 1991	fgm
    added battery_voltage, battery_level

    V2.60-F	Wed Oct 23 01:03:21 1991	rs
    wrote more accurate battery_voltage

    V2.60-G	Sun Oct 27 11:42:49 1991	rs
    wrote reflectance sensor primitives

    V2.60-H	Sun Oct 27 16:34:10 1991	fgm
    installed velocity sensing, control routines
    modified `alloff' to perform zero velocity setting
        when velocity control is enabled
    modified beeper routines so that beeper_off
        doesn't reset pitch

    V2.60-I	Wed Jan  8 13:23:31 1992	fgm
    started changes for Rev 2.1 Board

    V2.60-J	Wed Jan  8 13:23:31 1992	rs
    fixed escape vs. choose button 

    V2.60-K     Sat Jan 11 03:17:55 1992        arw
    fixed dip_switches, implemented analog and digital functions

    V2.60-L	Sat Jan 11 11:41:25 1992	fgm
    added analog port 8-11 to analog and digital;
    fixed bug that analog of a digital port returned inverted value

    V2.60-M	Tue Jan 14 17:41:51 1992	fgm
    installed digital port 1 from port A.  Board requires fix.
    fixed bug in analog that would not allow use of port 27
    put error test at end of digital routine to increase efficiency

    V2.60-N	Fri Jan 17 15:10:06 1992	fgm
    Installed routines for motors 4 and 5 on the expansion board.
    Simplified motor speed correlation so that if motor ports
    are to be used in parallel, parallel motors should be wired
    to a single motor driver chip.

    V2.60-O	Wed Jan 22 13:09:08 1992	fgm
    removed old "ir_on" and "ir_off" routines
    fixed bugs in unidirectional motor routines

    V2.60-P	Wed Jan 29 00:58:37 1992	fgm
    fixed lingering bug in speed array for unidir motor routines
    
    V2.60-Q     Sun Jan 16 21:06:06 EST 1994    mld
    preventa runtime error 03 when using motor() with values above 100
    or below -100
*/


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

/*
    mseconds() is now a C primitive that returns type <long>
*/

/*  returns time since reset or reset_system_time in seconds */
float seconds()
{
    return ((float) mseconds()) / 1000.;
}
void sleep(float seconds)
{
    msleep((long)(int)(seconds * 1000.));
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
{
    tone(500., .1);
}

/*  1/2 cycle delay in .5us goes in 0x26 and 0x27  */ 
void tone(float frequency, float length)
{
    set_beeper_pitch(frequency);
    beeper_on();
    sleep(length);
    beeper_off();
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
    bit_clear(0x1000, 0b00001000);	/* turn power to spkr off */
}

void set_beeper_pitch(float frequency)
{
    pokeword(0x26, (int)(1E6 / frequency));
}    
    
/***************************************************************** */
/*                                                                */
/* MOTOR PRIMITIVES                                               */
/*                                                                */
/*  	fd(n)	sets motor n to full on in the green direction    */
/*  	bk(n)	sets motor n to full on in the red direction      */
/*  	motor(n, s)	sets motor n on at speed s;               */
/*  			s= 100 is full on green,                  */
/*  			s= -100 is full on red,                   */
/*  			s= 0 is off                               */
/*  	off(n)		turns off motor n 			  */
/*                                                                */
/*  	alloff()	turns off all motors                      */
/*  	ao()		turns off all motors                      */
/*                                                                */
/*                                                                */
/*  	motors are numbered 0 through 3.                          */
/***************************************************************** */

/************************************************* */
/* location of various motor stuff for PWM routine */
/*                                                 */
/* poke byte into "motor" to be output by PWM	   */
/* motor:   0x0e                                   */
/*      low bits of "motor" determine directions;  */
/*      high bits determine on/off state.	   */ 
/*                                                 */
/* internal speeds are bit masks                   */
/*                                                 */
/* speed0:  0x22                                   */
/* speed1:  0x23                                   */
/* speed2:  0x24                                   */
/* speed3:  0x25                                   */
/************************************************* */

int _motor_speed[]= {7, 7, 7, 7, 7, 7};	/* init to full on */

int _speed_table[]= {
		       0b00000000,	/* speed 0 */
		       0b00010001,
		       0b01001001,			 
		       0b01010101,
		       0b01010111,
		       0b01110111,
		       0b01111111,
    		       0b11111111	/* speed 7 */
		     };

void fd(int motor)
{
    _set_motor(motor, 0, 7);
}

void bk(int motor)
{	
    _set_motor(motor, 1, 7);
}

void off(int motor)
{
    bit_clear(0x0e, 1 << (4 + motor));
}

void alloff()
{
    poke(0x0e, 0b0000000);
}

void ao()
{
    alloff();
}

void motor(int m, int speed)
{
  if (speed>100) speed=100;
  if (speed<-100) speed=-100;
  if (speed >= 0)
    _set_motor(m, 0, (speed + 3) / 14);
  else
    _set_motor(m, 1, (-speed + 3) / 14);
}


void _set_motor(int motor, int dir, int speed)
{
    bit_set(0x0e, 1 << (4 + motor));	/* turn motor on */
    if (dir)
	bit_set(0x0e, 1 << motor);	/* set direction for backward */
    else
	bit_clear(0x0e, 1 << motor);	/* set dir for forward */
    _motor_speed[motor]= speed;
    _set_motor_speeds();
}

void _set_motor_speeds()
{
    int speed01= (_speed_table[_motor_speed[0]] << 8)
      + _speed_table[_motor_speed[1]];
    int speed23= (_speed_table[_motor_speed[2]] << 8)
      + _speed_table[_motor_speed[3]];

    pokeword(0x22, speed01);
    pokeword(0x24, speed23);
}

/*********************/
/*** SENSOR INPUTS ***/
/*********************/

int stop_button()
{
    return ! (0x40 & (peek(0x7fff)));
}

int start_button()
{
    return ! (0x80 & (peek(0x7fff)));
}

void start_press()
{
    while (!start_button());
    while (start_button());
    beep();
}

void stop_press()
{
    while (!stop_button());
    while (stop_button());
    beep();
}

int analog(int port)
{
    if (port < 7) 
    {
    	return _raw_analog(port);
    }
    else if (port < 16)
    {
    	return 255 * !digital(port);
    }
    else
    {
    	printf("Analog port out of range\n");
    	beep();
    	return -1;
    }
}

int _raw_analog(int port)
{
    poke(0x1039, 0b10000000);
    poke(0x1030, port);
    return peek(0x1031);
}
    
int digital(int port)
{
    if (port < 7) /* analogs */
    	return analog(port) < 255;
    if (port == 7) /* TIC1 */
    	return !(peek(0x1000) & 1);
    if (port == 8) /* TIC2 */
    	return !(peek(0x1000) & 2);
    if (port == 9) /* PAI */
    	return !(peek(0x1000) & 128);
    if (port < 16) /* normal bit of 7fff as gotten from digital chip */
      return !((peek(0x7fff) >> (port - 10)) & 1);
    else {
	printf("Digital port outof range\n");
	beep();
	return -1;
    }
}

int knob()
{
    return _raw_analog(7);
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

/*  defer is now a C primitive  */


/********************************/
/*** System Interrupt Control ***/
/********************************/

/*

    These functions allow you to turn on and off various features
    controlled by the system interrupt routines.  The more features
    you turn off, the faster your code will run.

    On reset, the features have the following state:
       pulse width modulation ON
       infrared decoding      ON
       LCD printing           ON
       quad shaft decoding    OFF
       IR tranmission	      OFF

    This uses approx. 30% of total CPU time.

    Approximate benchmarks:

x    Feature		% of CPU
    -------		--------
    PWM			    3
    IR decode		   11
    LCD printing (active)   8
    LCD printing (inactive) 1
    quad shaft decode	    5
    IR transmission	    1
    
*/    

/*
  pulse width modulation control:
  if off, all motors run at full speed
  if on,  speed bytes are used to determine motor speed
*/
void system_pwm_on() {bit_set(0x39, 0b00000100);}
void system_pwm_off() {bit_clear(0x39, 0b00000100);}

/*
  printing to the LCD
  WARNING:  printf's will wedge once the print buffer becomes full
    if system printing is disabled  */
void system_print_on() {bit_set(0x39, 0b00000001);}
void system_print_off() {bit_clear(0x39, 0b00000001);}


/* random numbers from peeking at 2 MHZ system clock */
/* input from 2 to 32767 */
int random(int mod)
{
    return (peekword(0x100e) & 0x7fff) % mod;
}













