/**
 * Run at a maximum speed until an obstacle is detected 85 cm away, then stop.
 * 30 represents the minimum speed the robot will run at.
 * These 2 values are passed to move_behind()
 *
 * Last update: -
 * @version Final
 */
void main()
{
    init_motors();
    init_sonar();
    sonar_init_servo();

    printf("Runs until a wall is found 85cm\n");

    set_servo(0);
    sleep(0.175);

    ping();
    _move_behind_detected_distance_ = feetToCm(range());
    start_process(detect_distance_sonar());

    move_behind(85.0, 1.0, 30);

    _detect_distance_process_running_ = 0;
    while(_detect_distance_process_running_ != 1);

    set_servo(0);
    sonar_servo_off();
}
