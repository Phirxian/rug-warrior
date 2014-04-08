/**
 * The robot following/avoid a light infinitely
 * @global _mode_light_ it must be set
 */
void go_light()
{
    _go_light_running_ = 1;

    while(_go_light_running_)
    {
        int diff;
        diff = light_diff();
        diff *= _light_environnement_; /* Go more efficiently to the light specifing the environnement */

        /* Invert the mode */
        if(_mode_light_ == GO_DARK)
        {
            diff= -diff;
        }

        print_diff(diff);
        driveb(_motor_initial_speed_, diff);
    }

    stop();
    _go_light_running_ = -1;
}


/**
 * Avoid objects and go to the light or dark
 * @global _mode_light_ it must be set
 */
void go_light_detection()
{
    int ir;
    start_process(go_light());
    _go_light_detection_running_ = 1;

    while(_go_light_detection_running_)
    {
        /* Obstacle detected, I stop the run and I am escaping remotely */
        if((ir = ir_detect()) != OBSTACLE_NONE)
        {
            stop_process(_go_light_running_);
            wait_process(_go_light_running_);

            escape(ir);
            /* Run after the correction*/
            start_process(go_light());
        }
    }

    stop_process(_go_light_running_);
    wait_process(_go_light_running_);

    _go_light_detection_running_ = -1;
}

/**
 * Escape objects and go to the light or dark
 * @global _mode_light_ it must be set
 */
void go_light_touch()
{
    int bmp;
    start_process(go_light());
    _go_light_touch_running_ = 1;

    while(_go_light_touch_running_)
    {
        /* Obstacle touched, I stop the run and I am escaping */
        bmp = bumper_detection();

        if(bmp != 0 && bmp != BMP_BACK)
        {
            stop_process(_go_light_running_);
            wait_process(_go_light_running_);

            escape(bmp);
            /* Run after the correction*/
            start_process(go_light());
        }
    }

    stop_process(_go_light_running_);
    wait_process(_go_light_running_);

    _go_light_touch_running_ = -1;
}

/**
 * The robot following/avoid a light and will be stop at a distance predefinited
 * @global _mode_light_ it must be set
 */
void go_light_stop(int distance)
{
    int goForward; /* 1=> forward, 0 => STOP, -1 => backward */
    int margin; /* The margin used to stop the robot in a aera is function by the stop distance */
    int avCells; /* The average value between photo cells */

    _go_light_stop_running_ = 1;
    distance = cmToPhotons(distance+1); /* Photos cells and protection added */
    margin = cmToPhotons(_motor_initial_speed_/50);
    goForward = 1;

    while(_go_light_stop_running_ && (goForward != 0))
    {
        int diff; /* Diffference between photos cells */
        diff = light_diff();
        diff *= _light_environnement_; /* Go more efficiently to the light specifing the environnement */

        /* Invert the mode */
        if(_mode_light_ == GO_DARK)
        {
            diff= -diff;
        }

        /*display_info();*/ /* Display the difference and values of photo cells */
        avCells = (analog(photo_left) + analog(photo_right))/2; /* Refresh the average of cells */
        goForward = (avCells > (distance+margin)) - (avCells < (distance-margin));
        printf("%d - %d - %d\n", avCells, distance, margin);
        driveb(goForward*_motor_initial_speed_, diff);
    } /* The robot is on the right position */

    stop();
    _go_light_stop_running_ = -1;
}

/**
 * Running to 20cm of ligth
 * but escape obstacle with bumpers
 */
void go_light_stop_touch()
{
    int bmp;

    _mode_light_ = GO_LIGHT;
    _motor_initial_speed_ = 100;

    light_environnement();
    start_process(go_light_stop(20));

    while(_go_light_touch_running_)
    {
        bmp = bumper_detection();

        if(bmp != 0 && (bmp != BMP_BACK))
        {
            stop_process(_go_light_stop_running_);
            wait_process(_go_light_stop_running_);
            escape(bmp);
            start_process(go_light_stop(20));
        }
    }

    stop_process(_go_light_stop_running_);
    wait_process(_go_light_stop_running_);
    _go_light_touch_running_ = -1;
}
