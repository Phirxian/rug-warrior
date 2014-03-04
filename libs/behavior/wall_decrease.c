void main()
{
    int pid;
    float last;

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

        if(last >= 100.)
        {
            kill_process(pid);
            _running_process_running_ = 0;
            _motor_initial_speed_ = 50+(int)(last-50.);
            pid = start_process(running_forever());
        }
        else if(last <= 100.)
        {
            kill_process(pid);
            _running_process_running_ = 0;
            _motor_initial_speed_ = -50+(int)(-50.+last);
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
    /*kill_process(pid);*/
    sonar_servo_off();
    stop();
}
