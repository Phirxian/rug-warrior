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
/** 
 * Go Foward and when skirt is touched (push)
 * And Escape an obstacle when skirt is touched (bumper)
 */
void main() {

	/* Utilisatation de int bumper() */
	int bumper;
	bumper = bumper2();
	
	init_motors();
	
	drive(100,0);
	while(1) {
		/* En fonction du bumper touché le faire tourner dans le sens opposé et le faire repartir */
		
		if (bumper != 0) { /* A Bumper is touched */
			 /* Stop a motor */
			if (bumper == RIGHT) {
				drive(0,0);
				/* Spin Left at 30° or 60° OR use IR */
				rotate(L_MOTOR, 60.0, 50);
			} else if (bumper == LEFT) {
				drive(0,0);
				/* Spin Right at 30° or 60°  OR use IR */
				rotate(R_MOTOR, 60.0, 50);
			} else if (bumper == FRONT) {
				drive(0,0);
				/* Spin Right at 90° */
				rotate(R_MOTOR, 90.0, 50);
			} else if (bumper == BACK) {
				
				/* Go Forward */
				
			} else if (bumper == BACK_RIGHT) {
				printf("TODO");
				/* Spin Left at 30° */
				
			} else if (bumper == BACK_LEFT) {
				printf("TODO");
				/* Spin Right at 30° */
				
			}
			drive(100,0);

			sleep(0.5); /* Take a moment after the correction */
	
		}

	}
}

