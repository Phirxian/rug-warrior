#define R_MOTOR 1
#define L_MOTOR 2

float cmToFeet(float cm)
{
    return cm/30.48;
}

float feetToMotor(float feet, int speed)
{
    return (feet/0.67)*(100/speed);
}

void rotate(int flags, int angle, int speed)
{
    int invert;

    if(flags < 0 || flags > 3)
        return;

    invert = 1;

    if(flags == 3)
    {
        angle /= 2;
        invert = -1;
    }

    if(speed < 0)
    {
        invert *= -1;
        speed *= -1;
    }

    motor(0, speed*(flags & 1));
    motor(1, speed*(flags & 2)*invert);

    /** calculate the time for the angle **/
    /** see degree_to_pulse **/
    sleep(feetToMotor((float)angle/100.), speed);

    stop();
}
