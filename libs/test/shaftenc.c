/**
 * always print the shaft value
 * (updated by encoder_aux() process)
 * (automaticaly start @see shared/thread.c)
 */
void main()
{
    /**
     * if the rotor is on pair clicks
     * left_shaft & right_shaft = 1; 0 else
     */
    while(1)
    {
        init_velocity();
        printf("L %d    R %d\n", left_shaft(), right_shaft());
        sleep(1.);
    }
}
