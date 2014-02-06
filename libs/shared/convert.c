float PI = 3.1415956;

/*
 * absolute
 */
int abs(int arg)
{
    if(arg < 0)
        return -arg;
    else
        return arg;
}

/*
 * min
 */
int min(int a,int b)
{
    if(a<b)
        return(a);
    else
        return(b);
}

/*
 * min
 */
float fmin(float a,float b)
{
    if(a<b)
        return(a);
    else
        return(b);
}

/*
 * convert cm to feet
 */
float cmToFeet(float cm)
{
    return cm/30.48;
}

/*
 * convert feet to cm
 */
float feetToCm(float cm)
{
    return cm*30.48;
}

/*
 * convert radian to degree
 */
float radToDeg(float rad)
{
    return rad*57.2957795;
}

/*
 * convert degree to radian
 */
float degToRad(float deg)
{
    return deg/57.2957795;
}

/*
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

/*
 * Count the number of digits in an integer
 * (negative numbers add 1 for - sign)
 */
int digits(int signed_val)
{
    int digs, val;
    val = abs(signed_val);

    if(val < 100)
    {
        if(val < 10)
            digs = 1;
        else digs = 2;
    }
    else if(val < 10000)
    {
        if(val < 1000)
            digs = 3;
        else digs = 4;
    }
    else digs = 5;

    if(signed_val >= 0)
        return digs;
    else
        return (digs + 1);
}
