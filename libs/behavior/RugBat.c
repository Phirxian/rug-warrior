/* ADD GUILHEM */
/**
 * Make the servo scan back and forth between low_pos and high_pos
 * Delay tells the servo how long to wait between steps
 * ADD : Print the actual position of the rugBat between -100 & 100
 *       And make a predetermined number of loop
 * @param nLoop - number of laps remaining (must be superior to 1)
 */
void scan_display_position(int low_pos, int high_pos, int nLoop)
{
    int step = 1;
    if(nLoop < 1)
    {
        return;
    }

    /* Standard step size */
    /* Start can fail otherwise*/
    servo_pos = (low_pos + high_pos) / 2;

    while(1)
    {
        if((servo_pos < low_pos) || (servo_pos > high_pos))
        {
            /* Don't reverse instantly */
            sleep(3.0 * scan_delay);
            step = (- step);
        }

        servo_pos = servo_pos + step;
        set_servo(servo_pos);
        printf("Position %d\n", servo_pos);
        sleep(scan_delay);

        if(servo_pos == 100)    /* A laps has been realized */
        {
            nLoop--;
        }

        if(nLoop == 0)    /* All loop are realized */
        {
            return;
        }
    }
}

void main()
{
    /* Turn the RugBat*/
    sonar_init_servo();
    scan_display_position(-100,100, 5); /* 5 loop*/
    printf("END OF LOOPS");
    sleep(5.0);
    /* Calculate the distance continious between an object and the sonar */
    init_sonar();

    while(1)
    {
        ping();
        printf("%f", feetToCm(range()));
        sleep(2.0);
    }
}
