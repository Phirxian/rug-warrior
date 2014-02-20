#define RIGHT 		0b001
#define LEFT  		0b010
#define FRONT  		0b011
#define BACK  		0b100
#define BACK_RIGHT  0b101 /* USED ? */
#define BACK_LEFT   0b110 /* USED ? */

/* Return a 3-bit value representing which of the bumper switches are closed */
int bumper2()
{ int bmpr;
  bmpr = analog(3);			/* Switch closure: */
  if      (bmpr <  11) return 0b000;	/* none */
  else if (bmpr <  32) return 0b001;	/* A   */
  else if (bmpr <  53) return 0b010;	/*  B  */
  else if (bmpr <  74) return 0b011;	/* AB  */
  else if (bmpr <  96) return 0b100;	/*  C  */
  else if (bmpr < 117) return 0b101;	/* A C */
  else if (bmpr < 138) return 0b110;	/*  BC */
  else                 return 0b111;	/* ABC - (mechanically impossible) */
}

/** 
 * Go Foward and when skirt is touched (push)
 * And Escape an obstacle when skirt is touched (bumper)
 */
void main() {

	/* Utilisatation de int bumper() */
	int bumper;
	
	init_motors();
	
	drive(100,0);
	while(1) {
		/* En fonction du bumper touché le faire tourner dans le sens opposé et le faire repartir */
		bumper = bumper2();
		if (bumper != 0) { /* A Bumper is touched */
			
			/* Back the robot */
			stop();
			drive(-80,0);
			sleep(0.5);
			stop();
			
			
			 /* Stop a motor */
			if (bumper == RIGHT) {
				/* Spin Left at 30° or 60° OR use IR */
				printf("RIGHT\n");
				rotate(R_MOTOR, 60., 80);
			} else if (bumper == LEFT) {
				/* Spin Right at 30° or 60°  OR use IR */
				printf("LEFT\n");
				rotate(L_MOTOR, 60., 80);
			} else if (bumper == FRONT) {
				/* Spin Right at 90° */
				printf("FRONT\n");
				rotate(R_MOTOR, 90., 80);
			} else if (bumper == BACK) {
				/* Go Forward */
			} else if (bumper == BACK_RIGHT) {
				printf("TODO");
				/* Spin Left at 30° */
			} else if (bumper == BACK_LEFT) {
				printf("TODO");
				/* Spin Right at 30° */
			}
			

			sleep(0.75); /* Take a moment after the correction */
			drive(100,0); /* After the correction go foward with full speed*/
		}

	}
}
