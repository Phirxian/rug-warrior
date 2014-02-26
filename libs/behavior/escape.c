#define RIGHT 		0b001
#define LEFT  		0b010
#define FRONT  		0b011
#define BACK  		0b100
#define BACK_RIGHT  0b101 /* USED ? */
#define BACK_LEFT   0b110 /* USED ? */

/** TEST TODO : Faire deux prog séparés en parallèle ? */


/**
 *
 */
int bumper_detection() {

	int bmp;

	/*
	 * Duration of the detection (until the max duration detection is reached)
	 * to check which bumper(s) is/are detected ?
	 */
	long time;
	long MAX_LAPS_DETECTION = 50L;
	
	bmp = bumper();

	time = mseconds();

	/* No Bumper is touched */
	if (bmp == 0) {
		return 0;
	}

	if (bmp == LEFT || bmp == RIGHT) {
		while ((mseconds() - time  < MAX_LAPS_DETECTION) && bmp != FRONT) {
			bmp = max(bmp, bumper());
		}
	}

	return bmp;
}

/**
 * Move forward, and when the skirt is pushed,
 * escape from an obstacle (move back then rotate)
 * Last update: 17:12 GMT+1, Feb. 26, 2014
 */
void main() {

	/* Use of int bumper() */
	int bmp;

	init_motors();

	start_process(encoder_aux());

	_motor_initial_speed_ = 80;

	drive(100,0);
	while(1) {

		/*
		 * Depending on the pushed bumper(s),
		 * make it rotate in the opposite way,
		 * and move forward again
		 */
		bmp = bumper_detection();

		if (bmp != 0) { /* A Bumper is touched */

			/* Stop the robot */
			stop();

			/* Drive back a bit */
			drive(-80,0);
			sleep(0.5);
			stop();

			 /* Stop a motor */
			if (bmp == LEFT) {
				/* Spin Left at 30° or 60° OR use IR */
				printf("RIGHT\n");
				rotate(R_MOTOR, 60);
			} else if (bmp == RIGHT) {
				/* Spin Right at 30° or 60°  OR use IR */
				printf("LEFT\n");
				rotate(L_MOTOR, 60);
			} else if (bmp == FRONT) {
				/* Spin Right at 90° */
				printf("FRONT\n");
				rotate(L_MOTOR, 90);
			} else if (bmp == BACK) {
				/* Go Forward */
			} else if (bmp == BACK_RIGHT) {
				printf("TODO");
				/* Spin Left at 30° */
			} else if (bmp == BACK_LEFT) {
				printf("TODO");
				/* Spin Right at 30° */
			}

			sleep(0.75); /* Take a moment after the correction */
			drive(100,0); /* After the correction go foward with full speed*/
		}
	}
}
