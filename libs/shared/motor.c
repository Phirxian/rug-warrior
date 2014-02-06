#define R_MOTOR 1
#define L_MOTOR 2

/**
 * Conversion cm to feet
 * @param cm - the distance in centimeters used to be converted
 * @return the distance converted in feet
 */
float cmToFeet(float cm)
{
    return cm/30.48;
}

/*
 * calculate the time needed to run
 * in x feet with given speed [-100 : 100]
 */
float feetToMotor(float feet, int speed)
{
    float tmp = (float)speed;
    float inv = 100./tmp;
    return (feet/0.67)*inv;
}

/*
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
