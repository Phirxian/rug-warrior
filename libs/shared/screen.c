/**
 * Print val in the given number
 * of columns if possible
 */
void printn(int val, int cols)
{
    int digs;
    digs = digits(val);

    /* May not be possible to print in required number of cols */

    if(digs >= cols)
        printf("%d", val);
    else
    {
        repeat_char(32,cols - digs);
        printf("%d",val);
    }
}

/**
 * repeat ch on rep column
 */
void repeat_char(int ch, int rep)
{
    int j = 0;

    for(j = 0; j < rep; j++)
        printf("%c", ch);
}

/**
 * Display a difference proportionately (the more there is difference, the more of char displaying ')
 * Can be used to show a difference between 2 captors
 * @param diff - difference going from -100 to 100
 */
void print_diff(int diff)
{
    char PATTERN_LEFT = '<';
    char PATTERN_RIGHT = '>';
    /* Number of digits to will be written*/
    int number_digits;
    number_digits = (int) diff / (100/8);

    if(diff > 0)    /* Write on the right side */
    {
        repeat_char(' ', 8); /* Margin*/
        repeat_char(PATTERN_RIGHT, number_digits);
    }
    else if(diff < 0)      /* Write on the left side */
    {
        number_digits = -number_digits;
        repeat_char(' ', 8-number_digits); /* Dynamic Margin*/
        repeat_char(PATTERN_LEFT, number_digits);
    }

    printf("\n");
}
