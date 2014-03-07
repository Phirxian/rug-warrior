void main()
{
    int scan;
    printf("Runs until a wall is found 85cm\n");
    start_process(encoder_aux());

    init_motors();
    init_sonar();
    sonar_init_servo();

    move_behind(85.0, 1.0, 50);

    for(scan = -100; scan<100; ++scan)
    {
        set_servo(scan);
        ping();
        last = feetToCm(range());
        printf("scan %f\n", last);
    }

    set_servo(0);
    sonar_servo_off();
}
