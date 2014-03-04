#define R_MOTOR 1
#define L_MOTOR 2
#define C_MOTOR (R_MOTOR | L_MOTOR)

int _left_enc_counts_ = 0;
int _right_enc_counts_ = 0;
int _motor_initial_speed_ = 0;
int _motor_right_speed_ = 0;
int _motor_left_speed_ = 0;
int _motor_distance_ = 0;

int _running_process_running_ = 0;

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

        sleep(0.1);
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

        sleep(0.1);
    }

    motor(0, 0); motor(1, 0);
    _running_process_running_ = -1;
}

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
 * calculate the time needed to run
 * in x feet with given speed [-100 : 100]
 */
float feetToMotor(float feet, int speed)
{
    float tmp = (float)speed;
    float inv = 100./tmp;
    return (feet/0.67)*inv;
}

/**
 * flags : R_MOTOR, L_MOTOR, R_MOTOR | L_MOTOR
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

    needed = (int)(480.*((float)(angle))/2160.);

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

void move_behind(float distance, float marging)
{
    int pid;
    float last;
    int scan;

    init_motors();
    init_sonar();
    sonar_init_servo();

    set_servo(0);
    sleep(0.175);

    start_process(encoder_aux());

    while(1)
    {
        ping();
        last = feetToCm(range());

        printf("%f\n", last);

        if(last >= (distance+marging))
        {
            kill_process(pid);
            _running_process_running_ = 0;
            _motor_initial_speed_ = 50+(int)(last-(distance+marging));
            pid = start_process(running_forever());
        }
        else if(last <= (distance-marging))
        {
            kill_process(pid);
            _running_process_running_ = 0;
            _motor_initial_speed_ = -50+(int)(-(distance-marging).+last);
            pid = start_process(running_forever());
        }
        else if(!_motor_initial_speed_) break;
        else break;

        sleep(0.175);
    }

    _running_process_running_ = 0;
    while(_running_process_running_ != -1);
    set_servo(0);
    sonar_servo_off();
    stop();
}

/** A revoir, avec following light
 * Adjust the direction clockwise (right) or anti-clockwise (left)
 * USED WITH : running() or running_forever() 
 * @param 100 (clockwise) between -100 (anticlockwise)
 */
void direction(int clockwise) {
    
    /* BAD PARAM */
    if (clockwise < -100 || clockwise > 100) {
        return;
    }
    
    /* Reset param to avoid problems*/
    _motor_right_speed_ = 100;
    _motor_left_speed_ = 100; 
    
    /* Ajust the direction*/
    if (clockwise < 0) {
        _motor_right_speed_ -=clockwise; /* Slowing the right motor to anticlockwise*/
    } else if (clockwise > 0) {
        _motor_left_speed_ += clockwise; /* Slowing the left motor to clockwise*/
    }
    
}

