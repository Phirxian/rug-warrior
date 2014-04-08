/**
 * always print analogue(6) output
 */
void main()
{
    int val;
    printf(" \n \n");

    while(1)
    {
        val = analog(7);
        thermometer(val,0,255);
        printf("Analog-%d: %d\n",7, val);
        sleep(0.25);
    }
}
