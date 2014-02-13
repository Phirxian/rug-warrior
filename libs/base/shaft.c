/* Shaft.c */

/* Utility functions for monitoring the shaft encoders */

/* Functions for reading left and right "velocities" using PA7s pulse counter */
/* and an interrupt routine (in speed.icb) to catch pulses on PA0 */

void init_velocity()        /* Enable PA7 pulse counting and PA0 interrupt */
{
    poke(0x1026,0b01010000);  /* PACTL  PA7 input, trigger on rising edge */
    poke(0x1027,0);       /* PACNT  init to 0 */
    left_clicks = 0;      /* Global referenced by interrupt routine */
    bit_clear(0x1021,0b10);   /* TCTL2 Assign values to the two lowest order bits */
    bit_set(0x1021,0b01);   /* of tctl2 without changing any other bits */
    bit_set(0x1022,0b01);     /* TMSK1  enable IC3 interrupts */
}

int get_left_clicks()       /* Get the number of clicks and reset to 0 */
{
    int clks = left_clicks;   /* If called at regular intervals this funciton returns */
    left_clicks = 0;      /* the the left velocity in units of clicks per interval */
    return clks;
}

int get_right_clicks()      /* Same as above for right Shaft encoder */
{
    int clks = peek(0x1027);  /* PACNT is only an 8-bit register -- be sure to */
    poke(0x1027,0);       /* call it often enough so that it dosen't overflow */
    return clks;
}
