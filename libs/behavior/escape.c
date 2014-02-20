#define RIGHT 		0b001
#define LEFT  		0b010
#define FRONT  		0b011
#define BACK  		0b100
#define BACK_RIGHT  0b101 /* USED ? */
#define BACK_LIGHT  0b110 /* USED ? */

/** 
 * Go Foward and when skirt is touched (push)
 * And Escape an obstacle when skirt is touched (bumper)
 */
void main() {

	/* Utilisatation de int bumper() */
	int bumper;
	bumper = bumper();
	
	init_motors();
	
	drive(100,0);
	while(1) {
		/* En fonction du bumper touché le faire tourner dans le sens opposé et le faire repartir */
		
		if (bumper != 0) { /* A Bumper is touched */
			drive(0,0); /* Stop a motor */
			if (bumper == RIGHT) {
				/* Spin Left at 60° or 90° or 120° OR use IR*/
			} else if (bumper == LEFT) {
				/* Spin Right at 60° or 90° or 120° OR use IR */
			} else if (bumper == FRONT) {
				/* Spin at 180° */
			} else if (bumper == BACK) {
				/* Go Forward */
			} else if (bumper == BACK_RIGHT) {
				/* Spin Left at 30° */
			} else if (bumper == BACK_LEFT) {
				/* Spin Right at 30° */
			}
			drive(100,0);

			sleep(0.5); /* Take a moment after the correction */
	
		}

	}
}
