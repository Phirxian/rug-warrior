/* Code to run the BookBot.  This should ultimately exercise */
/* all the sensors and acturators */

/* Future enhancements:
       Continuous sound calibration -- Keep a running average of the noise detected
            by the mic, a spike above the average triggers the loud_sound module

*/

int T = 1;
int NIL = 0;

int STOP       = 0;
int FORWARD    = 1;
int BACKWARD   = 2;
int LEFT_TURN  = 3;
int RIGHT_TURN = 4;
int LEFT_ARC   = 5;
int RIGHT_ARC  = 6;

int cruise_command = FORWARD;
int photo_command = STOP;
int ir_command = STOP;
int bump_command = STOP;
/* Experimental data, 9 volts, concrete */
int msec_per_rev = 1071; /* 714; /* mSeconds to make 1 revolution */
int msec_per_rad = 170;  /* 113; /* mSeconds to move 1 robot radius */
int rev_2 = msec_per_rev / 2;   /* 180 degrees */
int rev_4 = msec_per_rev / 4;   /* 90 degrees */
int rev_8 = msec_per_rev / 8;   /* 45 degrees */

int sound_threshold = 64;
int loud_sound_flag = 0;    /* Was there a loud sound recently? */
int sound_abs_thresh = 227;
int sound_inhibit = 0;      /* Use this flag to turn off some sounds */

int back =  0b100;              /* Bumper status bits */
int left =  0b010;
int right = 0b001;

int bump_left  = 0;             /* Bumper status vars */
int bump_right = 0;
int bump_back  = 0;
int ir_left = 0;
int ir_right = 0;
int ir_detect_mask = 0b10000;           /* Bit E4 */
int ir_mask_l = 0b1000;             /* Bit D3 */
int ir_mask_r = 0b0100;             /* Bit D2 */
int ir_mask = ir_mask_l | ir_mask_r;

int cruise_active = T;
int photo_active = NIL;
int ir_active = NIL;
int bump_active = NIL;
int photo_cal = -8;
int photo_dead_zone = 2;        /* If the photo cells aren't equal... */
long tick = (long) 100;         /* Basic servo loop for everything */
long tick2 = (long)((int) tick / 2);
int port_e = 0x100A;            /* Should be in Library */

int m_mask_l = 0b100000;        /* Motor mask bits */
int m_mask_r = 0b010000;
int m_mask = m_mask_l | m_mask_r;

/* A5,4 => Enable Left, Right.  D5,4 => 0 - forward, 1 - backward */
int on = 0xff;
int off_ = 0;
int fwd = 0;
int bkw = m_mask_l | m_mask_r;

int sec(float secs)     /* Convert seconds to milliseconds */
{
    (int)(1000. * secs);
}

void wait(int milli_seconds)    /* Don't busy wait, check a timer */
{
    long timer_a;
    timer_a = mseconds() + (long) milli_seconds;

    while(timer_a > mseconds())
    {
        defer();
    }
}

int write_port(int port, int value, int mask)   /* Don't overwrite value in D */
{
    poke(port,(value & mask) | (peek(port) & ~mask));
}

void motor_cmd(int l_ena, int l_dir, int r_ena, int r_dir)
{
    write_port(port_a, (l_ena & m_mask_l) | (r_ena & m_mask_r), m_mask);
    write_port(port_d, (l_dir & m_mask_l) | (r_dir & m_mask_r), m_mask);
}

void move(int operation)
{
    if(operation == STOP)                 /* 0 - Stop */
        motor_cmd(off_,0,off_,0);
    else if(operation == FORWARD)             /* 1 - Forward */
        motor_cmd(on,fwd,on,fwd);
    else if(operation == BACKWARD)            /* 2 - Backward */
        motor_cmd(on,bkw,on,bkw);
    else if(operation == LEFT_TURN)           /* 3 - Left Turn */
        motor_cmd(on,bkw,on,fwd);
    else if(operation == RIGHT_TURN)          /* 4 - Right Turn */
        motor_cmd(on,fwd,on,bkw);
    else if(operation == LEFT_ARC)            /* 5 - Left Arc turn */
        motor_cmd(off_,0,on,fwd);
    else if(operation == RIGHT_ARC)           /* 6 - Right Arc turn */
        motor_cmd(on,fwd,off_,0);
}

/* The arbitration scheme is as follows:  Each behavior may compute an output
   for the motors.  It if does it stores the value in a global and sets an
   active flag.  The master motor controller resets the flags and sends the
   highest priorty command to the motor.
*/

void motor_control()
{
    while(1)
    {
        if(bump_active)
            move(bump_command);
        else if(ir_active)
            move(ir_command);
        else if(photo_active)
            move(photo_command);
        else if(cruise_active)
            move(cruise_command);
        else
            move(STOP);           /* No commands => STOP */

        defer();                  /* Update once per scheduler iteration */
    }
}

int cruise()                    /* Default activity */
{
    while(1)
    {
        cruise_command = FORWARD;           /* Robot forward */
        cruise_active = 1;
        wait(1000);                 /* Cruise once a second */
        /*     msleep((long)10 * tick);         /* Only every once in a while */
    }
}
/* Bright light => low numbers  */
int photo()                 /* Follow a light */
{
    int lpc, rpc, delta;            /* Left and Right Photo Cells */

    while(1)
    {
        lpc = analog(1) + photo_cal;
        rpc = analog(0);
        delta = rpc - lpc;           /* + => left sees bright, - => right */

        if(abs(delta) > photo_dead_zone)
        {
            if(delta > 0)
                photo_command = LEFT_ARC;     /* Left bright => turn left */
            else
                photo_command = RIGHT_ARC;    /* Otherwise turn right */

            photo_active = 1;            /* Activate when detected */
        }
        else
            photo_active = 0;            /* Deactivate when not detected */

        defer();            /* Once per scheduler tick */
        /*       msleep(tick);              /* limit activity */
    }
}

/* The three bump switches are connected as a voltage adder.  If the powering
   voltage divider has taps at 1/4, 1/2 and 1 times the supply then the voltage
   sum will be 1/3 * (A + B + C).  (Where A, B, and C are connected either
   to their corresponding tap or ground, 0).  Since the ACD gives values
   between 0 and 255 the possible voltages it will read will be 1/3 of 255
   times sums of 1/4, 1/2, and 1.
   A = 1/4 * 1/3 * 255 = 21, B = 42, C = 85,
   AB = 63, AC = 106, BC = 127, ABC = 148.
*/

int a_bump()            /* Analog Bump Detector */
{
    int val;
    val = analog(3);              /*  Get bump value */

    if(val > 74)
    {
        if(val > 116)
        {
            if(val > 137)
                return back | left | right;       /* Physically impossible (?) */
            else return back | left;
        }
        else if(val > 95)
            return back | right;
        else return back;
    }
    else
    {
        if(val > 31)
        {
            if(val > 52)
                return right | left;
            else return left;
        }
        else if(val > 10)
            return right;
        else return 0;
    }              /* There was no collision */
}

void bump_check()               /* Check for current bumps */
{
    int bump_stat;
    bump_stat = a_bump();        /* Check the analog bump */

    if(left & bump_stat)
        bump_left = 1;
    else
        bump_left = 0;

    if(right & bump_stat)
        bump_right = 1;
    else
        bump_right = 0;

    if(back & bump_stat)
        bump_back = 1;
    else
        bump_back = 0;
}

void damp()     /* Skirt bounces -- try to stop that */
{
    bump_command = STOP;  /* 'bump_active = 1' already in code */
    wait(125);
}

void bump()
{
    while(1)
    {
        bump_check();

        if(bump_left && bump_right)
        {
            bump_active = 1;
            damp();
            bump_command = BACKWARD;
            wait(msec_per_rad / 2);       /* Move back a bit */
            /*     msleep((long)2 * tick); */
            bump_command = LEFT_TURN;
            wait(rev_4);
        }
        /*      msleep((long) 4 * tick); } */
        else if(bump_left)
        {
            bump_active = 1;
            damp();
            bump_command = RIGHT_TURN;
            wait(rev_8);
        }
        /*      msleep((long)4 * tick); } */
        else if(bump_right)
        {
            bump_active = 1;
            damp();
            bump_command = LEFT_TURN;
            wait(rev_8);
        }
        /*      msleep((long)4 * tick); } */
        else if(bump_back)
        {
            bump_active = 1;
            damp();
            bump_command = LEFT_TURN;
            wait(rev_4);
        }
        /*      msleep((long)2 * tick); } */
        else
            bump_active = 0;

        defer();
        /*  msleep(tick2); */
    }
}

void ir()
{
    int val;

    while(1)
    {
        val = ir_detect();

        if(val == 0b11)        /* Left and right */
        {
            ir_active = T;
            ir_command = LEFT_ARC;
            /*    if (sound_inhibit == 0)
                     tone(1500.,.05); */
        }
        else if(val == 0b01)   /* Left IR */
        {
            ir_active = T;
            ir_command = RIGHT_ARC;
            /*    if (sound_inhibit == 0)
                     tone(2000.,.05); */
        }
        else if(val == 0b10)   /* Right IR */
        {
            ir_active = T;
            ir_command = LEFT_ARC;
            /*    if (sound_inhibit == 0)
                     tone(1000.,.05); */
        }
        else
        {
            ir_active = NIL;
        }

        /*    msleep(tick); } */
        defer();
    }
}

void alert_tune()
{
    tone(1046.5,0.200);
    tone(1396.9,0.200);
    tone(1046.5,0.200);
    tone(698.5,0.200);
}
/* Make this a process that looks for loud sounds */
int monitor_mike()  /* Look constantly for a loud sound */
{
    while(1)
    {
        if(loud_sound_flag = 0)
        {
            if(analog(2) > sound_abs_thresh)
                loud_sound_flag = 1;
        }
    }

    /*     else
           defer(); } */
}

int check_sound()   /* Play a tune when a loud sound is detected */
{
    while(1)
    {
        if(loud_sound_flag == 1)
        {
            defer();     /* Wait to see if it was a bump... */

            if(bump_active == 0)     /* We didn't crash into something */
            {
                sound_inhibit = 1;   /* Turn off other sounds */
                bicycle();       /* Play a song */
                sound_inhibit = 0;
                sleep(30.0);     /* Don't do it again right away... */
                loud_sound_flag = 0;
            }
        }

        defer();            /* Won't need any more time */
    }
}




int loud_sound_p()      /* T if Loud sound detected, NIL otherwise */
{
    int k,min,max,val;
    min = 255;
    max = 0;

    while(k < 100)
    {
        k++;
        val = analog(2);   /* Read mic */

        if(val > max)
            max = val;

        if(val < min)
            min = val;
    }

    if(max - min > sound_threshold)
        return T;
    else
        return NIL;
}


int cal_sound(int count)
{
    int max, min;
    int k,val;
    k = 0;
    max = 0;
    min = 255;
    tone(1660.,.1);
    sleep(.3);

    while(k < count)
    {
        k++;
        val = analog(2);  /* Read mic */

        if(val > max)
            max = val;

        if(val < min)
            min = val;
    }

    tone(880.,.1);
    return max - min;
}

int monitor_sound(int count)
{
    int k,val;
    tone(1660.,.1);
    sleep(.3);

    while(k < count)
    {
        k++;
        val = analog(2);

        if(val > 220)
            alert_tune();
    }
}

void mic_check(int count, int bit)
{
    int i;

    while(i < count)
    {
        i++;
        write_port(port_a,bit,bit);
        write_port(port_a,0,bit);
    }
}

void kill()                 /* Stop everything */
{
    move(STOP);
    beep();
}

/* void go() */
void main()
{
    init_motors();
    sleep(0.5);
    alert_tune();            /* Show that processes are starting */
    start_process(monitor_mike(), 1);    /* 1 tick */
    start_process(motor_control());
    start_process(monitor_mike(), 1);    /* 1 tick */
    start_process(cruise());
    start_process(monitor_mike(), 1);    /* 1 tick */
    start_process(photo());
    start_process(monitor_mike(), 1);    /* 1 tick */
    start_process(ir());
    start_process(monitor_mike(), 1);    /* 1 tick */
    start_process(bump());
    start_process(check_sound());    /* Maybe play a tune */
}

