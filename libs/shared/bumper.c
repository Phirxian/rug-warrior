#define BMP_RIGHT  		0b001
#define BMP_LEFT		0b010
#define BMP_FRONT  		0b011
#define BMP_BACK  		0b100

/**
 * Tool-Class Bumpers-related
 * @version 270214-1541
 */

/**
 * Compensate the fact that there's a difference between instantaneous detection and the actual test
 * The main change is to increase the accuracy of the detection of the front bumper:
 * We insert an active temporisation when bumper() returns a front detection (left && right);
 * See if the front is touched (will probably need to increase the angle of the hit), to contrary bumper()
 * CAUTION: LAP OF THE DETECTION IS NOT LONG -> OVERRIDE LEFT & RIGHT, LOW -> use bumper()
 * @return the touched bumper
 *         else 0 if no bumper(s) is/are touched
 */
int bumper_detection() {

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

	/* When the front bumper is touched, test with a temporisation */
    if (bmp == BMP_LEFT) {
	 	while (bumper() != BMP_RIGHT && (mseconds() - time  < MAX_LAPS_DETECTION));
	} else if (bmp == BMP_RIGHT) {
        while (bumper() != BMP_LEFT && (mseconds() - time  < MAX_LAPS_DETECTION));
    }

	return bmp; /* No bumper is touched OR direct front OR back: no correction*/
}
