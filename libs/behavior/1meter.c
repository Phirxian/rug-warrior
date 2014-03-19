/**
 * Run over a distance of 1.20 meters
 *
 * Last update: -
 * @version Final
 */
void main()
{
    _motor_distance_ = (int)(600./1.178)-1;
    _motor_initial_speed_ = 100;

    printf("Running for\n1.2 meter");
    init_motors();

    start_process(encoder_aux());
    start_process(running());

    while(1)
    {
        printf("Running Test   ");
        printn(_right_enc_counts_-_left_enc_counts_,5);
        printf(" ");
        printn(_motor_right_speed_,5);
        printn(_motor_left_speed_,5);
        printf("\n");

        sleep(0.1);
    }
}
