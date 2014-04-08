/**
 * Run at a maximum speed until an obstacle is detected 100 cm away,
 * then slow down until the obstacle is 50 cm away, then stop.
 * These 2 values are passed to move_behind()
 *
 * Last update: March. 21 2014 - 9:37
 * @version Final
 */
void main()
{
    int pid;
    float last;

    init_sonar();
    sonar_init_servo();

    printf("Wall decrease behavior\n");

    set_servo(0);
    sleep(0.175);
    _motor_initial_speed_ = 100;

    ping();
    _move_behind_detected_distance_ = feetToCm(range());
    start_process(detect_distance_sonar());

    /**
     * run util 1meter as found
     */

    pid = start_process(running_forever());

    while(_move_behind_detected_distance_ > 100.0)
        sleep(_process_yield_time_);

    kill_process(pid);

    /**
     * decrese speed for move_behind(50.0, 1.0, 30)
     */

    move_behind(50.0, 1.0, 30);

    /**
     * clear process
     */

    stop_process(_detect_distance_process_running_);
    wait_process(_detect_distance_process_running_);

    set_servo(0);
    sonar_servo_off();
}
