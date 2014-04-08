void therm(int val) /* Val 0 to 255 */
{
    int j;
    int blks;                               /* Number of blocks that are set: val/16 */
    blks = (val >> 4) + (1 & (val >> 3));   /* Divide with round off */

    for(j = 1; j<= blks; j++)
        printf(">");

    for(j = blks; j < 16; j++)
        printf("-");
}

/**
 * always print the microphone value
 * like progress-bar
 */
void main()
{
    int sum, i;

    /*  leds(0);    /* Mic may detect electrial noise caused by IR osc */

    while(1)
    {
        sum = 0;

        for(i = 0; i < 64; i++)                 /* Average over 64 values */
            sum = sum + abs(analog(2) - 128);   /* Each sample in range 0 to 128 */

        therm(sum >> 5);             /* Divide by 64 to average samples and multiply by */
        printf("Microphone Test\n"); /* 2 to convert therm() range from 128 to 256 */
    }
}
