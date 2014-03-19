/**
 * Enable PA7 pulse counting and PA0 interrupt
 */
void init_velocity()
{
    poke(0x1026,0b01010000);  /* PACTL  PA7 input, trigger on rising edge */
    poke(0x1027,0);           /* PACNT  init to 0 */
    left_clicks = 0;          /* Global referenced by interrupt routine */
    bit_clear(0x1021,0b10);   /* TCTL2 Assign values to the two lowest order bits */
    bit_set(0x1021,0b01);     /* of tctl2 without changing any other bits */
    bit_set(0x1022,0b01);     /* TMSK1  enable IC3 interrupts */
}

/**
 * Get the number of left clicks and reset to 0
 */
int get_left_clicks()
{
    int clks = left_clicks;   /* If called at regular intervals this funciton returns */
    left_clicks = 0;          /* the the left velocity in units of clicks per interval */
    return clks;
}

/**
 * Get the number of right clicks and reset to 0
 */
int get_right_clicks()
{
    int clks = peek(0x1027);  /* PACNT is only an 8-bit register -- be sure to */
    poke(0x1027,0);           /* call it often enough so that it dosen't overflow */
    return clks;
}
