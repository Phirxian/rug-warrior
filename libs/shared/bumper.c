/**
 * Compensate the fact that there's a difference between instantaneous detection and the actual test
 * The main change is to increase the accuracy of the detection of the front bumper:
 * We insert an active temporisation when bumper() returns a front detection (left && right);
 * See if the front is touched (will probably need to increase the angle of the hit), to contrary bumper()
 * CAUTION: LAP OF THE DETECTION IS NOT LONG -> OVERRIDE LEFT & RIGHT, LOW -> use bumper()
 * @return the touched bumper
 *         else 0 if no bumper(s) is/are touched
 * @version 270214-1541
 */
int bumper_detection()
{
    int bmp;
    /*
     * Duration of the detection (until the max duration is reached),
     * to check which bumper(s) is/are detected
     */
    long time;
    long MAX_LAPS_DETECTION = 50L;
    bmp = bumper(); /* First detection */
    /* Save the current time, to make a correction with a temporisation */
    time = mseconds();

    /* When the front bumper is touched, test the other bumper with a temporisation */
    if(bmp == BMP_LEFT)
    {
        while((bmp = bumper()) != BMP_RIGHT && (mseconds() - time  < MAX_LAPS_DETECTION));  /* Proactive Temposisation */

        /* Is the bumper right is touched during the LAPS? */
        if(bmp == BMP_RIGHT)
        {
            bmp = BMP_FRONT;
        }
    }
    else if(bmp == BMP_RIGHT)
    {
        while((bmp = bumper()) != BMP_LEFT && (mseconds() - time  < MAX_LAPS_DETECTION));  /* Proactive Temposisation */

        /* Is the bumper left is touched during the LAPS? */
        if(bmp == BMP_LEFT)
        {
            bmp = BMP_FRONT;
        }
    }

    return bmp; /* No bumper is touched OR direct front OR back: no correction*/
}
