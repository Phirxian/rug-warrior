/**
 * calculate the approximate time needed to run
 * in @feet with given speed [-100 : 100]
 */
float feetToMotor(float feet, int speed)
{
    float tmp = (float)speed;
    float inv = 100./tmp;
    return (feet/0.67)*inv;
}

/**
 * start it with thread
 * dependency : encoder_aux() process
 * ! reset the encoders count
 */
void running()
{
    int diff;
    int inv = (-1)*(_motor_initial_speed_ < 0) +
              (_motor_initial_speed_ > 0);
    _motor_right_speed_ = inv*_motor_initial_speed_;
    _motor_left_speed_ = inv*_motor_initial_speed_;
    _running_process_running_ = 1;
    reset_encoder_aux();

    while(_running_process_running_ && (_motor_left_speed_ > 0 || _motor_right_speed_ > 0))
    {
        diff = _right_enc_counts_ - _left_enc_counts_;

        if(_motor_right_speed_ - diff > 0)
            _motor_right_speed_ -= diff;

        if(_motor_left_speed_ + diff > 0)
            _motor_left_speed_ += diff;

        if(_left_enc_counts_ >= _motor_distance_)
            _motor_left_speed_ = 0;

        if(_right_enc_counts_ >= _motor_distance_)
            _motor_right_speed_ = 0;

        motor(0, inv*_motor_left_speed_);
        motor(1, inv*_motor_right_speed_);
        sleep(_process_yield_time_);
    }

    _running_process_running_ = -1;
}

/**
 * start it with thread
 * dependency : encoder_aux() process
 * ! reset the encoders count
 * same behavior as running()
 * but running forever
 */
void running_forever()
{
    int diff;
    int inv = (-1)*(_motor_initial_speed_ < 0) +
              (_motor_initial_speed_ > 0);
    _motor_right_speed_ = inv*_motor_initial_speed_;
    _motor_left_speed_ = inv*_motor_initial_speed_;
    _running_process_running_ = 1;
    reset_encoder_aux();

    while(_running_process_running_)
    {
        diff = _right_enc_counts_ - _left_enc_counts_;

        if(_motor_right_speed_ - diff > 0)
            _motor_right_speed_ -= diff;

        if(_motor_left_speed_ + diff > 0)
            _motor_left_speed_ += diff;

        motor(0, inv*_motor_left_speed_);
        motor(1, inv*_motor_right_speed_);
        sleep(_process_yield_time_);
    }

    motor(0, 0);
    motor(1, 0);
    _running_process_running_ = -1;
}

/**
 * flags : R_MOTOR, L_MOTOR, C_MOTOR
 * angle : degree
 * dependency process: encoder_aux
 * @see _motor_initial_speed_
 * ! reset the encoders count
 */
void rotate(int flags, int angle)
{
    int invert;
    int needed;
    int adjust;
    float radius;

    if(flags < 0 || flags > 3)
        return;

    radius = 14.;
    invert = 1;
    adjust = -(angle < 0) + (angle > 0);
    angle = (angle < 0)*(-angle) + (angle > 0)*angle;

    if(flags == 3)
    {
        angle /= 2;
        invert *= -1;
        radius /= 2.;
    }

    _motor_right_speed_ = adjust*_motor_initial_speed_*(flags & 1)*invert;
    _motor_left_speed_ = adjust*_motor_initial_speed_*(flags & 2);
    reset_encoder_aux();
    needed = (int)(474.*((float)(angle))/2160.);

    while(_motor_left_speed_ || _motor_right_speed_)
    {
        if(flags & 1 && _right_enc_counts_ >= needed)
            _motor_right_speed_ = 0;

        if(flags & 2 && _left_enc_counts_ >= needed)
            _motor_left_speed_ = 0;

        motor(0, _motor_left_speed_);
        motor(1, _motor_right_speed_);
    }
}

/**
 * move the robot in front of object to the distance
 * of @distance in cm
 * @margin > 0.22 (distance between two shaft)
 * @distance and @margin need to have the same sign
 */
void move_behind(float distance, float marging, int minimal_speed)
{
    float last;
    int scan;
    int pid;
    pid = 0;
    _move_behind_process_running_ = 1;
    _running_process_running_ = -1;

    while(_move_behind_process_running_)
    {
        last = _move_behind_detected_distance_;

        if(last >= (distance+marging))
        {
            _running_process_running_ = 0;

            while(_running_process_running_ >= 0 && pid);

            _motor_initial_speed_ = minimal_speed+(int)(last-(distance+marging));
            pid = start_process(running_forever());
        }
        else if(last <= (distance-marging))
        {
            _running_process_running_ = 0;

            while(_running_process_running_ >= 0 && pid);

            _motor_initial_speed_ = -minimal_speed+(int)(-(distance-marging)+last);
            pid = start_process(running_forever());
        }
        else if(!_motor_initial_speed_) break;
        else break;

        sleep(_process_yield_time_);
    }

    _running_process_running_ = 0;

    while(_running_process_running_ != -1);

    _move_behind_process_running_ = -1;
}

/**
 * parrallele processes for distance update in move_behind
 */
void detect_distance_sonar()
{
    _detect_distance_process_running_ = 1;

    while(_detect_distance_process_running_)
    {
        ping();
        _move_behind_detected_distance_ = feetToCm(range());
        printf("%f\n", _move_behind_detected_distance_);
        sleep(_process_yield_time_);
    }

    _detect_distance_process_running_ = -1;
}
