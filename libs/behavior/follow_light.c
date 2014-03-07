/** INSTABLE, A TESTER et à revoir */

/**
 * The robot following/avoid a light infinitely and he escape obstacle
 * @param mode if mode != 0, the robot go to the light
 *                        if mode == 0, the robot go to the dark
 */
void go_light_dark(int mode) {

    /* Initialize motor at the maximum speed */
    _motor_initial_speed_ = 100;

    while(1) {

        int diff;
        diff = light_diff();

        /* Invert the mode */
        if (mode == 0) {
            diff= -diff;
        }

        print_diff(diff);
        direction(diff);
        sleep(1.0);
    }


}

/* A TESTER, ATTENTION, le robot ne semble que faire escape */
/* Add the motor and the escaping to the robot */
void follow_light() {

    /* Initialize all components needed to run the robot */
  	init_motors();
    start_process(escape());
    start_process(go_light_dark(1));

}

void main() {
    follow_light();
}

