/************************************************************************/
/*                                  */
/* "servo.c" for the 6.270 'bot                         */
/*  servo support code                      */
/* by Anne Wright                           */
/* Sat Jan 11                               */
/*                                  */
/************************************************************************/

/* The values MIN_SERVO_WAVETIME and MAX_SERVO_WAVETIME given here can
   only be approximate because the appropriate range depends on the servo.
   These values should be conservative, and therefore may not allow the
   full range of movement from your servo.

   To customize this for a given servo, set the value of MIN_SERVO_WAVETIME
   to 0, and MAX_SERVO_WAVETIME to 10000 at the IC interaction prompt.
   Then call servo(x); at the prompt where x gets slowly smaller until
   you find the lower limit.  You can tell when it hits the limit because
   the body will heat up and it will buzz.

   The appropriate value of MIN_SERVO_WAVETIME is the smallest value of x
   which does not cause the servo to heat up.  Set it to this value in
   your initialization routine.

   Servos are usually able to go a little more than 180 degrees.  The
   accuracy of the servo_rad and servo_deg routines depend on the
   position set by MAX_SERVO_WAVETIME being close to 180 degrees away
   from the position set by MIN_SERVO_WAVETIME.  If you want those
   routines to be accurate, call servo(x); to get 180 degrees off.  If
   instead you want the maximum possible excursion, follow the same
   routine as above to find the largest value of x which does not
   cause the servo to heat up.  Again, set MAX_SERVO_WAVETIME in your
   initialization routine to the value you choose.

   Note that floating point is not very efficient, so it is better to
   use integers when you can.  Therefore it is most efficient to use
   the integer pulse widths directly rather than to call servo_rad and
   servo_deg.  You can use the conversion functions radian_to_pulse or
   degree_to_pulse to initially calculate the appropriate values, then
   use those integers.
*/

int MIN_SERVO_WAVETIME = 1400;
int MAX_SERVO_WAVETIME = 4530;

#define SERVO_RANGE (MAX_SERVO_WAVETIME-MIN_SERVO_WAVETIME)

#define rexcursion 3.14159
#define dexcursion 180.

void servo_on()
{
    asm_servo_on(0);
}

void servo_off()
{
    asm_servo_off(0);
}
/************************************************************************/
/* Servo movement commands                                              */
int servo(int period) /* argument in clock cycles of pulse, moves servo */
{
    if(period>MAX_SERVO_WAVETIME)
        return (servo_pulse_wavetime=MAX_SERVO_WAVETIME);
    else if(period<MIN_SERVO_WAVETIME)
        return (servo_pulse_wavetime=MIN_SERVO_WAVETIME);
    else
        return(servo_pulse_wavetime=period);
}

int servo_rad(float angle) /* argument in radians, moves servo */
{
    return servo(radian_to_pulse(angle));
}

int servo_deg(float angle) /* argument in degrees, moves servo */
{
    return servo(degree_to_pulse(angle));
}

/************************************************************************/
/* Pulse width calculations                                             */
int radian_to_pulse(float angle) /* argument in radians, returns pulse width */
{
    return ((int)(angle*((float)SERVO_RANGE)/rexcursion)+MIN_SERVO_WAVETIME);
}

int degree_to_pulse(float angle) /* argument in degrees, returns pulse width */
{
    return ((int)((angle*((float)SERVO_RANGE))/dexcursion)+MIN_SERVO_WAVETIME);
}
