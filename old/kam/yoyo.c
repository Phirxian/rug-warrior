/* Yoyo -- when the back bumper is pressed the robot goes out a measured distance.
   Then it comes back.
   Requires: track.c
 */

int yo_inches = 30;        /* Go out this many inches */

int yoyo(int yo_inches)
{ 
    int yo_clicks =  yo_inches * 2;   /* Approx relationship between clicks and inches */
    while(1)
        {
            if (bumper() == 0b100)	       /* Bumper was hit from the back */
                {
                    track(90,0,yo_inches);  /* Go out */
                    track(-90,0,yo_inches); /* Come back */
                    sleep(0.5);		/* Wait a moment in case bumper is vibraiting */
        }
    }
}
        
void yo_tune()
{ tone(1000.0 * (2. ^ 12./12.) ,0.15);
  tone(1000.0 * (2. ^  8./12.) ,0.15);
  tone(1000.0 * (2. ^  5./12.) ,0.15);
  tone(1000.0 * (2. ^  8./12.) ,0.15);
  tone(1000.0 * (2. ^ 12./12.) ,0.15); }

void start_yoyo() 
{   printf("Yo-Yo\n");
    yo_tune();
    init_velocity();             /* Needed by track */
    sleep(1.0);
    start_process(yoyo(yo_inches));
}
