void main()
{
    init_motors();
    init_sonar();
    sonar_init_servo();

    printf("Follow a light source 20 cm away\n");

    set_servo(0);
    sleep(0.175);

    ping();
    _move_behind_detected_distance_ = feetToCm(range());
    start_process(detect_distance_sonar());

    move_behind(20.0, 1.0, 30);

    _detect_distance_process_running_ = 0;
    while(_detect_distance_process_running_ != 1);

    set_servo(0);
    sonar_servo_off();
}
