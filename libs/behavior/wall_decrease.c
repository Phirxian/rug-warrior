/**
 * Run at a maximum speed until an obstacle is detected 100 cm away,
 * then slow down until the obstacle is 30 cm away, then stop.
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

    printf("Wall decrease behavior\n");

    set_servo(0);
    sleep(0.175);

    ping();
    _move_behind_detected_distance_ = feetToCm(range());
    start_process(detect_distance_sonar());

    move_behind(100.0, 1.0, 30);

    _detect_distance_process_running_ = 0;
    while(_detect_distance_process_running_ != 1);

    set_servo(0);
    sonar_servo_off();
}
