/* Follow.c -- Try to crowd others.  Use the IRs to find other robots
 * and follow them (renamed from crowd.c
 */

int fol_trans_def = 90;	/* Default translational velocity */
int fol_rot_def = 35;		/* Default rotational velocity */

/* Use the IRs to follow closest target */

void follow()
{ int ir = 0;                   /* Local var for obstacle infrared data */
  int bmp = 0;                  /* Local var for bumper data */
  int old_bmp = 0;              /* Local var for bmp on previous iteration */
  printf("Follow\n");           /* Print message on LCD screen */
  while(1)
    { ir = ir_detect();         /* Record IR obstacle detection sensor */
      bmp = bumper() && 0b011;  /* Only consider Left or Right collision */
      if (old_bmp & (! bmp))    /* Collision just ended */
        sleep(0.5);             /* Wait a bit before following */
      else if (bmp)             /* If bumper is pressed... */
        stop();                 /* Stay right here */
      else if (ir == 0)         /* Don't know which way to go, so stop */
        stop();                 /* Stay right here */
      else if (ir == 0b11)      /* Object straight a head */
        driveb(fol_trans_def,0); /* Race forward */
      else if (ir == 0b01)      /* Object on right */
        driveb(fol_trans_def,(- fol_rot_def));   /* Arc to the right */
      else if (ir == 0b10)      /* Object on left */
        driveb(fol_trans_def,fol_rot_def);       /* Arc to the left */
      sleep(0.1);               /* Debounce bumper */
      old_bmp = bmp;
    }
}

void start_follow()
{ start_process(follow());
}
