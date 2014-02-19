#define R_MOTOR 1
#define L_MOTOR 2

int _left_enc_counts_ = 0;
int _right_enc_counts_ = 0;
int _motor_initial_speed_ = 0;
int _motor_right_speed_ = 0;
int _motor_left_speed_ = 0;
int _motor_distance_ = 0;

/**
 * start it with thread
 * dependency : encoder_aux() process
 */
void running()
{
    int diff;

    int inv = (-1)*(_motor_initial_speed_ < 0) +
                   (_motor_initial_speed_ > 0);

    _motor_right_speed_ = _motor_initial_speed_;
    _motor_left_speed_ = _motor_initial_speed_;

    while(1)
    {
        diff = _right_enc_counts_ - _left_enc_counts_;

        _motor_right_speed_ -= inv*diff;
        _motor_left_speed_ += inv*diff;

        if(_left_enc_counts_ >= _motor_distance_) _motor_left_speed_ = 0;
        if(_right_enc_counts_ >= _motor_distance_) _motor_right_speed_ = 0;

        motor(0, inv*abs(_motor_left_speed_));
        motor(1, inv*abs(_motor_right_speed_));
    }
}

/**
 * count the switching state of encoders
 * value are stored into
 * _left_enc_counts_ and _right_enc_counts_
 * use this function under parallele thread
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
 * speed : [-100 : 0] U [0 : 100]
 */
void rotate(int flags, float angle, int speed)
{
    int invert;
    float radius = 14.;

    if(flags < 0 || flags > 3)
        return;

    invert = 1;

    if(flags == 3)
    {
        angle /= 2.;
        invert = -1;
        radius /= 2.;
    }

    motor(0, speed*(flags & 1));
    motor(1, speed*(flags & 2)*invert);

    /* calculate the time for the angle */
    sleep(feetToMotor(cmToFeet((angle/360.)*2.*PI*radius), speed));

    stop();
}
