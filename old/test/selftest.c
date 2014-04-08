/* Rug Warrior Self Test */

/* Code depends of having register-defs.c already loaded
   -- call through .lis file */

/*****************************************************************************/
/* Utility Routines                                                          */
/*****************************************************************************/

/* Print character ch, rep times */
void repeat_char(int ch, int rep)
{
    int j = 0;

    for(j = 0; j < rep; j++)
        printf("%c", ch);
}

/* Count the number of digits in an integer (negative numbers add 1 for - sign) */
int digits(int signed_val)
{
    int digs, val;
    val = abs(signed_val);

    if(val < 100)
    {
        if(val < 10)
            digs = 1;
        else digs = 2;
    }
    else if(val < 10000)
    {
        if(val < 1000)
            digs = 3;
        else digs = 4;
    }
    else digs = 5;

    if(signed_val >= 0)
        return digs;
    else return (digs + 1);
}

void printn(int val, int cols)  /* Print val in the given number of columns if possible */
{
    int digs;         /* Number of digits */
    digs = digits(val);

    if(digs >= cols)
        printf("%d", val);  /* May not be possible to print in required number of cols */
    else
    {
        repeat_char(32,cols - digs);
        printf("%d",val);
    }
}

/******************************************************************************/
/* Self test routines                                                         */
/******************************************************************************/

/* 0123456789ABCDEF
          [>>>>
   Photo Lxxx Rxxx* */
/* Lower numbers => brighter light.  The difference inticator pointes toward the
   brighter light */
void photo_test()
{
    int pc_left, pc_right;

    while(1)
    {
        pc_left  = analog(photo_left);
        pc_right = analog(photo_right);
        therm_diff(pc_right - pc_left);   /* Diff thermometer points toward brighter light */
        printf("Photo R");
        p_byte(pc_right);
        printf(" L");
        p_byte(pc_left);
        printf("\n");
        sleep(0.25);
    }
}

/* Microphone test
   This unsophisticated test just detects overall sound level at some
   unspecified sampling rate */
void mic_test()
{
    int sum, i;
    leds(0);  /* Mic may detect electrial noise caused by IR osc */

    while(1)
    {
        sum = 0;  /* Collect some samples */

        for(i = 0; i < 64; i++)   /* Average over 64 values */
            sum = sum + abs(analog(2) - 128);   /* Each sample in range 0 to 128 */

        therm(sum >> 5);   /* Divide by 64 to average samples and multiply by */
        printf("Microphone Test\n"); /* 2 to convert therm() range from 128 to 256 */
    }
}

/* IR Test */
void ir_test()
{
    int ir_val;

    while(1)
    {
        ir_val = ir_detect();

        if(ir_val == 0b00)
            printf("                "); /* Clear */
        else if(ir_val == 0b11)
            printf(" <<<<<====>>>>> "); /* Both */
        else if(ir_val == 0b01)
            printf("          >>>>> "); /* Right */
        else
            printf(" <<<<<          "); /* Left */

        printf("IR Test\n");
        sleep(0.35);
    }
}

/* Motor Test
   0123456789ABCDEF
   Motor cc L vvvv    Where cc is the number of clicks during the past interval
   Test  cc R vvvv*   and vvvv is the commanded velocity (as a percentage of max)
*/

void display_mot_test(float lvelf, float rvelf)
{
    int lvel, rvel;
    lvel = (int) lvelf;   /* Cooerce float to int for corect printing prperties */
    rvel = (int) rvelf;
    printf("Motor ");
    printn(get_left_clicks(),2);
    printf(" L ");
    printn(lvel,4);
    printf(" ");
    printf("Test  ");
    printn(get_right_clicks(),2);
    printf(" R ");
    printn(rvel,4);
    printf(" ");
    printf("\n");
}

void set_show_motors(int left, int right)
{
    motor(0, left);
    motor(1, right);
    display_mot_test((float) left, (float) right);
}

void motor_test()
{
    int i;
    float interval = 0.5; /* Click counting Interval for velocity computation */
    init_velocity();  /* Turn on automatic velocity measuring */

    while(1)
    {
        for(i = 0; i <= 100; i = i + 10)  /* Move forward with increasing speed */
        {
            set_show_motors(i,i);    /* Both motors move forward */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(100,100);    /* +Max */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(0,0);    /* Stop */
            sleep(interval);
        }

        for(i = 0; i <= 100; i = i + 10)  /* Move backward with increasing speed */
        {
            set_show_motors((- i), (- i));
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(-100,-100);    /* -Max */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(0,0);    /* Stop */
            sleep(interval);
        }

        for(i = 0; i <= 100; i = i + 10)  /* Spin with increasing speed */
        {
            set_show_motors(i,(- i));    /* Left fwd, right backward */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(100,-100);    /* +-Max */
            sleep(interval);
        }

        for(i = 0; i <= 200; i = i + 10)  /* Reverse direction while spinning */
        {
            set_show_motors((100 - i), (i - 100));
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(-100,100);    /* -+Max */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(0,0);    /* Stop */
            sleep(interval);
        }
    }
}


float test_delay = 0.25;    /* Delay between subsequent iterations of a test */

int ivar = 0;           /* Global integer variable for debugging */
float fvar = 0.0;       /* Global float variable for debugging */


int abs(int arg)        /* Absolute value of an integer */
{
    if(arg < 0)
        return - arg;
    else
        return arg;
}

/* Print a 3 digit 8-bit integer that takes up exactly 3 characters */

void p_byte(int val)
{
    if(val < 10)
        printf("  %d", val); /* ..n */
    else if(val < 100)
        printf(" %d", val);  /* .nn */
    else
        printf("%d", val);   /* nnn */
}

/* LCD test - should exercise all characters at all positions (but dosen't for now) */
void lcd_test()
{
    int j;
    int chr = 32;
    printf("\nLCD Test\n");
    sleep(1.5);

    while(1)
    {
        for(j = 1; j<= 31; j++)
        {
            printf("%c",chr);
            chr++;
        }

        sleep(1.5);   /* Time to see the screen */
        printf("\n"); /* Clear screen for next round */
    }
}

/* Piezo test */
void piezo_test()
{
    int ctr = 0;

    while(1)
    {
        printf("\n");
        /*      printf("Piezo test"); */
        ctr++;
        /*       printf(" %d",ctr); */
        note_display = 1; /* Enable note frequency display */
        cof();
        note_display = 0; /* Disable display */
    }
}

/*  0123456789ABCDEF  */
/* [Bumper   A B C  ] */
/* [Test      val  *] */

void bumper_test()
{
    int bmpr;
    printf("\n");

    while(1) /* 0123456789ABCDEF */
    {
        bmpr = bumper();
        printf("Bumper   ");       /*9ABCDEF */

        if(bmpr == 0) printf(" none  ");
        else
        {
            if(bmpr & 0b001) printf(" A");
            else printf("  ");

            if(bmpr & 0b010) printf(" B");
            else printf("  ");

            if(bmpr & 0b100) printf(" C ");
            else printf("   ");
        }

        printf("Test       %d\n", analog(3)); /* Raw bumper reading */
        sleep(0.5);
    }
}

/*
      if      (bmpr <  11) printf(" none  ");
      else if (bmpr <  32) printf(" A     ");
      else if (bmpr <  53) printf("   B   ");
      else if (bmpr <  74) printf(" A B   ");
      else if (bmpr <  96) printf("     C ");
      else if (bmpr < 117) printf(" A   C ");
      else if (bmpr < 132) printf("   B C ");
      else                 printf(" A B C ");
      printf("Test       %d\n", bmpr);
      sleep(0.5);
    }}
*/

/* Port PA0, PA7 Shaft encoder
/* [SE Left  0 123  ]  Col 9 Current value 1 or 0
   [LE Right 1 123 &]  Col BCD Current count
*/

/*0123456789ABCDEF
  Encoder  L nnnn
  Test     R nnnn* */

int enc_proc_handle = 0;
int l_enc_counts = 0;
int r_enc_counts = 0;

/* Emperically, the Right SE is connected to PA7, left to PA0, opposite to
   what layout schematic shows */
void encoder_aux()  /* Copy encoders to LEDs and count clicks */
{
    int l_old, r_old, l_new, r_new;   /* Trigger on Hi -> Low transition */

    while(1)
    {
        l_new = peek(porta) & 0b00000001;
        r_new = peek(porta) & 0b10000000;

        if(l_old & ~l_new)    /* (old and not new) => 1 -> 0 transition */
            l_enc_counts++;     /* Increment counter */

        if(r_old & ~r_new)
            r_enc_counts++;

        l_old = l_new;        /* Update transition detector */
        r_old = r_new;

        if(l_new)         /* Non-zero => turn LED on */
            bit_set(portd,0b000100);
        else bit_clear(portd,0b000100);

        if(r_new)
            bit_set(portd,0b001000);
        else bit_clear(portd,0b001000);
    }
}

void encoder_test()
{
    if(enc_proc_handle /= 0)
        kill_process(enc_proc_handle);  /* Don't start the process twice */

    enc_proc_handle = start_process(encoder_aux());   /* Show SE state on LEDs */

    while(1)  /*0123456789A                     BCDEF*/
    {
        printf("Shaft    L ");
        printn(l_enc_counts,4);
        printf(" ");
        printf("Encoders R ");
        printn(r_enc_counts,4);
        printf("\n");
        sleep(0.5);
    }
}

void port_test()
{
    while(1)  /*0123456789ABCDEF*/
    {
        printf("PortA: %b PortD: %b\n", peek(0x1000), peek(0x1009));
        sleep(0.75);
    }
}

/* Display an analog value on one line of the LCD */

void therm(int val) /* Val 0 to 255 */
{
    int j;
    int blks;         /* Number of blocks that are set: val/16 */
    blks = (val >> 4) + (1 & (val >> 3)); /* Divide with round off */

    for(j = 1; j<= blks; j++)
        printf(">");

    for(j = blks; j < 16; j++)
        printf("-");
}

/* 0123456789ABCDEF     */
/*        []       :  0 */
/*        [>>>>    :  4 */
/*   <<<<<<]       : -6 */


/* therm_diff is for displaying the difference between two quantities */
void therm_diff(int val)
{
    if(val == 0)
        printf("       []       "); /* Zero */
    else if(val < -8)
        printf("%c<<<<<<<]       ",127);    /* Less than -8 */
    else if(val < 0)
    {
        repeat_char(32,8 + val);  /* 32 = space */
        repeat_char(60,abs(val)); /* 60 = "<" */
        printf("]       ");
    }
    else if(val > 8)
        printf("       [>>>>>>>%c",126);    /* More than +8 */
    else
    {
        printf("       [");
        repeat_char(62,val);
        repeat_char(32,8 - val);
    }
}

/* Uses integer division with more-or-less correct roundoff */

int thermometer(int val, int low, int high)
{
    int range = high - low;
    int blks = (((val - low) * 16) + (range >> 1)) / range;
    int j;

    if(blks < 0)
        blks = 0;
    else if(blks > 16)
        blks = 16;

    for(j = 1; j <= blks; j++)  printf("%c",219);  /* 219 => [] */

    for(j = blks; j < 16; j++)  printf("-");

    return blks;
}

void pyro_test(int lo, int hi)
{
    int val;
    printf("\n\n");

    while(1)
    {
        val = analog(pyro);
        thermometer(val,lo,hi);
        printf("Pyro: %d\n", val);
        sleep(test_delay);
    }
}

void analog_test(int channel)
{
    int val;
    printf("\n\n");

    while(1)
    {
        val = analog(channel);
        thermometer(val,0,255);
        printf("Analog-%d: %d\n",channel, val);
        sleep(test_delay);
    }
}

/* Test LEDs */
void led_test()
{
    int ctr = 0;

    while(1)
    {
        leds(ctr);
        ctr = (ctr + 1) & 0xF;
        printf("LED test        %b\n", ctr);
        sleep(0.5);
    }
}

/* 0123456789ABCDEF
   User     PA1 0
   Digital  PA2 1 * */

void user_digital_test()
{
    int dig;

    while(1)
    {
        dig = peek(porta) & 0b00000110;
        printf("User     PA1 %d", digital(1));
        printf("  ");
        printf("Digital  PA2 %d", digital(2));
        printf("\n");
        sleep(0.5);
    }
}

void main()
{
    sleep(0.75);
    test_number++;

    if((test_number > 14) | (test_number < 0))
        test_number = 0;

    while(test_number == 0)
    {
        printf("Rug Warrior selftest\n");
        sleep(2.0);
        printf("Press RESET to  ");
        printf("select test\n");
        sleep(2.0);
    }

    if(test_number == 1)
        lcd_test();
    else if(test_number == 2)
        led_test();
    else if(test_number == 3)
        piezo_test();
    else if(test_number == 4)
        photo_test();
    else if(test_number == 5)
        bumper_test();
    else if(test_number == 6)
        mic_test();
    else if(test_number == 7)
        ir_test();
    else if(test_number == 8)
        encoder_test();
    else if(test_number == 9)
        motor_test();
    else if(test_number == 10)
        user_digital_test();
    else if(test_number == 11)
        analog_test(6);
    else if(test_number == 12)
        analog_test(7);
    else if(test_number == 13)
        pyro_test(100,155);
}
