/**
 * Turn the RugBat then Compute the distance between the rugbat and the object
 */
void main()
{
    /* Turn the RugBat*/
    sonar_init_servo();
    scan_display_position(-100,100, 5); /* 5 loop*/
    printf("END OF LOOPS");
    sleep(5.0);
    printf("\n\n");
    sonar_servo_off();
    /* Calculate the distance continious between an object and the sonar */
    init_sonar();

    while(1)
    {
        ping();
        printf("%f\n", feetToCm(range()));
        sleep(2.0);
    }
}
