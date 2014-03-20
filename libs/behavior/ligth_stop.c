int MAX_STOP_DISTANCE = 30; /* The maximum distance of the detection */
float CM_TO_LIGHT_COEFFICIENT = 8.0/20.0; /* Linear coefficient obtained after tests */

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
    distance = cmToPhotons(distance);
    margin = cmToPhotons(_motor_initial_speed_/10);
    avCells = (analog(photo_left) + analog(photo_right))/2;
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

        display_info(); /* Display the difference and values of photo cells */

        avCells = (analog(photo_left) + analog(photo_right))/2; /* Refresh the average of cells */
        goForward = (avCells > distance+margin) - (avCells < distance-margin);
        driveb(goForward*_motor_initial_speed_, diff);


    } /* The robot is on the right position */

    stop();
    _go_light_stop_running_ = -1;
}


/**
 * Calculate the distance to stop the robot => Set prealablemently the "_light_stop_distance_" to stop (in cm)
 * Take care of the stop distance
 * CAUTION : If the distance it superior to will be too high the distance will be set to -1
 *           If the more environnement is light, the less accurancy is.
 */
int cmToPhotons(int distance)
{
    if(distance > MAX_STOP_DISTANCE)
    {
        distance = -1;
    }
    else
    {
        distance = (int)((float)distance * CM_TO_LIGHT_COEFFICIENT);
    }
}

void main()
{
    init_motors();
    _mode_light_ = GO_LIGHT;
    light_environnement();
    _motor_initial_speed_ = 50;
    go_light_stop(20);
}
