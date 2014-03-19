/**
 * Move forward, and when an obstacle is detected,
 * escape from it (move back then rotate)
 * 
 * Last update: -
 * @version Final
 */
void main()
{
    int pid;
    int ir;  /* Temporarily store the ir_detect() value */

    /* Initialize all components needed to run the robot */
    init_motors();

    _motor_initial_speed_ = 100;
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
            _motor_initial_speed_ = 100;
            pid = start_process(running_forever());
        }
    }
}
