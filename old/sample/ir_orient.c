/* orient.c         Mon Jan 10 13:21:00 EST 1994
   by Matt Domsch

   This code will orient a robot to face forward.  It receives the
   100Hz IR signal from the opponent robot, expecting that it won't
   move too much before we get oriented.  This may be unrealistic
   during the contest, but it's a good starting point.
*/


int FRONT_IR = 6;
int LEFT_IR = 7;
int RIGHT_IR = 5;
int BACK_IR = 4;
int LEFT_MOTOR = 0;
int RIGHT_MOTOR = 2;
int LEFT_TURN = 1;
int RIGHT_TURN = 0;

void main(){
  float t;
  int speed=100;
  int current_direction;
  printf("Running orient  Press ESC\n");
  while(!escape_button());
  while(escape_button());
  t=seconds()+3.0;
  while (seconds()<t){
    printf("Countdown       %f\n",t-seconds());
    sleep(.1);
  }
  ir_receive_on();
  set_ir_receive_frequency(100);
  while (ir_counts(FRONT_IR)<5) {
    printf("F=%d B=%d L=%d R=%d\n",ir_counts(FRONT_IR),
	   ir_counts(BACK_IR),
	   ir_counts(LEFT_IR),
	   ir_counts(RIGHT_IR));
    if (ir_counts(LEFT_IR) >5) /* Transmitter to the left.  Turn left. */
      {turn(LEFT_TURN,speed); current_direction=LEFT_TURN;}
    if (ir_counts(RIGHT_IR) >5) /* Transmitter to the right.  Turn right. */
      {turn(RIGHT_TURN,speed); current_direction=RIGHT_TURN;}
    if (ir_counts(BACK_IR)>5) /* Transmitter behind us.  Turn right. */
      {turn(RIGHT_TURN,speed); current_direction=RIGHT_TURN;}
    if (!ir_counts(LEFT_IR)&&!ir_counts(RIGHT_IR)&&!ir_counts(BACK_IR))
      /* We're lost.  Keep turning */
      turn(current_direction,speed);
    sleep(.1);
  }
  /* Now we've made it this far.  Aim tighter */
  beep();
  while (ir_counts(LEFT_IR)>5 || ir_counts(RIGHT_IR)>5){
    if (ir_counts(LEFT_IR) >5) /* Transmitter to the left.  Turn left. */
      {motor(LEFT_MOTOR,-speed);motor(RIGHT_MOTOR,speed); speed-=10;}
    if (ir_counts(RIGHT_IR) >5) /* Transmitter to the right.  Turn right. */
      {motor(LEFT_MOTOR,speed);motor(RIGHT_MOTOR,-speed); speed-=10;}
    if (speed<25) speed=25;
    
  }
  ao();
  printf("All done.\n");
}

void turn(int direction,int speed){
  if (direction) /* Turn left */
    {motor(LEFT_MOTOR,-speed);motor(RIGHT_MOTOR,speed);}
  else /* Turn right */
    {motor(LEFT_MOTOR,speed);motor(RIGHT_MOTOR,-speed);}
}    
