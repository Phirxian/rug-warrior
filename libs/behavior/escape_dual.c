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
void main()
{
    int pid;
    int bmp; /* Store, temporarily the bumper values */
    int ir;  /* Store, temporarily the ir values */

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
            _motor_initial_speed_ = 100;
            start_process(running_forever());
        }
    }
}
