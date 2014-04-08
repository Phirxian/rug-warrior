/*  Version 2.60-P  */

int photo_right = 0;

int photo_left = 1;

int microphone = 2;

/*******************************************/
/*** lib_r22.c                           ***/
/*** Randy Sargent, Fred Martin 1991-92  ***/
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
    tone(500., .3);
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
    _set_motor(motor, motor%2, 7);
}

void bk(int motor)
{
    _set_motor(motor, (motor%2)^1, 7);
}

void off(int motor)
{
    if (motor < 4)
      bit_clear(0x0e, 1 << (4 + motor));
    else if (motor == 4)
      bit_clear((int)&exp_motor_bits, 0b10000000);
    else if (motor == 5)
      bit_clear((int)&exp_motor_bits, 0b01000000);
}

void alloff()
{
    poke(0x0e, 0b00001010);
    bit_clear((int)&exp_motor_bits, 0b11000011);
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
    _set_motor(m, m%2, (speed + 3) / 14);
  else
    _set_motor(m, (m%2)^1, (-speed + 3) / 14);
}


void _set_motor(int motor, int dir, int speed)
{
    if (motor<6) {
        _motor_speed[motor]= speed;
        _set_motor_speeds();
    }
    if (motor < 4) {
        if (dir) {
            bit_set(0x0e, 0b10001 << motor);
        } else {
            bit_clear(0x0e, 1 << motor);
            bit_set(0x0e, 0b10000<<motor);
        }
    } else if (motor == 4) {
        if (dir) {
            bit_clear((int)&exp_motor_bits, 0b00010000);
            bit_set((int)&exp_motor_bits,   0b10100000);
        } else {
            bit_clear((int)&exp_motor_bits, 0b00100000);
            bit_set((int)&exp_motor_bits,   0b10010000);
        }
    } else if (motor == 5) {
        if (dir) {
            bit_clear((int)&exp_motor_bits, 0b00001000);
            bit_set((int)&exp_motor_bits,   0b01000100);
        } else {
            bit_clear((int)&exp_motor_bits, 0b00000100);
            bit_set((int)&exp_motor_bits,   0b01001000);
        }
    }
 }

void _set_motor_speeds()
{
    int speed01= (_speed_table[_motor_speed[0]] << 8)
      + _speed_table[_motor_speed[1]];
    int speed23= (_speed_table[_motor_speed[2]] << 8)
      + _speed_table[_motor_speed[3]];
    int speed45= (_speed_table[_motor_speed[5]] << 8)
      + _speed_table[_motor_speed[4]];

    pokeword(0x22, speed01);
    pokeword(0x24, speed23);
    exp_motor_speeds= speed45;
}

void led_out1(int s)
{
    if (s) bit_set((int)&exp_motor_bits, 1);
    else  bit_clear((int)&exp_motor_bits, 1);
}

void led_out0(int s)
{
    if (s) bit_set((int)&exp_motor_bits, 2);
    else  bit_clear((int)&exp_motor_bits, 2);
}

void motor4_left(int s)
{
    if (s) {
	bit_set  ((int)&exp_motor_bits, 0b10000000);
	bit_clear((int)&exp_motor_bits, 0b00010000);
	_motor_speed[4]= 7;
	exp_motor_speeds= (exp_motor_speeds&0xFF00) + 0xFF;
    } else
      bit_set((int)&exp_motor_bits, 0b00010000);
}

void motor4_right(int s)
{
    if (s) {
	bit_set  ((int)&exp_motor_bits, 0b10000000);
	bit_clear((int)&exp_motor_bits, 0b00100000);
	_motor_speed[4]= 7;
	exp_motor_speeds= (exp_motor_speeds&0xFF00) + 0xFF;
    } else
      bit_set((int)&exp_motor_bits, 0b00100000);
}

void motor5_left(int s)
{
    if (s) {
	bit_set  ((int)&exp_motor_bits, 0b01000000);
	bit_clear((int)&exp_motor_bits, 0b00000100);
	_motor_speed[5]= 7;
	exp_motor_speeds= (exp_motor_speeds&0xFF) + 0xFF00;
    } else
      bit_set((int)&exp_motor_bits, 0b00000100);
}

void motor5_right(int s)
{
    if (s) {
	bit_set  ((int)&exp_motor_bits, 0b01000000);
	bit_clear((int)&exp_motor_bits, 0b00001000);
	_motor_speed[5]= 7;
	exp_motor_speeds= (exp_motor_speeds&0xFF) + 0xFF00;
    } else
      bit_set((int)&exp_motor_bits, 0b00001000);
}

/*********************/
/*** SENSOR INPUTS ***/
/*********************/

int choose_button()
{
    return ! (2 & (peek(0x7fff)));
}

int escape_button()
{
    return !(1 & (peek(0x7fff)));
}

/* for compatibility */
int right_button() {return escape_button();}
int left_button() {return choose_button();}

/*  DIP switches:  compendium of thresholded dip_switch() calls */
/*  DIP switches are numbered from 1 (highest bit) to 4 (lowest bit)  */

int dip_switches()
{
  int i,value;
  for(i=28,value=0;i<32;i++)
    value=(value<<1) | !(read_analog(i)<128);
  return value;
}

int dip_switch(int which)
{
  return(!(read_analog(27+which)<128));
}

int analog(int port)
{
    if (port<8) /* accessing a hardware digital port */
      if (digital(port)) return 0;
      else return 255;
    if (port<12) /* accessing direct 6811 analog port */
      return analog_header_port(port);
    if (port<36) /* accessing analog port on an analog mux */
      return(read_analog(port));
    else {/* out of range */
	printf("Analog port out of range\n");
	beep();
	return -1;}
}

int digital(int port)
{
    if (port==0) /* this is first bit of PORTA */
      return !(peek(0x1000)&1);
    if (port==1) /* this is the second bit of PORTA */
      return !(peek(0x1000)&2);
    if (port<8) /* normal bit of 7fff as gotten from digital chip */
      return !((peek(0x7fff)>>port)&1);
    if(port<36)
      return(read_analog(port)<127);
    else {
	printf("Digital port outof range\n");
	beep();
	return -1;
    }
}

int motor_force(int port)
{
    return analog_header_port(port+4);
}

int frob_knob()
{
    return read_analog(32);
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















