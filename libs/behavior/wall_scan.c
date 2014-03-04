void main()
{
    int pid;
    float last;
    int scan;

    printf("Runs until a wall is found 85cm\n");

    init_motors();
    init_sonar();
    sonar_init_servo();

    set_servo(0);
    sleep(0.175);

    start_process(encoder_aux());

    while(1)
    {
        ping();
        last = feetToCm(range());

        printf("%f\n", last);

        if(last >= 86.)
        {
            kill_process(pid);
            _running_process_running_ = 0;
            _motor_initial_speed_ = 50+(int)(last-87.);
            pid = start_process(running_forever());
        }
        else if(last <= 82.)
        {
            kill_process(pid);
            _running_process_running_ = 0;
            _motor_initial_speed_ = -50+(int)(-83.+last);
            pid = start_process(running_forever());
        }
        else if(!_motor_initial_speed_) break;
        else break;


        /*
         * The closest the Rug-Warrior is
         * from a wall, the more the
         * Rug-Warrior is waiting
         * for a signal
         */
        /*sleep(feetToMotor(cmToFeet(last - 85.), _motor_initial_speed_));*/
        sleep(0.175);
    }

    _running_process_running_ = 0;

    for(scan = -100; scan<100; ++scan)
    {
        set_servo(scan);
        ping();
        last = feetToCm(range());
        printf("scan %f\n", last);
    }

    set_servo(0);
    sonar_servo_off();
    stop();
}
