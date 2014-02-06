/* Shaft encoder example */
/* by Matt Domsch (luigi@mit.edu) */
int FL_MOTOR = 0;
int RL_MOTOR = 2;
int FR_MOTOR = 1 ;
int RR_MOTOR = 3;
int L_ENCODER = 1;
int R_ENCODER = 0;
int SHAFT_ENCODER_THRESHHOLD = 3;
int LEFT = 1;
int RIGHT = -1;

int SHAFT_ENCODER_SPEED_DECREMENT=100;
persistent int rightturnclicks;
persistent int turnaroundclicks;

void main(){
  initialize_routines();
  if (dip_switch(4)) calibrate();
  printf("Robot drive.    Press CHOOSE\n");
  while(wait_button(CYCLE_B)!=CHOOSE_B);
  countdown(3.);
  measured_drive(100,4000);
  stop2();
}

void measured_drive(int speed, int distance){
/* speed -100 <= 0 <= 100 */
/* distance in ticks on the encoder */

  int lenc,renc;
  if (speed>100) speed=100;
  if (speed<-100) {speed=-100; SHAFT_ENCODER_SPEED_DECREMENT*=-1;}
  reset_encoder(0);reset_encoder(1);
  while (read_encoder(L_ENCODER) < distance &&
	 read_encoder(R_ENCODER) < distance) {
    lenc=read_encoder(L_ENCODER);
    renc=read_encoder(R_ENCODER);
    if (lenc-renc>SHAFT_ENCODER_THRESHHOLD)
      {/* left side is running faster.  Slow it down. */
	printf("Left side ahead by %d\n",lenc-renc);
	mmotor(RL_MOTOR,speed-SHAFT_ENCODER_SPEED_DECREMENT);
	mmotor(RR_MOTOR,speed);
	mmotor(FL_MOTOR,speed-SHAFT_ENCODER_SPEED_DECREMENT);
	mmotor(FR_MOTOR,speed);
      }
    else if (renc-lenc>SHAFT_ENCODER_THRESHHOLD)
      {/* right side is running faster.  Slow it down. */
	printf("Right side ahead by %d\n",lenc-renc);
	mmotor(RL_MOTOR,speed);
	mmotor(RR_MOTOR,speed-SHAFT_ENCODER_SPEED_DECREMENT);
	mmotor(FL_MOTOR,speed);
	mmotor(FR_MOTOR,speed-SHAFT_ENCODER_SPEED_DECREMENT);
      }
    else {
      printf("L=%d R=%d\n",lenc,renc);
      mmotor(RL_MOTOR,speed);
      mmotor(RR_MOTOR,speed);
      mmotor(FL_MOTOR,speed);
      mmotor(FR_MOTOR,speed);
    }
  }
  off(FL_MOTOR);
  off(FR_MOTOR);
  off(RL_MOTOR);
  off(RR_MOTOR);
}

void initialize_routines(){
  enable_encoder(L_ENCODER);enable_encoder(R_ENCODER);
}

void shutdown_routines(){
  disable_encoder(L_ENCODER);disable_encoder(R_ENCODER);
}

void mmotor(int m, int speed){
  if (m >=0 && m <=5) {
    if (speed>100) speed=100;
    if (speed<-100) speed=-100;
    motor(m,speed);}
}
void countdown(float t){
  float v;
  v=seconds()+t;
  while (seconds()<v){
    printf("Countdown       %f\n",v-seconds());
    sleep(.1);}
}

int measured_turn(int direction, int speed, int distance){
/* speed -100 <= 0 <= 100 */
/* distance in ticks on the encoder */
/* direction: 1=LEFT  -1=RIGHT */
  int lenc,renc,lmotorspeed,rmotorspeed;
  if (direction==LEFT) {lmotorspeed=-1*speed;rmotorspeed=speed;}
  else if (direction==RIGHT) {lmotorspeed=speed;rmotorspeed=-1*speed;}
  else {printf("Illegal turn direction\n");
	return(-1);}


  stop2();
  reset_encoder(L_ENCODER);reset_encoder(R_ENCODER);
  while (read_encoder(L_ENCODER) < distance &&
	 read_encoder(R_ENCODER) < distance) {
    lenc=read_encoder(L_ENCODER);
    renc=read_encoder(R_ENCODER);
    printf("L=%d R=%d\n",lenc,renc);
    if (lenc-renc>SHAFT_ENCODER_THRESHHOLD)
      {/* left side is running faster.  Slow it down. */
	mmotor(RL_MOTOR,lmotorspeed-SHAFT_ENCODER_SPEED_DECREMENT);
	mmotor(RR_MOTOR,rmotorspeed);
	mmotor(FL_MOTOR,lmotorspeed-SHAFT_ENCODER_SPEED_DECREMENT);
	mmotor(FR_MOTOR,rmotorspeed);
      }
    else if (renc-lenc>SHAFT_ENCODER_THRESHHOLD)
      {/* right side is running faster.  Slow it down. */
	mmotor(RL_MOTOR,lmotorspeed);
	mmotor(RR_MOTOR,rmotorspeed-SHAFT_ENCODER_SPEED_DECREMENT);
	mmotor(FL_MOTOR,lmotorspeed);
	mmotor(FR_MOTOR,rmotorspeed-SHAFT_ENCODER_SPEED_DECREMENT);
      }
    else {
      mmotor(RL_MOTOR,lmotorspeed);
      mmotor(RR_MOTOR,rmotorspeed);
      mmotor(FL_MOTOR,lmotorspeed);
      mmotor(FR_MOTOR,rmotorspeed);
    }
  }
  off(FL_MOTOR);
  off(FR_MOTOR);
  off(RL_MOTOR);
  off(RR_MOTOR);
}

void stop2(){
  int lenc,renc;
  off(FL_MOTOR);
  off(RL_MOTOR);
  off(FR_MOTOR);
  off(RR_MOTOR);
  reset_encoder(L_ENCODER);
  reset_encoder(R_ENCODER);
  sleep(.1);
  while (read_encoder(L_ENCODER) || read_encoder(R_ENCODER)){
    reset_encoder(L_ENCODER);
    reset_encoder(R_ENCODER);
    sleep(.05);
  }
  printf("Robot stopped.\n");
}

int adjust_turn(){
  int turn;
  int frob;
  printf("adjust_turn     Press CHOOSE\n");
  while(wait_button(CYCLE_B)!=CHOOSE_B);
  while (!escape_button()){
    while(!choose_button() && !escape_button()){
      frob=frob_knob()*100/255;
      printf("Press choose/ESC%d\n",frob);
      sleep(.1);
    }
    if (choose_button()) {
      while(choose_button());
      countdown(1.);
      measured_turn(LEFT,100,frob);
    }
  }
  while(escape_button());
  return(frob);
}

void calibrate(){
  rightturnclicks=adjust_turn();
}
