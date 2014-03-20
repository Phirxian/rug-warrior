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
 * Display a value with a constant number of digits => "  1" OR " 10" OR "100"
 */
void p_byte(int val)
{
    if(val < 10)
        printf("  %d", val);
    else if(val < 100)
        printf(" %d", val);
    else
        printf("%d", val);
}

/**
 * Display a difference proportionately
 * Can be used to show a difference between 2 captors
 * @param diff - difference
 */
void print_diff(int diff)
{

    if(diff == 0)
        printf("       []       "); /* Zero */
    else if(diff < -8)
        printf("%c<<<<<<<]       ",127);    /* Less than -8 */
    else if(diff < 0)
    {
        repeat_char(32,8 + diff);  /* 32 = space */
        repeat_char(60,abs(diff)); /* 60 = "<" */
        printf("]       ");
    }
    else if(diff > 8)
        printf("       [>>>>>>>%c",126);    /* More than +8 */
    else
    {
        printf("       [");
        repeat_char(62,diff);
        repeat_char(32,8 - diff);
    }

}
