/**
 * Move forward, and when the skirt is pushed,
 * escape from an obstacle (move back then rotate)
 * Last update: 17:25, Feb. 27, 2014
 */
void main() {
	
	int bmp; /* Store, temporarily the bumper touced */
	int run_pid; /* Store the pid of the running process, stop the robot when is killed*/
	
	/* */
	init_motors(); 
	start_process(encoder_aux());
	
	_motor_initial_speed_ = 100;
	run_pid = start_process(running_forever());

	while(1) {

		/*
		 * Depending on the pushed bumper(s),
		 * make it rotate in the opposite way,
		 * and move forward again
		 */
		bmp = bumper_detection();
		
		/* LEFT OR RIGHT  bumper is touched */
		if (bmp != 0 && bmp != BMP_BACK) { 
		
			/* Stop the robot */
			kill_process(run_pid);
			stop();
			
			/* Drive back a bit */			
			drive(-80,0);
			sleep(0.50);
			
			/* Stop a motor */
			if (bmp == BMP_RIGHT) {
				/* Spin Left at 30° or 60° */
				printf("RIGHT\n");
				rotate(R_MOTOR, 30);
				
			} else if (bmp == BMP_LEFT) {
				/* Spin Right at 30° or 60°*/
				printf("LEFT\n");
				rotate(L_MOTOR, 30);
				
			} else { /* if (bmp == BMP_FRONT) { */
				/* Spin Right at 90° */
				printf("FRONT\n");
				rotate(L_MOTOR, 90);
			}
			
			sleep(0.50); /* Take a moment after to go foward(avoid overbalance frontly) */
			
			/* The robot can go foward after the dodge */
			_motor_initial_speed_ = 100;
			run_pid = start_process(running_forever());
		}
		
	}
}
