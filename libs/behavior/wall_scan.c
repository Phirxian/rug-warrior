/**
 * Run at a maximum speed until an obstacle is detected 85 cm away,
 * slow down until the obstacle is close,
 * then scan the world in front of robot (180°)
 *
 * Last update: -
 * @version Final
 */
void main()
{
    int scan;
    float last;

    init_sonar();
    sonar_init_servo();

    printf("Runs until a wall is found 85cm\n");

    set_servo(0);
    sleep(0.175);
    _motor_initial_speed_ = 100;

    ping();
    _move_behind_detected_distance_ = feetToCm(range());
    start_process(detect_distance_sonar());

    move_behind(85.0, 1.0, 30);

    printf("wait\n");

    stop_process(_detect_distance_process_running_);
    wait_process(_detect_distance_process_running_);

    printf("scan\n");

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
