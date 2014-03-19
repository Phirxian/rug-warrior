/**
 * Move forward, and when the skirt is pushed,
 * escape from an obstacle (move back then rotate)
 * 
 * Last update: 14:25, Mar. 4, 2014
 * @version 1.1, utilisation des 2 moteurs pour accelerer la rotation par 2
 */
void main()
{
    int bmp; /* Store, temporarily the bumper values */

    /* Initialize all components needed to run the robot */
    init_motors();

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
        if (bmp != 0 && bmp != BMP_BACK)
        {
            /* Stop the robot */
            _running_process_running_ = 0;
            while(_running_process_running_ != -1);
            escape(bmp);
            /* The robot can go foward after the dodge */
            _motor_initial_speed_ = 100;
            start_process(running_forever());
        }
    }
}
