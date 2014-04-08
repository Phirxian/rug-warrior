/* Echo -- Repeat the sounds you've heard */

/* Empirically, the most easily heard (by a second robot) combination is
   LOW = 1500, HIGH = 1600, Dur = a minimum of 0.008
 */


int sound_delta = 50;   /* Must be low for robots to hear each other */
long hold_time = 175L;      /* Numer of milliseconds to hold high time */
int loud_p = 0;         /* 0 => quite, 1 => loud */
int sound_count = 0;

int listen_p = 1;            /* Make it 0 during echo execution */

int s_level_max = 0;

float warb_low = 1500.0;
float warb_high = 1600.0;
float warb_dur = 0.01;

void warb()
{
    float freq = warb_low;
    twitch();         /* Motion accompanies sound */
    beeper_on();

    for(freq = warb_low; freq < warb_high; freq = freq + 10.0)
    {
        set_beeper_pitch(freq);
        sleep(warb_dur);
    }

    beeper_off();
}

/**
 * Sample the microphone to see how much noise there is
 */
void sample_sound()
{
    int s_level = 0;            /* Instantaneous sound level */
    long go_low_time = 0L;
    long current_time = 0L;

    while(1)
    {
        while(listen_p)
        {
            current_time = mseconds();
            s_level = abs(analog(microphone) - 128);    /* Abs difference from 128 */

            if(s_level > s_level_max)
                s_level_max = s_level;

            if(s_level > sound_delta)
                go_low_time = current_time + hold_time; /* Compute new go low time */

            if(current_time > go_low_time)
                loud_p = 0;
            else
            {
                if(loud_p == 0)   /* Was 0, now 1 => add to transition count */
                    sound_count++;

                loud_p = 1;
            }
        }
    }
}

/**
 * Count the number of low high transitions during some interval.
 * Start a timer at the first low high transition, then keep checking for
 * more low high transitions until time runs out
 */
int echo_cmd = 0;

/**
 * This is how long we get to gather the sound
 */
float cmd_period = 1.2;

void capture_command()
{
    int old_count = 0;

    while(1)
    {
        if(old_count != sound_count)
        {
            sleep(cmd_period);    /* Wait for the sounds to happen */
            echo_cmd = sound_count;
            sound_count = 0;
        }
    }
}

void twitch_aux()
{
    drive(100,0);
    sleep(.07);
    drive(-100,0);
    sleep(0.07);
    drive(0,0);
}

void twitch()
{
    start_process(twitch_aux(),1);
}

void echo_control()
{
    int i = 0;

    while(1)
    {
        if(echo_cmd != 0)
        {
            listen_p = 0;

            for(i = 0; i < echo_cmd; i++)
            {
                warb();
                sleep(0.2);
            }

            sleep(0.15);  /* Wait a little longer before listening again */
            echo_cmd = 0;
            listen_p = 1;
        }
    }
}

void rpt()
{
    while(1)
    {
        printf("Echo     Cnt: %d Max sound: %d\n", sound_count, s_level_max);
        s_level_max = 0;
        sleep(0.4);
    }
}

void start_echo()
{
    start_process(sample_sound());
    start_process(capture_command(),1);
    start_process(echo_control(),1);
    start_process(rpt(),1);
}
