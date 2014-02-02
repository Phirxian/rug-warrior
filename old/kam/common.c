/*  Functions and parameters common to all the Rug Warrior Pro
 *  example behaviors.
 */

/***********************************************************************/
/* Common parameters */

/* No two drive motors respond in exactly the same way to the same
   applied voltage.  Use the drive_bias term to correct for biases
   in your robot.  If your robot arcs to the right, make drive_bias
   positive, arcs to the left require a negative correction. */

int drive_bias = 0;	/* Open loop correction term for drive motors */

/***********************************************************************/
/* Common Utilities */

/* Absolute value function for integers */

int abs(int val)
{ 
  if (val < 0)
    return (- val);
  else
    return val;
}

/* Determine if VAL is approximately equal (within TOL) of CMP */

int ap_eq(int val, int cmp, int tol)
{
  if ( (abs(val - cmp)) <= tol)
    return 1;
  else
    return 0;
 }

/* Limit a VAL to the range MIN <= VAL <= MAX */

int clip (int val, int min, int max)
{ if (val < min)
    return min;
  else if (val > max)
    return max;
  else
    return val;
}

int min(int a, int b)	/* Find the minimum of two arguments */
{  if (a < b)
        return a;
    else
        return b;
}

int max(int x, int y)	/* Find the maximum of two arguments */
{ if (x > y)
   return x;
  else
   return y;
}


void driveb(int trans, int rot)		/* Correct for motor bias */
{ int rot_bias = (drive_bias * trans) / 100;
  motor(0,trans - (rot + rot_bias));
  motor(1,trans + (rot + rot_bias));
}
