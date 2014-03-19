/**
 * Indices for accessing sensors connected to the A/D converter.
 *  e.g. to read value of right photo cells use analog(photo_right)
 */
int photo_right = 0;

/**
 * Indices for accessing sensors connected to the A/D converter.
 *  e.g. to read value of left photo cells use analog(photo_left)
 */
int photo_left = 1;


/**
 * Indices for accessing sensors connected to the A/D converter.
 *  e.g. to read value of microphone sensors use analog(microphone)
 */
int microphone = 2;

/**
 * Motor Control Primitives
 * Index for timer register
 */
int TOCx[2]     = {0x1018,     0x101A};

/**
 * Motor Control Primitives
 * Port D direction bits
 */
int dir_mask[2] = {0b00010000, 0b00100000};

/**
 * Motor Control Primitives
 * Port A PWM bits
 */
int pwm_mask[2] = {0b01000000, 0b00100000};

/**
 * Make sure init_motors is called after a reset
 */
int init_motors_dummy = init_motors();

/**
 * Digital ports 1 and 2 are unassigned,
 * digital ports 3,4,5,6 are outputs
 * Return a bit from any of the port A line
 */
int digital(int port)
{
    return 1 & (peek(0x1000) >> (port & 7));
}

/**
 * Return state of left shaft encoder
 */
int left_shaft()
{
    return (peek(0x1000) & 0b00000001);
}

/**
 * Return state of right shaft encoder
 */
int right_shaft()
{
    return (peek(0x1000) & 0b10000000 && 1);
}

/**
 * Motor Control Primitives
 * Must be called to enable motors
 */
int init_motors()
{
    bit_set(0x1009,0b110000); /* Set PD4,5 as outputs for motor direction */
    poke(0x100C,0b01100000);  /* OC1M Output compare 1 affects PA5,6 */
    bit_set(0x1020,0b10100000);     /* TCTL1 OC3 turns off PA5, OC2 PA6 */
    bit_clear(0x1020,0b01010000); /* Use set and clear to avoid other bits */
    pokeword(0x1016,0);       /* When TCNT = 0, OC1 fires */
}

/**
 * Stop both drive motors
 */
void stop()
{
    bit_clear(0x100D,pwm_mask[0]);    /* Let OC1 turn off motors rather */
    bit_clear(0x100D,pwm_mask[1]);    /* than turn them on */
}

/**
 * Vel is in the range [-100, +100], index = 0 => Left, = 1 => Right
 */
void motor(int index, int vel)
{
    int avel = vel;

    if(vel == 0)               /* Check first for zero vel */
    {
        bit_clear(0x100D,pwm_mask[index]); /*  OC1 turns off the motor */
        return;
    }              /* And, you're done */

    bit_set(0x100D,pwm_mask[index]);   /* Otherwise let OC1 control the motors */

    if(vel > 0)
    {
        bit_set(0x1008, dir_mask[index]);   /* Forward rotation */
    }
    else
    {
        avel = (- vel);
        bit_clear(0x1008, dir_mask[index]);
    } /* Backward rotation */

    if(avel > 99) pokeword(TOCx[index], 0);
    else pokeword(TOCx[index], (655 * avel));
}

/**
 * Use drive to control motion of the robot.
 * A positive rot_vel makes the robot
 * turn left.
 */
void drive(int trans_vel, int rot_vel)
{
    motor(0,trans_vel - rot_vel);
    motor(1,trans_vel + rot_vel);
}

/**
 * Return a 3-bit value representing which of the bumper switches are closed
 */
int bumper()
{
    int bmpr;
    bmpr = analog(3);                     /* Switch closure: */

    if(bmpr <  11) return 0b000;          /* none */
    else if(bmpr <  32) return 0b001;     /* A   */
    else if(bmpr <  53) return 0b010;     /*  B  */
    else if(bmpr <  74) return 0b011;     /* AB  */
    else if(bmpr <  96) return 0b100;     /*  C  */
    else if(bmpr < 117) return 0b101;     /* A C */
    else if(bmpr < 138) return 0b110;     /*  BC */
    else                return 0b111;     /* ABC - (mechanically impossible) */
}

/**
 * ir_detect returns:
 * 0b00 => no reflection, 0b01 => reflection on right,
 * 0b10 => reflection on left, 0b11 => reflection on both sides
 */
int ir_detect()
{
    int val1, val2, val3;
    val1 = peek(0x100A) & 0b10000;    /* IR Detector connected to PE4 */
    bit_set(0x1008,0b0100);       /* Turn on Left emitter, PD2 */
    msleep(1L);               /* Wait 1 millisecond */
    val2 = peek(0x100A) & 0b10000;    /* Should be Low if signal detected */
    bit_clear(0x1008,0b0100);     /* Turn off Left emitter */
    bit_set(0x1008,0b1000);       /* Turn on Right emitter, PD3 */
    msleep(1L);               /* Wait 1 millisecond */
    val3 = peek(0x100A) & 0b10000;
    bit_clear(0x1008,0b1100);     /* Turn emitters off */
    /* For detection, detector must be high when emitter is off, low when on */
    return ((val1 & ~val2) >> 3)  | ((val1 & ~val3) >> 4); /* HI -> LOW */
}
