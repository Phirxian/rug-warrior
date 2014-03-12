/**
 * Absolute value function for integers
 */
int abs(int val)
{
    if(val < 0)
        return (- val);
    else
        return val;
}

/**
 * Determine if VAL is approximately equal (within TOL) of CMP
 */
int ap_eq(int val, int cmp, int tol)
{
    return (abs(val - cmp)) <= tol;
}

/**
 * Limit a VAL to the range MIN <= VAL <= MAX
 */
int clip(int val, int min, int max)
{
    if(val < min)
        return min;
    else if(val > max)
        return max;
    else
        return val;
}

/**
 * Find the minimum of two arguments
 */
int min(int a, int b)
{
    if(a < b)
        return a;
    else
        return b;
}

/**
 * Find the maximum of two arguments
 */
int max(int x, int y)
{
    if(x > y)
        return x;
    else
        return y;
}
