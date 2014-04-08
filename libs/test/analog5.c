/**
 * always print analogue(5) output
 */
void main()
{
    int val;
    printf(" \n \n");

    while(1)
    {
        val = analog(5);
        thermometer(val,0,255);
        printf("Analog-%d: %d\n",5, val);
        sleep(0.25);
    }
}
