/**
 * reset button states
 */
persistent int test_number;

/**
 * restart the systeme timer
 * seconds() return 0 after this call
 */
void reset_system_time()
{
    pokeword(0x14, 0);
    pokeword(0x12, 0);
}

/**
 * returns valid time from 0 to 32,767 minutes (approx)
 */
float seconds()
{
    return ((float) mseconds()) / 1000.;
}

/**
 * yield the current process for @seconds
 */
void sleep(float seconds)
{
    msleep((long)(seconds * 1000.));
}

/**
 * yield the current process for @msec
 */
void msleep(long msec)
{
    long end_time= mseconds() + msec;

    while(1)
    {
        /* if the following test doesn't execute at least once a second,
           msleep may not halt */
        long done= mseconds()-end_time;

        if(done >= 0L && done <= 1000L) break;
    }
}

/**
 * Old freq was 500 for a periode of 0.3second
 */
void beep()
{
    tone(1000., .3);
}

/**
 * 1/2 cycle delay in .5us goes in 0x26 and 0x27
 */
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

/**
 * enable beeper output
 */
void beeper_on()
{
    bit_set(0x1020, 0b00000001);
    bit_set(0x1022, 0b00001000);
}

/**
 * disable beeper output
 */
void beeper_off()
{
    bit_clear(0x1022, 0b00001000);
    bit_clear(0x1020, 0b00000001);
    pokeword(0x26, 0);
    bit_clear(0x1000, 0b00001000);  /* turn power to spkr off */
}

/**
 * set the bepper tone to be frequency Hz
 * the subsequent function is then used to turn the bepper on
 */
void set_beeper_pitch(float frequency)
{
    pokeword(0x26, (int)(1E6 / frequency));
}

/**
 * return the current time (ms) increased by @timeout (ms)
 */
long timer_create_mseconds(long timeout)
{
    return mseconds() + timeout;
}

/**
 * return the current time (sec) increased by @timeout (ms)
 */
long timer_create_seconds(float timeout)
{
    return mseconds() + (long)(timeout * 1000.);
}

/**
 * check if the given (previous timer) are outdated
 */
int timer_done(long timer)
{
    return timer < mseconds();
}

/**
 * return the value of sensor port numbered @port
 * range in [0 - 255]
 */
int analog(int port)
{
    poke(0x1039, 0b10000000);
    poke(0x1030, port);
    return peek(0x1031);
}

/*****************************/
/*** Multi-Tasking Support ***/
/*****************************/

/**
 * gives process that calls it 256 ticks (over 1/4 sec)
 * more to run before being swapped out
 * call repeatedly to hog processor indefinitely
 */
void hog_processor()
{
    poke(0x0a, 0);
}

