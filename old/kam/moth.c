/*  MOTH.C -- Seek (or avoid) a light source
 *  [ Requires abs() and clip() from common.c ]
 */

/* MOTH display function
 * Call with bright_p = 1 to have robot seek light
 * birght_p = -1 makes robot seek dark
 */

void moth_display(int bright_p, int left_photo, int right_photo, int turn)
{ 
  if (bright_p == 1)
    /*      0123456789ABCDEF */
    printf("MOTH  Seek light");
  else
    printf("MOTH  Seek dark ");
    /*    01345689ABCDEF */
  printf("L%d R%d  T%d\n", left_photo, right_photo, turn);
  }

int photo_gain = 1;	/* Control system parameter */
int rot_max = 100;	/* Max ans min allowed rotational speeds */
int moth_trans = 0;	/* Translational velocity */
int moth_rot = 0;	/* Rotational velocity */


/* Aim toward or away from the light */
/* light_p = +1 => point toward light, -1, point away */

void moth_point(int bright_p)
{ int left_photo, right_photo, diff, rot;
  while(1)
    { left_photo =  analog(photo_left);	/* Grab photo cell values */
      right_photo = analog(photo_right);
      diff = right_photo - left_photo;	/* Left/Right difference */
      moth_rot  = clip( (photo_gain * bright_p * diff), (- rot_max), rot_max);
      moth_display(bright_p, left_photo, right_photo, moth_rot);	/* Show choices */
      sleep(0.25);	/* Repeat period */
    }
}

int moth_trans_def = 80;	/* Default translational velocity */

/* Stop moving forward if robot bumps something */

void moth_bump()
{ while (1)
    {
      if (bumper() == 0)	/* Nothing bumped */
	moth_trans = moth_trans_def;	/* Maximum speed */
      else
	moth_trans = 0;		/* Otherwise, stop */
    }
}

/* Send behavior commands to the motors */

void moth_drive()
{ while(1)
    driveb(moth_trans, moth_rot);
}

void start_moth(int light_p)		/* 1 => follow light, -1 => dark */
{ start_process(moth_point(light_p));	/* Compute which direction to rotate */
  start_process(moth_bump());			/* Stop on collision */
  start_process(moth_drive());			/* Motor control */
}

