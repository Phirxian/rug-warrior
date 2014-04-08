float PI = 3.1415956;

/**
 * convert cm to feet
 */
float cmToFeet(float cm)
{
    return cm/30.48;
}

/**
 * convert feet to cm
 */
float feetToCm(float cm)
{
    return cm*30.48;
}

/**
 * convert radian to degree
 */
float radToDeg(float rad)
{
    return rad*57.2957795;
}

/**
 * convert degree to radian
 */
float degToRad(float deg)
{
    return deg/57.2957795;
}

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
 * Calculate the distance to stop the robot => Set prealablemently the "_light_stop_distance_" to stop (in cm)
 * Take care of the stop distance
 * CAUTION : If the distance it superior to will be too high the distance will be set to -1
 *           If the more environnement is light, the less accurancy is.
 */
int cmToPhotons(int distance)
{
    if(0 > distance || distance > MAX_STOP_DISTANCE)
    {
        return 0;
    }
    return (int)((float)distance * CM_TO_LIGHT_COEFFICIENT);
}

/**
 * calculate the difference between two value
 * of the photo sensor and aproximate is termal value
 */
int thermometer(int val, int low, int high)
{
    int range = high - low;
    int blks = (((val - low) * 16) + (range >> 1)) / range;
    int j;

    if(blks < 0)
        blks = 0;
    else if(blks > 16)
        blks = 16;

    for(j = 1; j <= blks; j++)
        printf("%c",219);  /* 219 => [] */

    for(j = blks; j < 16; j++)
        printf("-");

    return blks;
}

/**
 * Count the number of digits in an integer
 * (negative numbers add 1 for - sign)
 */
int digits(int signed_val)
{
    int digs, val;
    val = abs(signed_val);

    if(val < 100)
    {
        if(val < 10) digs = 1;
        else digs = 2;
    }
    else if(val < 10000)
    {
        if(val < 1000) digs = 3;
        else digs = 4;
    }
    else digs = 5;

    if(signed_val >= 0) return digs;
    else return (digs + 1);
}
