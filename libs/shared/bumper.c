#define BMP_RIGHT  		0b001
#define BMP_LEFT		0b010
#define BMP_FRONT  		0b011
#define BMP_BACK  		0b100

/**
 * Satisfy the weakness, than bumper() (lib_rwp.c)
 * The main change is to increase the accurency of the detection of the front bumper :
 *      In the case that the front is the left & right, we insert an active temporisation 
 *      To see if the front is touched (increased the angle of the touched), contrary bumper()
 * CAUTION : LAP OF THE DETECTION NOT BE HIGH -> OVERPASS LEFT & RIGHT, LOW -> use bumper()
 * @return the bumper touched 
 *         else 0 if no bumper are touched
 */
int bumper_detection() {

	int bmp;

	/*
	 * Duration of the detection (until the max duration detection is reached)
	 * to check which bumper(s) is/are detected ?
	 */
	long time;
	long MAX_LAPS_DETECTION = 50L;
	
	bmp = bumper(); /* First detection */

	/* Save the actual time, to make a correction with a temporisation */
	time = mseconds();
	
	/* When a bumper in the front is touched, I test with a te*/
    if (bmp == BMP_LEFT) {
	 	while (bumper() != BMP_RIGHT && (mseconds() - time  < MAX_LAPS_DETECTION));
	} else if (bmp == BMP_RIGHT) {
        while (bumper() != BMP_LEFT && (mseconds() - time  < MAX_LAPS_DETECTION));
    }
    
	return bmp; /* No bumper is touched or direct front OR back no correction*/
}
