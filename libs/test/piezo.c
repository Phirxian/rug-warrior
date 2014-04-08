/**
 * play song
 * @see cof()
 */
void main()
{
    int ctr = 0;

    while(1)
    {
        printf(" \n");
        ctr++;
        /* Enable note frequency display */
        note_display = 1;
        cof();
        /* Disable display */
        note_display = 0;
    }
}
