#define LIGHT 0
#define DARK  1

/**
 * The robot following/avoid a light infinitely and he escape obstacle
 * @global _mode_light_ if mode == LIGHT, the robot go to the light
 * if mode == DARK, the robot go to the dark
 */
void go_light() {
    
	_go_light_running_ = 1;
    while(_go_light_running_) {
    
        int diff;
        diff = light_diff();
		diff *=4; /* Lighthen environnement */
		
        /* Invert the mode */
        if (_mode_light_ == DARK) {
            diff= -diff;
        }
        
        print_diff(diff);
        driveb(_motor_initial_speed_, diff);
		
    }
	
	stop();
	_go_light_running_ = -1;
    
}


/**
 * Avoid objects and go to the light or dark
 * @global _mode_light_ if mode != LIGHT, the robot go to the light
 * if mode == DARK, the robot go to the dark
 */
void go_light_detection() {

	int ir;
	start_process(go_light());
	_go_light_detection_running_ = 1;
	
	while (_go_light_detection_running_) {
		/* Obstacle detected, I stop the run and I am escaping remotely */
		if ((ir = ir_detect()) != OBSTACLE_NONE) {
		
			_go_light_running_ = 0;
			while (_go_light_running_ != -1);
			
			escape_distant(ir);
			/* Run after the correction*/
			start_process(go_light());
		}
	}
	
	_go_light_running_ = 0;
	while (_go_light_running_ != -1);
	_go_light_detection_running_ = -1;
}

/**
 * Escape objects and go to the light or dark
 * @param mode if mode != LIGHT, the robot go to the light
 * if mode == DARK, the robot go to the dark
 */
void go_light_touch() {
	
	int bmp;
	start_process(go_light());
	_go_light_touch_running_ = 1;
	
	while (_go_light_touch_running_) {
		/* Obstacle touched, I stop the run and I am escaping */
		bmp = bumper_detection();
		if (bmp != 0 && bmp != BMP_BACK) {
			/* _motor_right_speed_ -= 40;
			 * _motor_left_speed_ -= 40;
			 * sleep(0.1);
			*/
			_go_light_running_ = 0;
			while (_go_light_running_ != -1);
			
			escape(bmp);
			/* Run after the correction*/
			start_process(go_light());
		}
	}
	
	_go_light_running_ = 0;
	while (_go_light_running_ != -1);
	_go_light_touch_running_ = -1;
}

void main() {
	init_motors();
	_motor_initial_speed_ = 50;
	_mode_light_ = LIGHT;
	go_light();
	
}
