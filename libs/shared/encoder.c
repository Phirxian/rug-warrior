/**
 * count the switching state of encoders
 * value are stored into
 * _left_enc_counts_ and _right_enc_counts_
 * use this function under parallel thread
 */
void encoder_aux()
{
    int l_old, r_old, l_new, r_new;

    while(1)
    {
        l_new = left_shaft();
        r_new = right_shaft();

        if(l_old & ~l_new)
            _left_enc_counts_++;

        if(r_old & ~r_new)
            _right_enc_counts_++;

        l_old = l_new;
        r_old = r_new;
    }
}

/**
 * reset encoders count
 */
void reset_encoder_aux()
{
    while(_left_enc_counts_)
    {
        _left_enc_counts_ = 0;
        sleep(0.001);
    }
    while(_right_enc_counts_)
    {
        _right_enc_counts_ = 0;
        sleep(0.001);
    }
}

/**
 * check if encoder_aux() process
 * are not in running state
 * can be not thread-safe
 */
void encoder_reset()
{
    _left_enc_counts_ = 0;
    _right_enc_counts_ = 0;
}

/**
 * use this to chech if your
 * your running process or
 * runing forever process or
 * move_behind process is not
 * blocked by somethink
 * (shaft does not change)
 */
void check_encoder()
{
    int last_right;
    int last_left;

    while(1)
    {
        sleep(_check_enc_yield_time_);
        if(last_right == _right_enc_counts_ &&
           last_left == _left_enc_counts_ &&
           _right_enc_counts_ != 0)
        {
            _running_process_running_ = 0;
            _move_behind_process_running_ = 0;
            /* stop rotation */
            _motor_left_speed_ = 0;
            _motor_right_speed_ = 0;
        }
        last_right = _right_enc_counts_;
        last_left = _left_enc_counts_;
    }
}
