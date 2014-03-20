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
            _go_light_running_ = 0;

            while(_go_light_running_ != -1);

            escape(ir);
            /* Run after the correction*/
            start_process(go_light());
        }
    }

    _go_light_running_ = 0;

    while(_go_light_running_ != -1);

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
            _go_light_running_ = 0;

            while(_go_light_running_ != -1);

            escape(bmp);
            /* Run after the correction*/
            start_process(go_light());
        }
    }

    _go_light_running_ = 0;

    while(_go_light_running_ != -1);

    _go_light_touch_running_ = -1;
}


void main()
{
    light_environnement();
    init_motors();
    _motor_initial_speed_ = 50;
    _mode_light_ = GO_LIGHT;
    go_light_touch();
}
