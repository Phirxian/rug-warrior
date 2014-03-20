/*
* Rotation tests, using one or both motors
* with +/- a given angle
* @param degree Angle in degrees
*/
void rotate_test_all_for(int degree)
{
    printf("center %d\n", degree);
    rotate(C_MOTOR, degree);
    sleep(1.);

    printf("inv center %d\n", -degree);
    rotate(C_MOTOR, -degree);
    sleep(1.);

    printf("left %d\n", degree);
    rotate(L_MOTOR, degree);
    sleep(1.);

    printf("inv left %d\n", -degree);
    rotate(L_MOTOR, -degree);
    sleep(1.);

    printf("right %d\n", degree);
    rotate(R_MOTOR, degree);
    sleep(1.);

    printf("inv right %d\n", -degree);
    rotate(R_MOTOR, -degree);
    sleep(1.);
}

/**
 * Allow the user to select a program
 *** TODO Traduire en anglais
 *  a)  Avancer de 1m 20 avec une tolérance de 1 cm
 *  b)  Tourner autour d’une roue avec un angle de 45/90/180/270/360/a°
 *  c)  Tourner autour de son axe central avec un angle de 45/90/180/270/360/a°
 *  d)  Échapper aux obstacles lors du toucher
 *  e)  Arrêt du robot à 85 cm du mur
 *  f)  Arrêt du robot à 85 cm du mur puis cartographie sur 180° des objets autour du robot (affichage LCD des distances)
 *  g)  Départ tout droit à fond et ralentissement quand l’obstacle est à 1 m puis arrêt lorsque l’obstacle est à 50 cm
 *  h)  Éviter les obstacles avec la détection distante
 *  i)  Éviter les obstacles à distance et échapper aux obstacles au toucher
 *  j)  Suivre une lumière (lampe  torche)
 *  k)  Arrêt lorsque le robot est à 20 cm d’une source lumineuse
 *  l)  Suivre un mur à distance
 *  m)  Sortir d’un labyrinthe
 */
void main()
{
    sleep(0.75);
    test_number++;

    if((test_number > 13) | (test_number < 0))
        test_number = 0;

    while(test_number == 0)
    {
        printf("Rug Warrior Pro for Kam C.\n");
        sleep(1.5);
        printf("Press RESET to  select test\n");
        sleep(1.5);
        printf("VAYSSADE Jehan-Antoine\n");
        sleep(1.5);
        printf("MATEOS Christophe\n");
        sleep(1.5);
        printf("SERENE Guilhem\n");
        sleep(1.5);
    }

    /**
	 * Run over a given distance in centimeters
	 * The distance is the first number inside
	 * _motor_distance_ (default: 120)
	 *
	 * Last update: 16:16, Fri. 20 Mar.
	 * @version Final
	 */
	if(test_number == 1)
    {
        _motor_distance_ = (int)(120/1.178)-1;
		_motor_initial_speed_ = 100;

		printf("Running for\n1.20 meter");

		running();
    }
    /**
	 * Rotation tests
	 * Last update: 16:16, Fri. 20 Mar.
	 * @version Final
	 */
	else if(test_number == 2)
    {
        _motor_initial_speed_ = 100;
		printf("Rotate sample using %d speed\n", _motor_initial_speed_);
		sleep(1.);

		rotate_test_all_for(45);
		rotate_test_all_for(90);
		rotate_test_all_for(180);
		rotate_test_all_for(270);
		rotate_test_all_for(360);

		printf("Finished!\n");
    }
    /**
	 * Move forward, and when the skirt is pushed,
	 * escape from an obstacle (move back then rotate)
	 *
	 * Last update: 14:25, Mar. 4, 2014
	 * @version 1.1, utilisation des 2 moteurs pour accelerer la rotation par 2
	 */
	else if(test_number == 3)
    {
        int bmp; /* Temporarily store the bumpers value */

		_motor_initial_speed_ = 100;
		start_process(running_forever());

		while(1)
		{
			/*
			 * Depending on the pushed bumper(s),
			 * make it rotate in the opposite way,
			 * and move forward again
			 */
			bmp = bumper_detection();

			/* LEFT OR RIGHT  bumper is touched */
			if (bmp != 0 && bmp != BMP_BACK)
			{
				/* Stop the robot */
				_running_process_running_ = 0;
				while(_running_process_running_ != -1);
				escape(bmp);
				/* The robot can go forward after the dodge */
				_motor_initial_speed_ = 100;
				start_process(running_forever());
			}
		}
    }
    /**
	 * Run at a maximum speed until an obstacle is detected 85 cm away, then stop.
	 * 30 represents the minimum speed the robot will run at.
	 * These 2 values are passed to move_behind(), and can be edited at wish.
	 *
	 * The cartography will be run only after a second reset button hit
	 *
	 * Last update: 16:37, Fri. 20 Mar.
	 * @version Final
	 */
	else if(test_number == 4 || test_number == 5)
    {
        init_sonar();
		sonar_init_servo();

		printf("Run until the sonar finds a wall 85 cm\n");

		set_servo(0);
		sleep(0.175);

		ping();
		_move_behind_detected_distance_ = feetToCm(range());
		start_process(detect_distance_sonar());

		move_behind(85.0, 1.0, 30);

		_detect_distance_process_running_ = 0;
		while(_detect_distance_process_running_ != 1);

		/* Cartography */
		if( test_number == 5)
		{
			for(scan = -100; scan<100; ++scan)
			{
				set_servo(scan);
				ping();
				last = feetToCm(range());
				printf("Scan %f\n", last);
			}
		}

		set_servo(0);
		sonar_servo_off();
    }
	/**
	 * TODO
	 */
    else if(test_number == 6)
    {
        printf("Go, Slow & Stop at 50cm\n");
        sleep(1.0);
        go_stop(50, 100);
    }
    /**
	 * Move forward, and when an obstacle is detected,
	 * escape from it (move back then rotate)
	 *
	 * Last update: 16:45, Fri. 20 Mar.
	 * @version Final
	 */
	else if(test_number == 7)
    {
        int pid;
		int ir;  /* Temporarily store the ir_detect() value */

		printf("Run until a wall is found at 85 cm (IR)\n");

		_motor_initial_speed_ = 100;
		pid = start_process(running_forever());

		while(1)
		{
			/*
			 * Depending on the value returned by ir_detect(),
			 * make it rotate in the opposite way,
			 * and move forward again
			 */
			ir = ir_detect();

			/* ir has to be different from 0 */
			if(ir != OBSTACLE_NONE)
			{
				/* Stop the robot */
				kill_process(pid);

				escape(ir);
				/* The robot can go forward after the detection */
				_motor_initial_speed_ = 100;
				pid = start_process(running_forever());
			}
		}
    }
    /**
	 * Move forward, and when the skirt is pushed,
	 * escape from an obstacle (move back then rotate)
	 * @see escape.c
	 *
	 * When an obstacle is detected by ir,
	 * escape from it (move back then rotate)
	 * @see escape_distant.c
	 *
	 * Last update: 9:25, Mar. 19, 2014
	 * @version 1.0
	 */
	else if(test_number == 8)
    {
        int pid;
		int bmp; /* Store, temporarily the bumper values */
		int ir;  /* Store, temporarily the ir values */

		printf("Escape from contact and distance\n");

		_motor_initial_speed_ = 100;
		start_process(running_forever());

		while(1)
		{
			/*
			 * Depending on the pushed bumper(s),
			 * make it rotate in the opposite way,
			 * and move forward again
			 */
			bmp = bumper_detection();

			/*
			 * Depending on the value returned by ir_detect(),
			 * make it rotate in the opposite way,
			 * and move forward again
			 */
			ir = ir_detect();

			/* LEFT OR RIGHT  bumper is touched */
			if((bmp != 0 && bmp != BMP_BACK) || ir != OBSTACLE_NONE)
			{
				/* Stop the robot */
				_running_process_running_ = 0;
				while(_running_process_running_ != -1);

				if(ir) escape(ir);
				else   escape(bmp);

				/* The robot can go foward after the dodge */
				_motor_initial_speed_ = 100;
				start_process(running_forever());
			}
		}
    }
	/**
	 * TODO: This comment
	 */
    else if(test_number == 9)
    {
        printf("Follow the light\n");
        sleep(1.0);
        light_environnement();
		_motor_initial_speed_ = 50;
		_mode_light_ = GO_LIGHT;
		go_light();
    }
    else if(test_number == 10)
    {
        printf("Follow and stop at 20 cm from a light\n");
        sleep(1.0);
        _mode_light_ = GO_LIGHT;
		light_environnement();
		_motor_initial_speed_ = 50;
		go_light_stop(20);
    }
    else if(test_number == 11)
    {
        printf("Following Wall\n");
        sleep(1.0);
        follow_wall();
    }
    else if(test_number == 12)
    {
        printf("Out of Maze\n");
        sleep(1.0);
        labyrinth();
    }
}
