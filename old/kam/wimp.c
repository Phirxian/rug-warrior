/* Wimp.c -- move away from any touch
 */

float oct = 440.0;

float wfreq(int step)
{
    if(step == 0)
        return oct;
    else
        return (oct * (2.0 ^ ((float) step / 12.0)));
}

int wimp_active = 0;    /* Don't start process when already running */

void start_wimp_tune()
{
    if(! wimp_active)
        start_process(wimp_tune());
}

void wimp_tune()
{
    wimp_active = 1;
    oct = 1760.0;
    tone(wfreq(5),0.1);
    tone(wfreq(4),0.1);
    tone(wfreq(5),0.1);
    tone(wfreq(4),0.1);
    wimp_active = 0;
}

/* Move away from any bump -- play an anoyed tune */

void wimp()
{
    int bmpr;
    printf("Wimp\n");

    while(1)
    {
        bmpr = bumper();

        if(bmpr != 0)        /* Somehow, we were bumped */
        {
            start_wimp_tune();      /* Play I'm-an-annoyed-wimp tune */

            if(bmpr == 0b110)       /* Back and Left bump */
                track(0,-80,5);       /* Spin right ~60 deg */
            else if(bmpr == 0b010)    /* Left Bump */
                track(0,-80,10);      /* Spin Right 120 deg */
            else if(bmpr == 0b011)    /* Left and Right => Front */
            {
                track(-80,0,4);     /* Backup then turn away */
                track(0,80,17);
            }
            else if(bmpr == 0b001)    /* Right Bump */
                track(0,80,10);       /* Spin Left 120 deg */
            else if(bmpr == 0b101)    /* Right and Back bump */
                track(0,80,5);       /* Spin Left 60 deg */

            track(80,0,8);          /* Go forward a bit */
            sleep(0.75);            /* Let bumper damp out */
        }
    }
}

void start_wimp()
{
    tone(1000.0,.1);
    wimp_tune();
    sleep(1.0);
    init_velocity();         /* Needed by track */
    start_process(wimp());
}
