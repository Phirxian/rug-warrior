/*
 * save on globals
 * for killing the maze_process
 * and resetart them in main()
 * version used durring the présentation
 */
int ir_previous = 0;

void maze_process()
{
    int ir_current;
    int ir_tmp;

    init_sonar();
    sonar_init_servo();

    _motor_initial_speed_ = 50;
    start_process(running_forever());

    sleep(0.2);

    while(1)
    {
        /*
         * Depending on the ir_detect() value,
         * make it rotate, and move forward again
         */
        ir_current = ir_detect();

        /* Something is detected */
        if(ir_current != 0)
        {
            stop_process(_running_process_running_);
            wait_process(_running_process_running_);

            if(ir_current == OBSTACLE_FRONT)
            {
                printf("FRONT\n");


                rotate(C_MOTOR, -70*(ir_previous == OBSTACLE_RIGHT));
                rotate(C_MOTOR,  70*(ir_previous != OBSTACLE_RIGHT));

                /*
                 * if two consecutive front was detected
                 * then rotate
                 */

                ir_tmp = ir_detect();

                if(ir_tmp == OBSTACLE_FRONT)
                {
                    rotate(C_MOTOR,  140*(ir_previous == OBSTACLE_RIGHT));
                    rotate(C_MOTOR, -140*(ir_previous != OBSTACLE_RIGHT));
                }

                /*
                 * if three consecutive front was detected
                 * then rotate
                 */

                ir_tmp = ir_detect();

                if(ir_tmp == OBSTACLE_FRONT)
                {
                    rotate(C_MOTOR, -250*(ir_previous == OBSTACLE_RIGHT));
                    rotate(C_MOTOR,  250*(ir_previous != OBSTACLE_RIGHT));
                }

                ir_previous = 0;
            }
            else
            {
                printf("move out\n");
                rotate(C_MOTOR, -30 * (ir_current == OBSTACLE_RIGHT));
                rotate(C_MOTOR,  30 * (ir_current == OBSTACLE_LEFT));
                ir_previous = ir_current;
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

            sleep(0.175);
        }
    }
}

/**
 * follow the right wall on the maze
 * can technicali resolve the maze in a certain time
 * Last update: -
 * @version -
 */
void main()
{
    int bmp;
    int pid;

    pid = start_process(maze_process());

    while(1)
    {
        bmp = bumper_detection();
        if(bmp)
        {
            kill_process(pid);

            _motor_right_speed_ = 0;
            _motor_left_speed_ = 0;

            join_process(_running_process_running_);

            drive(-50,0);
            sleep(0.50);

            if(ir_previous)
                escape(ir_previous);
            else
                escape(bmp);

            pid = start_process(maze_process());
        }
        sleep(0.5);
    }
}
