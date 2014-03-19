/*
 * Follow a light source and stop at 20 centimeters
 * File: light_distance.c
 * Last edition: -
 * Version: -
 */

float period_gain = 0.0005;

/**
 * Initiate a sonar pulse.  init_sonar must be called before a ping can
 * be sent.
 */
void ping_precise()
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
float range_precise()
{
    if((peek(_tflg1) & 0b10) == 0)     /* If IC2 didn't see echo */
        return -1.0;    /* Let calling routine decide which flag */
    else          /*  indicates no echo */
        return
            ((float)((peekword(_tic2) - peekword(_tic1)) >> 1) * 0.000569);
}

/*
 * Compute the length of the pause between each iteration
 * from the sum of the photo cells.
 * Protected against negative periods.
 */
float period(int left, int right)
{
    return (period_gain * (float)(max(0, (512 - (left + right)))));
}

void light_distance()
{
    int left = 0;
    int right = 0;
    float resultat;

    init_sonar();

    while(1)
    {
        ping_precise();
        resultat = feetToCm(range_precise());
        printf("%f\n", resultat);
        sleep(2.);
        /*left  = analog(photo_left);
        right = analog(photo_right);
        sleep(period(left, right));*/   /*  then wait a while */
    }
}

void main()
{
    light_distance();
}
