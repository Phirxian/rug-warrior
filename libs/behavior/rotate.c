#define SLEEP 2.
/*
 * Rotation tests, using one or both motors
 * with +/- a given angle
 * @param degree Angle in degrees
 */
void rotate_test_all_for(int degree)
{
    printf("Center %d\n", degree);
    rotate(C_MOTOR, (int)(((float)(degree))*1.8));
    sleep(SLEEP);

    printf("Inv center %d\n", -degree);
    rotate(C_MOTOR, -(int)(((float)(degree))*1.8));
    sleep(SLEEP);

    printf("Left %d\n", degree);
    rotate(L_MOTOR, degree);
    sleep(SLEEP);

    printf("Inv left %d\n", -degree);
    rotate(L_MOTOR, -degree);
    sleep(SLEEP);

    printf("Right %d\n", degree);
    rotate(R_MOTOR, degree);
    sleep(1.);

    printf("Inv right %d\n", -degree);
    rotate(R_MOTOR, -degree);
    sleep(SLEEP);
}

/*
 * Rotation tests
 * Last update: -
 * @version Final
 */
void main()
{
    init_motors();

    _motor_initial_speed_ = 80;
    printf("Rotate sample using %d speed\n", _motor_initial_speed_);

    /*rotate_test_all_for(45);
    rotate_test_all_for(90);*/
    rotate_test_all_for(180);
    /*rotate_test_all_for(270);*/
    /*rotate_test_all_for(360);*/

    printf("end\n");
}
