#define SHAFT_MULT 3
#define SHAFT_DIFF 12

/**
 * Move forward, and when a wall is detected, follow it
 * Last update: -
 * @version -
 */
void main()
{
    int ir_current; /* Store, temporarily the ir values */
    int ir_previous = 0;
    int light;

    /* Initialize all components needed to run the robot */
    init_motors();

    _motor_initial_speed_ = 100;
    start_process(running_forever());

    while(1)
    {
        int enc_dif = _right_enc_counts_-_left_enc_counts_;
        /*
         * Depending on the ir_detect() value),
         * make it rotate, and move forward again
         */
        ir_current = ir_detect();

        /* Something is detected */
        if(ir_current != 0)
        {
            if(ir_current == OBSTACLE_FRONT)
            {
                printf("FRONT\n");
                /* Stop the robot */
                stop_process(_running_process_running_);
                wait_process(_running_process_running_);

                /* Check the light difference */
                light = light_diff();
                rotate(C_MOTOR, ((light < 0) - (light >= 0))*90);

                /* The robot can go foward after the dodge */
                start_process(running_forever());
                ir_previous = 0;
            }
            else
            {
                printf("move out %d\n", 10 * ((ir_previous == OBSTACLE_LEFT) - (ir_previous == OBSTACLE_RIGHT)));
                /* Stop the robot */
                stop_process(_running_process_running_);
                wait_process(_running_process_running_);

                rotate(C_MOTOR, 10 * ((ir_current == OBSTACLE_LEFT) - (ir_current == OBSTACLE_RIGHT)));

                /* The robot can go foward after the dodge */
                start_process(running_forever());
                ir_previous = ir_current;
            }

        }
        else if(ir_previous)
        {
            printf("move to %d\n", -10 * ((ir_previous == OBSTACLE_LEFT) - (ir_previous == OBSTACLE_RIGHT)));
            /* Stop the robot */
            stop_process(_running_process_running_);
            wait_process(_running_process_running_);

            /* Check the light difference */
            rotate(C_MOTOR, -10 * ((ir_previous == OBSTACLE_LEFT) - (ir_previous == OBSTACLE_RIGHT)));

            /* The robot can go foward after the dodge */
            start_process(running_forever());
        }

        sleep(0.1);
    }
}
