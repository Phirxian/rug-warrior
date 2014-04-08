#define SHAFT_MULT 3
#define SHAFT_DIFF 12

/**
 * Move forward, and when a wall is detected, follow it
 * Last update: -
 * @version -
 */
void main()
{
    int ir_current;
    int ir_previous = 0;
    int light;

    _motor_initial_speed_ = 50;
    start_process(running_forever());

    while(1)
    {
        /*
         * Depending on the ir_detect() value),
         * make it rotate, and move forward again
         */
        ir_current = ir_detect();

        /* Something is detected */
        if(ir_current != 0)
        {
            stop_process(_running_process_running_);
            wait_process(_running_process_running_);

            /**
             * found an angle where ir doesn't detect
             * whitout sleeping & restarting running process
             */

            while(ir_current != 0)
            {
                if(ir_current == OBSTACLE_FRONT)
                {
                    printf("FRONT\n");

                    if(ir_previous)
                    {
                        rotate(C_MOTOR, -30*(ir_previous == OBSTACLE_RIGHT));
                        rotate(C_MOTOR,  30*(ir_previous == OBSTACLE_LEFT));
                    }
                    else
                    {
                        light = light_diff();
                        rotate(C_MOTOR, ((light < 0) - (light >= 0))*90);
                    }
                }
                else
                {
                    printf("move out\n");

                    rotate(C_MOTOR, -30 * (ir_current == OBSTACLE_RIGHT));
                    rotate(C_MOTOR, 30 * (ir_current == OBSTACLE_LEFT));

                    ir_previous = ir_current;
                }
                ir_current = ir_detect();
            }

            start_process(running_forever());
        }
        else if(ir_previous)
        {
            printf("move to\n");

            stop_process(_running_process_running_);
            wait_process(_running_process_running_);

            rotate(L_MOTOR, 10 * (ir_previous == OBSTACLE_RIGHT));
            rotate(R_MOTOR, 10 * (ir_previous == OBSTACLE_LEFT));

            start_process(running_forever());
        }

        sleep(0.05);
    }
}
