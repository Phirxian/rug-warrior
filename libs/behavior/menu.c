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

int ir_previous = 0;

void maze_process()
{
    int ir_current;
    int ir_tmp;
    long timer = 0L;

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


                if(timer_done(timer) || timer == 0L)
                {
                    rotate(C_MOTOR, -70*(ir_previous == OBSTACLE_RIGHT));
                    rotate(C_MOTOR,  70*(ir_previous != OBSTACLE_RIGHT));
                    timer = timer_create_mseconds(1000L);
                }
                else
                {
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

                    timer = timer_create_mseconds(500L);
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
 * Allow the user to select a program<br>
 *  a)  Avancer de 1m 20 avec une tolérance de 1 cm<br>
 *  b)  Rotate with an axis (well) de 45/90/180/270/360/a°<br>
 *  c)  Escape on envirnoment when Rug-Warrio is hit by somethink<br>
 *  d)  Stop at 85cm of a wall<br>
 *  e)  Stop at 85cm of wall then scan around the robot with Rug-Bat<br>
 *  f)  Running to a wall (1m) and decrease his speed to 50cm<br>
 *  g)  Escape obstacle environment whit IR<br>
 *  h)  Comportement C + G<br>
 *  i)  Follow a light<br>
 *  j)  Stop the robot at 20cm of a light<br>
 *  k)  Follow a wall whit IR<br>
 *  l)  Out of maze<br>
 */
void main()
{
    sleep(0.75);
    test_number++;

    if((test_number > 14) | (test_number < 0))
        test_number = 0;

    while(test_number == 1)
    {
        printf("Rug Warrior Pro for Kam C.\n");
        sleep(1.5);
        printf("Press RESET to  select test\n");
        sleep(1.5);
        printf("VAYSSADE Jehan-Antoine\n");
        sleep(1.5);
        printf("MATEOS Christophe\n");
        sleep(1.5);
        printf("SERENE Guilhem\n");
        sleep(1.5);
    }

    /**
     * Run over a given distance in centimeters
     * The distance is the first number inside
     * _motor_distance_ (default: 120)
     *
     * Last update: 16:16, Fri. 20 Mar.
     * @version Final
     */
    if(test_number == 2)
    {
        _motor_distance_ = (int)(120./1.178)-1;
        _motor_initial_speed_ = 100;
        printf("Running for\n1.20 meter");
        running();
    }
    /**
     * Rotation tests
     * Last update: 16:16, Fri. 20 Mar.
     * @version Final
     */
    else if(test_number == 3)
    {
        _motor_initial_speed_ = 50;
        printf("Rotate sample using %d speed\n", _motor_initial_speed_);
        sleep(1.);
        rotate_test_all_for(180);
        printf("Finished!\n");
    }
    /**
     * Move forward, and when the skirt is pushed,
     * escape from an obstacle (move back then rotate)
     *
     * Last update: 14:25, Mar. 4, 2014
     * @version 1.1, utilisation des 2 moteurs pour accelerer la rotation par 2
     */
    else if(test_number == 4)
    {
        int bmp; /* Temporarily store the bumpers value */
        _motor_initial_speed_ = 100;
        start_process(running_forever());

        while(1)
        {
            /*
             * Depending on the pushed bumper(s),
             * make it rotate in the opposite way,
             * and move forward again
             */
            bmp = bumper_detection();

            /* LEFT OR RIGHT  bumper is touched */
            if(bmp != 0 && bmp != BMP_BACK)
            {
                /* Stop the robot */
                _running_process_running_ = 0;

                while(_running_process_running_ != -1);

                escape(bmp);
                /* The robot can go forward after the dodge */
                _motor_initial_speed_ = 100;
                start_process(running_forever());
            }
        }
    }
    /**
     * Run at a maximum speed until an obstacle is detected 85 cm away, then stop.
     * 30 represents the minimum speed the robot will run at.
     * These 2 values are passed to move_behind(), and can be edited at wish.
     *
     * The cartography will be run only after a second reset button hit
     *
     * Last update: 16:37, Fri. 20 Mar.
     * @version Final
     */
    else if(test_number == 6 || test_number == 5)
    {
        int scan;
        float last;

        init_sonar();
        sonar_init_servo();

        printf("Run until the sonar finds a wall 85 cm\n");
        _motor_initial_speed_ = 100;

        set_servo(0);
        sleep(0.175);

        ping();
        _move_behind_detected_distance_ = feetToCm(range());

        start_process(detect_distance_sonar());
        move_behind(85.0, 1.0, 30);

        join_process(_detect_distance_process_running_);

        /* Cartography */
        if(test_number == 6)
        {
            for(scan = -100; scan<100; ++scan)
            {
                set_servo(scan);
                ping();
                last = feetToCm(range());
                printf("Scan %f\n", last);
            }
        }

        set_servo(0);
        sonar_servo_off();
    }
    /**
     * wall decresase, start at max speed between [oo ; 1meter]
     * then decrese to 50 cm
     */
    else if(test_number == 7)
    {
        int pid;

        printf("Go, Slow & Stop at 50cm\n");
        _motor_initial_speed_ = 100;

        sleep(1.0);
        init_sonar();
        sonar_init_servo();

        set_servo(0);
        sleep(0.175);

        ping();
        _move_behind_detected_distance_ = feetToCm(range());
        start_process(detect_distance_sonar());

        /**
         * run util 1meter as found
         */
        pid = start_process(running_forever());

        while(_move_behind_detected_distance_ > 100.0)
            sleep(_process_yield_time_);

        kill_process(pid);

        /**
         * decrese speed for move_behind(50.0, 1.0, 30)
         */
        move_behind(50.0, 1.0, 30);

        /**
         * clear process
         */
        stop_process(_detect_distance_process_running_);
        wait_process(_detect_distance_process_running_);

        set_servo(0);
        sonar_servo_off();
    }
    /**
     * Move forward, and when an obstacle is detected,
     * escape from it (move back then rotate)
     *
     * Last update: 16:45, Fri. 20 Mar.
     * @version Final
     */
    else if(test_number == 8)
    {
        int pid;
        int ir;  /* Temporarily store the ir_detect() value */

        printf("Escape distance\n");
        _motor_initial_speed_ = 50;
        pid = start_process(running_forever());

        while(1)
        {
            /*
             * Depending on the value returned by ir_detect(),
             * make it rotate in the opposite way,
             * and move forward again
             */
            ir = ir_detect();

            /* ir has to be different from 0 */
            if(ir != OBSTACLE_NONE)
            {
                /* Stop the robot */
                kill_process(pid);
                escape(ir);
                /* The robot can go forward after the detection */
                _motor_initial_speed_ = 50;
                pid = start_process(running_forever());
            }
        }
    }
    /**
     * Move forward, and when the skirt is pushed,
     * escape from an obstacle (move back then rotate)
     * @see escape.c
     *
     * When an obstacle is detected by ir,
     * escape from it (move back then rotate)
     * @see escape_distant.c
     *
     * Last update: 9:25, Mar. 19, 2014
     * @version 1.0
     */
    else if(test_number == 9)
    {
        int pid;
        int bmp; /* Store, temporarily the bumper values */
        int ir;  /* Store, temporarily the ir values */

        printf("Escape from contact and distance\n");

        _motor_initial_speed_ = 50;
        start_process(running_forever());

        while(1)
        {
            /*
             * Depending on the pushed bumper(s),
             * make it rotate in the opposite way,
             * and move forward again
             */
            bmp = bumper_detection();
            /*
             * Depending on the value returned by ir_detect(),
             * make it rotate in the opposite way,
             * and move forward again
             */
            ir = ir_detect();

            /* LEFT OR RIGHT  bumper is touched */
            if((bmp != 0 && bmp != BMP_BACK) || ir != OBSTACLE_NONE)
            {
                /* Stop the robot */
                _running_process_running_ = 0;

                while(_running_process_running_ != -1);

                if(ir) escape(ir);
                else   escape(bmp);

                /* The robot can go foward after the dodge */
                _motor_initial_speed_ = 50;
                start_process(running_forever());
            }
        }
    }
    /**
     * follow the light
     */
    else if(test_number == 10)
    {
        printf("Follow the light\n");
        sleep(1.0);
        light_environnement();
        _motor_initial_speed_ = 50;
        _mode_light_ = GO_LIGHT;
        go_light_touch();
    }
    /**
     * run to the light to 20cm
     */
    else if(test_number == 11)
    {
        printf("Follow and stop at 20 cm from a light\n");
        sleep(1.0);
        _mode_light_ = GO_LIGHT;
        light_environnement();
        _motor_initial_speed_ = 50;
        go_light_stop(20);
    }
    /**
     * Move forward, and when a wall is detected, follow it
     */
    else if(test_number == 12)
    {
        int ir_current;
        int ir_previous = 0;
        int light;

        printf("Following Wall\n");
        sleep(1.0);

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
    else if(test_number == 13)
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
}
