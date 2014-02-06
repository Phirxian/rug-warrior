/*  Contest code for starting and stopping machines     */

/*  Updated/checked 1-19-95 by Aaron Belenky            */
/*  for the 1995 IAP 6.270 contest          */

/*************************************/
/*** Starting and Stopping Machine ***/
/*************************************/

/* ANY MODIFICATION OF THIS CODE IS DISCOURAGED, AS YOU WILL BE
   DISQUALIFIED IF YOUR MACHINE FAILS TO FULLY STOP AT THE END OF 60
   SECONDS.  HACK AT YOUR OWN RISK!

   You should only call start_machine(int)
   The rest are helper procedures.
   _average_analog_sensor() may be of some use.

   You can also vary the number of samples averaged together
   in an attempt to fine tune speed/accuracy the sensors. */

/*
 * Call with analog port of light sensor used to detect starting lamp
 */
void start_machine(int port)
{
    /* Notes: ir_xmit_period and ir_freq_rcv_select are system
    global variables. Using these bypasses the need for
    set_ir_transmit_frequency() and set_ir_receive_frequency() */
    int on_value, off_value, trigger_value;

    /* STEP 1.  Choose IR frequencies and goal based on DIP switch 1 */
    while(!escape_button())
    {
        if(dip_switch(1))
        {
            /* DIP_SWITCH(1) = 1; +45deg polarization */
            printf("Xmit STRIPED    ESC to continue\n");
            ir_xmit_period= 10000;       /* Transmit at 100 Hz */
            ir_freq_rcv_select= 125;     /* Recieve 125 Hz */
        }
        else
        {
            /* DIP_SWITCH(1) = 0; -45deg polarization */
            printf("Xmit SOLID      ESC to continue\n");
            ir_xmit_period= 8000;         /* Transmit at 125 Hz */
            ir_freq_rcv_select= 100;      /* Recieve 100 Hz */
        }

        sleep(.2);      /* To stablize display */
    } /* END while(!escape_button()) */

    while(escape_button());

    /* STEP 2.  Wait for user to press choose button, indicating
        that machine is positioned over starting lamp (which is on). */

    while(!choose_button())
    {
        printf("%d when ON       Press CHOOSE\n", analog(port));
        sleep(0.15); /* delay helps printf's look nice */
    }

    on_value= _average_analog_sensor(port, 20);
    /* 20 Samples of the light when it is on */

    while(choose_button());

    /* STEP 3.  Wait for user to press choose button, indicating
       that machine is positioned over starting lamp (which is off). */

    while(!choose_button())
    {
        printf("%d when OFF      Press CHOOSE\n", analog(port));
        sleep(0.15); /* delay helps printf's look nice */
    }

    off_value= _average_analog_sensor(port, 20);  /* take 20 samples */

    while(choose_button());   /* wait for user to release button */

    /* STEP 4.  Compute mid-range value.  Wait for an averaged sample
        to fall below that value; then start machine. */
    trigger_value= (on_value + off_value) / 2;

    while(_average_analog_sensor(port, 6) > trigger_value)
    {
        printf("%d waiting for %d\n", analog(port), trigger_value);
        sleep(.1);
    }

    /* STEP 5.  Enable start. */
    printf("G O  !!! \n");
    reset_system_time();
    ir_transmit_on();   /* turn on IR           */
    start_process(_stop_machine(60.), 1, 100); /* give it one tick  */
}

int _average_analog_sensor(int port, int samples)
{
    int i, sum=0;

    for(i=0; i<samples; i++)
        sum += analog(port);

    return sum/samples;
}

void _stop_machine(float time)
{
    while(seconds() < time);

    while(1)
    {
        hog_processor();
        ir_transmit_off();  /* turn off IR */
        alloff();
        servo_off();
        beeper_off();
        /*  ADD YOUR OWN COMMANDS TO TURN OFF OTHER EQUIPMENT HERE */
    }
}
