/*
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

/*
 * repeat ch on rep column
 */
void repeat_char(int ch, int rep)
{
    int j = 0;

    for(j = 0; j < rep; j++)
        printf("%c", ch);
}
