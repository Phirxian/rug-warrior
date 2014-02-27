void main()
{
    float last = 100.0;

    _motor_initial_speed_ = 100;

    printf("runing while not in front of wall 50cm");

    init_motors();
    init_sonar();
    sonar_init_servo();

    set_servo(0);

    start_process(encoder_aux());
    start_process(running_forever());

    while(last >= 52.)
    {
        ping();
        last = feetToCm(range());

        printf("%f\n", last);

        /*
         * more the rug-warrior is
         * front of wall more the
         * rug-warrior is waiting
         * for a signals
         */
        sleep(0.1);
    }

    _running_process_running_ = 0;
    sonar_servo_off();
    stop();
}
