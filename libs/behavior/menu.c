
/**
 * Allow the user to select a program 
 *** TODO Traduire en anglais
 *  a)	Avancer de 1m 20 avec une tolérance de 1 cm
 *  b)	Tourner autour d’une roue avec un angle de 45/90/180/270/360/a°
 *  c)	Tourner autour de son axe central avec un angle de 45/90/180/270/360/a° 
 *  d)	Échapper aux obstacles lors du toucher
 *  e)	Arrêt du robot à 85 cm du mur
 *  f)	Arrêt du robot à 85 cm du mur puis cartographie sur 180° des objets autour du robot (affichage LCD des distances)
 *  g)	Départ tout droit à fond et ralentissement quand l’obstacle est à 1 m puis arrêt lorsque l’obstacle est à 50 cm
 *  h)	Éviter les obstacles avec la détection distante
 *  i)	Éviter les obstacles à distance et échapper aux obstacles au toucher
 *  j)	Suivre une lumière (lampe  torche)
 *  k)	Arrêt lorsque le robot est à 20 cm d’une source lumineuse
 *  l)	Suivre un mur à distance
 *  m)	Sortir d’un labyrinthe
 */
void main()

{ sleep(0.75);

  test_number++;

  if ((test_number > 13) | (test_number < 0))

    test_number = 0;

  while (test_number == 0)

    { printf("Rug Warrior Pro for Kam C.\n");

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

	if (test_number == 1) {
		printf("1.2 meters\n"); \n sleep(1.0);
		ahead(120);
    
	} else if (test_number == 2) {
		printf("Rotation around a wheel\n"); 
		sleep(1.0);
		rotate_wheel(45);
		sleep(1.0);
		rotate_wheel(90);
		sleep(1.0);
		rotate_wheel(180);
		sleep(1.0);
		rotate_wheel(270);
		sleep(1.0);
		rotate_wheel(360);
		sleep(1.0);
    
	} else if (test_number == 3) {
		printf("Rotation around the axis\n");
		sleep(1.0);
		rotate_axis(30);
		sleep(1.0);
		rotate_axis(45);
		sleep(1.0);
		rotate_axis(90);
		sleep(1.0);
		rotate_axis(180);
		sleep(1.0);
		rotate_axis(270);
		sleep(1.0);
		rotate_axis(360);
		sleep(1.0);
    
  } else if (test_number == 4) {
    printf("Escape obstacles\n"); 
    sleep(1.0);
    escape();

  } else if (test_number == 5) {
    printf("Stop at 85 cm wall\n"); 
    sleep(1.0);
    stop_wall(85);
    
  } else if (test_number == 6) {
    printf("Stop at 85 cm wall\n"); 
    sleep(1.0);
    stop_wall(85);
    sleep(1.0);
    rotate(R_MOTOR, 180);
    sleep(1.0);
    cartography(50);

  } else if (test_number == 7) {
    printf("Go, Slow & Stop at 50cm\n");
    sleep(1.0);
    go_stop(50, 100);

  } else if (test_number == 8) {
    printf("Avoid Obstacles\n"); 
    sleep(1.0);
    avoid();

  } else if (test_number == 9) {
    printf("Avoid and escape Obstacles\n"); 
    sleep(1.0);
    avoid_escape();

  } else if (test_number == 9) {
    printf("Following Light\n"); 
    sleep(1.0);
    follow_light();
  
  } else if (test_number == 11) {
    printf("Search & Stop at 20cm of light\n"); 
    sleep(1.0);
    stop_light(20);
    
  } else if (test_number == 12) {
    printf("Following Wall\n"); 
    sleep(1.0);
    follow_wall();
  
  } else if (test_number == 13) {
    printf("Out of Maze\n"); 
    sleep(1.0);
    labyrinth();
  }
  
}
