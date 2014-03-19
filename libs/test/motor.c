/**
 * display the motor state on the LCD screen
 */
void display_mot_test(int lvel, int rvel)
{
    printf("Motor ");
    printn(get_left_clicks(),2);
    printf(" L ");
    printn(lvel,4);
    printf(" ");
    printf("Test  ");
    printn(get_right_clicks(),2);
    printf(" R ");
    printn(rvel,4);
    printf(" ");
    printf(" \n");
}

/**
 * set the motor speed and display on the LCD screen
 */
void set_show_motors(int left, int right)
{
    motor(0, left);
    motor(1, right);
    display_mot_test(left, right);
}

/**
 * basic motor test
 */
void main()
{
    int i;
    float interval = 0.5; /* Click counting Interval for velocity computation */
    init_velocity();      /* Turn on automatic velocity measuring */

    while(1)
    {
        for(i = 0; i <= 100; i = i + 10)  /* Move forward with increasing speed */
        {
            set_show_motors(i,i);         /* Both motors move forward */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(100,100);     /* +Max */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(0,0);         /* Stop */
            sleep(interval);
        }

        for(i = 0; i <= 100; i = i + 10)  /* Move backward with increasing speed */
        {
            set_show_motors((- i), (- i));
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(-100,-100);    /* -Max */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(0,0);         /* Stop */
            sleep(interval);
        }

        for(i = 0; i <= 100; i = i + 10)  /* Spin with increasing speed */
        {
            set_show_motors(i,(- i));     /* Left fwd, right backward */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(100,-100);    /* +-Max */
            sleep(interval);
        }

        for(i = 0; i <= 200; i = i + 10)  /* Reverse direction while spinning */
        {
            set_show_motors((100 - i), (i - 100));
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(-100,100);    /* -+Max */
            sleep(interval);
        }

        for(i = 0; i <= 2; i++)
        {
            set_show_motors(0,0);         /* Stop */
            sleep(interval);
        }
    }
}
