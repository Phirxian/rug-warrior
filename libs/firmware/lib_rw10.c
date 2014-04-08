/* lib_bb.c */

/*

   VERSION HISTORY FOR lib_bb.c LIBRARY FILE

   V2.41	Jun 6 1991			jrs
   implemented motor code for Bbot

   V2.42	Mon Jul 15 09:47:15 1991	fgm
   implemented PWM interface routines

*/   

int init_on_load()
{
    /* Functions to be called when this module is -loaded- */
   init_motors();
}

int _foobar= init_on_load();

void frob(int loc, int bits)
{
    while (1) {
	sleep(.1);
	bit_set  (loc, bits);
	sleep(.1);
	bit_clear(loc, bits);
    }
}
	
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
{
    tone(500., .3);
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



/*
 * example code using timers (twiddle your thumbs for 2.5 seconds)
 *
 {
   long timer1;
   timer1= timer_create_seconds(2.5);
   while (!timer_done(timer1)) {
     twiddle_my_thumbs();
     if (cow_jumped_over_the_moon()) {
       \* WOW! I guess I had better give myself two minutes *\
       timer1= timer_create_seconds(120.);
     }
   }
 }
 */

/***************************************************************** */
/*                                                                */
/* MOTOR PRIMITIVES                                               */
/*                                                                */
/* .	fd(n)	sets motor n to full on in the green direction    */
/* .	bk(n)	sets motor n to full on in the red direction      */
/* .	motor(n, s)	sets motor n on at speed s;               */
/* .			s= 100 is full on green,                  */
/* .			s= -100 is full on red,                   */
/* .			s= 0 is off.                              */
/* .	off(n)		turns off motor n.			  */
/*                                                                */
/* .	alloff()	turns off all motors.                     */
/* .	ao()		turns off all motors.                     */
/*                                                                */
/*                                                                */
/* .	motors are numbered 0 through 3.                          */
/***************************************************************** */

/* location of bit masks for PWM routine 	   */
/*                                                 */
/* internal speeds are bit masks                   */
/*                                                 */
/* speed0:  0x22                                   */
/* speed1:  0x23                                   */

int _speed_table[]= {
    		       0b00000000,	/* speed 0 */
		       0b00010001,
		       0b01001001,			 
		       0b01010101,
		       0b01010111,
		       0b01110111,
		       0b01111111,
    		       0b11111111	/* speed 1 */
		     };

int port_a= 0x1000;
int port_d= 0x1008;

void init_motors()
{
    bit_set(0x1026, 0b00001000);
    poke(0x1009, 0b00111110);
}

void fd(int m)
{
    motor(m, 100);
}

void bk(int m)
{	
    motor(m, -100);
}

void off(int m)
{
    motor(m, 0);
}

void alloff()
{
    ao();
}

void ao()
{
    off(0);
    off(1);
}

void motor(int m, int speed)
{
    if (m==0 || m==1) {
	if (speed >= 0) {
	    bit_set(port_d, (0b00010000 << m));  /* set direction */
	    poke(0x22+m, _speed_table[(speed+7)/14]);
	}
	else {
	    bit_clear(port_d, (0b00010000 << m)); /* set direction */
	    poke(0x22+m, _speed_table[(7-speed)/14]);
	}
    }
}

    
/*  upper six bits from digital input port;
    lower two bits from low two bits of 6811's PORTA  */
int digital(int port)
{
    if (port > 1)
      return 1 & (peek(0x7fff) >> port);
    else
      return 1 & (peek(0x1000) >> port);
}

int analog(int port)
{
    poke(0x1039, 0b10000000);
    poke(0x1030, port);
    return peek(0x1031);
}

/*************************/
/*** EXTRA TIMER PORTS ***/
/*************************/

void timer2(int on)
{
    if (on) 
      bit_set (0x1000, 0b01000000);
    else
      bit_clear (0x1000, 0b01000000);
      
}

void timer3(int on)
{
    if (on) 
      bit_set (0x1000, 0b00100000);
    else
      bit_clear (0x1000, 0b00100000);
      
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

/*  gives process that calls it one tick (0.001 sec)
    more to run before being swapped out   */

/* SUPERCEDED BY NEW PCODE INSTRUCTION
void defer()
{
    poke(0x0a, 1);
}
*/

/********************************/
/*** System Interrupt Control ***/
/********************************/

/*
  pulse width modulation control:
  if off, all motors run at full speed
  if on,  speed bytes are used to determine motor speed
*/
void system_pwm_on() {bit_set(0x39, 0b00000100);}
void system_pwm_off() {bit_clear(0x39, 0b00000100);}

