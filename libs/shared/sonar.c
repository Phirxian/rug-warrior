/**
 * Edit to select alternate jumper [0 - 3]
 */
int RugBat_jumper = 0;

/**
 * RugBat adress acess
 */
int RugBat_addr = 0x4000 + (RugBat_jumper * 0x1000);

/**
 * Addresses of MC68HC11 internal registers used by RugBat
 */
int _tctl1 = 0x1020;

/**
 * Addresses of MC68HC11 internal registers used by RugBat
 */
int _tctl2 = 0x1021;

/**
 * Addresses of MC68HC11 internal registers used by RugBat
 */
int _tflg1 = 0x1023;

/**
 * Addresses of MC68HC11 internal registers used by RugBat
 */
int _oc1m  = 0x100C;

/**
 * Addresses of MC68HC11 internal registers used by RugBat
 */
int _oc1d  = 0x100D;

/**
 * Addresses of MC68HC11 internal registers used by RugBat
 */
int _toc1  = 0x1016;

/**
 * Addresses of MC68HC11 internal registers used by RugBat
 */
int _toc5  = 0x101E;

/**
 * Addresses of MC68HC11 internal registers used by RugBat
 */
int _tic1  = 0x1010;

/**
 * Addresses of MC68HC11 internal registers used by RugBat
 */
int _tic2  = 0x1012;

/**
 * Enable input capture of rising edge waveforms on lines PA1 and PA2.
 * init_sonar() must be called before ping().
 */
void init_sonar()
{
    beeper_off();
    bit_set(_tctl2,0b010100); /* Use bit_set and bit_clear rather than */
    bit_clear(_tctl2,0b101000);   /* poke to avoid changing other tctl2 bits */
}

/**
 * Initiate a sonar pulse.  init_sonar must be called before a ping can
 * be sent.
 */
void ping()
{
    poke(_tflg1,0b10);        /* Writing 1 clears echo received flag */
    bit_set(RugBat_addr,0b001);   /* Initiate a PING */
    sleep(0.030);         /* Wait 30 milliseconds for an echo */
    bit_clear(RugBat_addr,0b001); /* Clear the echo line */
}

/**
 * Compute range in feet.  range() returns a meaningful result
 * only when called after ping().  If range() returns -1.0 then
 * no valid echo was received.
 */
float range()
{
    if((peek(_tflg1) & 0b10) == 0)     /* If IC2 didn't see echo */
        return -1.0;    /* Let calling routine decide which flag */
    else          /*  indicates no echo */
        return
            ((float)((peekword(_tic2) - peekword(_tic1)) >> 1) * 0.000569);
}

/**
 * Global, current commanded servo pos
 */
int servo_pos = 0;

/**
 * Wait between commanded positions
 */
float scan_delay = 0.01;

/**
 * Make it possible for the servo to run.
 */
void sonar_enable_servo()
{
    poke(RugBat_addr,0b010); /* Speaker off, Sonar PWM on */
}

/**
 * Terminate servo action
 */
void sonar_disable_servo()
{
    poke(RugBat_addr,0b100); /* Speaker on, Sonar PWM off */
}

/**
 * Setup PA3 to produce the PWM signal for the servo.
 */
void sonar_init_servo()
{
    sonar_enable_servo();
    bit_set(_tctl1,0b11);          /* OC5 sets pin PA3 */
    bit_set(_oc1m,0b1000);         /* OC1 affects PA3 */
    bit_clear(_oc1d,0b1000);       /* The OC1 effect is to clear PA3 */
    pokeword(_toc1,0x0000);        /* OC1 fires on TCNT overflow */
    pokeword(_toc5,0xFFFF);        /* On for maximum time */
}

/**
 * Turn the servo off.
 */
void sonar_servo_off()
{
    sonar_disable_servo();
    bit_clear(_tctl1,0b11);        /* OC5 won't bother PA3 */
    bit_clear(_oc1m,0b1000);       /* Neither will OC1 */
}


/**
 * Empirical data for a Futaba S5102 servo: Min = 500 uSec, Max = 2450
 */
int servo_center = 1475;
int servo_gain = 10;

/**
 * Set servo pules length to usec.
 */
void pwm_servo(int usec)
{
    pokeword(_toc5,usec<<1);   /* Convert 1/2 uSec ticks to 1 uSec units */
}

/**
 * Set the servo to a position between -100 and +100.  Zero points forward.
 */
void set_servo(int pos)
{
    pwm_servo(pos * servo_gain + servo_center);
}

/**
 * Make the servo scan back and forth between low_pos and high_pos
 * Delay tells the servo how long to wait between steps
 */
void scan(int low_pos, int high_pos)
{
    /* Standard step size */
    int step = 1;

    /* Start can fail otherwise*/
    servo_pos = (low_pos + high_pos) / 2;

    while(1)
    {
        if((servo_pos < low_pos) || (servo_pos > high_pos))
        {
            /* Don't reverse instantly */
            sleep(3.0 * scan_delay);
            step = (- step);
        }

        servo_pos = servo_pos + step;
        set_servo(servo_pos);
        sleep(scan_delay);
    }
}

/**
 * Make the servo scan back and forth between low_pos and high_pos
 * Delay tells the servo how long to wait between steps
 * ADD : Print the actual position of the rugBat between -100 & 100
 *       And make a predetermined number of loop
 * @param nLoop - number of laps remaining (must be superior to 1)
 */
void scan_display_position(int low_pos, int high_pos, int nLoop)
{
    int step = 1;
    if(nLoop < 1)
    {
        return;
    }

    /* Standard step size */
    /* Start can fail otherwise*/
    servo_pos = (low_pos + high_pos) / 2;

    while(1)
    {
        if((servo_pos < low_pos) || (servo_pos > high_pos))
        {
            /* Don't reverse instantly */
            sleep(3.0 * scan_delay);
            step = (- step);
        }

        servo_pos = servo_pos + step;
        set_servo(servo_pos);
        printf("Position %d\n", servo_pos);
        sleep(scan_delay);

        if(servo_pos == 100)    /* A laps has been realized */
        {
            nLoop--;
        }

        if(nLoop == 0)    /* All loop are realized */
        {
            return;
        }
    }
}
