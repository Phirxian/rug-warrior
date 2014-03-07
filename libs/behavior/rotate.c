void rotate_test_all_for(int degree)
{
    printf("center %d\n", degree);
    rotate(C_MOTOR, degree);
    sleep(1.);

    printf("inv center %d\n", -degree);
    rotate(C_MOTOR, -degree);
    sleep(1.);

    printf("left %d\n", degree);
    rotate(L_MOTOR, degree);
    sleep(1.);

    printf("inv left %d\n", -degree);
    rotate(L_MOTOR, -degree);
    sleep(1.);

    printf("right %d\n", degree);
    rotate(R_MOTOR, degree);
    sleep(1.);

    printf("inv right %d\n", -degree);
    rotate(R_MOTOR, -degree);
    sleep(1.);
}

void main()
{
    init_motors();

    _motor_initial_speed_ = 100;
    printf("rotate sample using %d speed\n", _motor_initial_speed_);

    rotate_test_all_for(45);
    rotate_test_all_for(90);
    rotate_test_all_for(180);
    rotate_test_all_for(270);
    rotate_test_all_for(360);

    printf("end\n");
}
