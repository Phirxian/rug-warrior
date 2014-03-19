/*
 * Run until an obstacle is detected 85 cm away, then
 * scan the area at 180 degrees;
 * Display the detected obstacle distances on the digital screen
 *
 * Last update: -
 * @version Final
 */
void main()
{
    int scan;

    init_motors();
    init_sonar();
    sonar_init_servo();

    printf("Run until a wall is found then scan\n");

    set_servo(0);
    sleep(0.175);

    ping();
    _move_behind_detected_distance_ = feetToCm(range());
    start_process(detect_distance_sonar());

    move_behind(85.0, 1.0, 30);

    _detect_distance_process_running_ = 0;
    while(_detect_distance_process_running_ != 1);

    for(scan = -100; scan<100; ++scan)
    {
        set_servo(scan);
        ping();
        last = feetToCm(range());
        printf("Scan %f\n", last);
    }

    set_servo(0);
    sonar_servo_off();
}
