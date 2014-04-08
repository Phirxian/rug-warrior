/**
 * always print analogue(6) output
 */
void main()
{
    int val;
    printf(" \n \n");

    while(1)
    {
        val = analog(6);
        thermometer(val,0,255);
        printf("Analog-%d: %d\n",6, val);
        sleep(0.25);
    }
}
