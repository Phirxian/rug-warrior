/* Sample code ROVER */
/* Karsten Ulland (katrin@mit.edu) */

int L=0,B=1,R=2,LE=0,RE=1,RTP=27,LTP=26,RBP=24,LBP=23;
int RSPEED=0,LSPEED=0;
int SNIFF=0,RSNIFF=0,LSNIFF=0;
int EDGE=0,REDGE=0,LEDGE=0;
int PHOBIA=0,RPHOBIA=0,LPHOBIA=0;
int WANDER=0,RWANDER=60,LWANDER=60;
int FASTER=13,SLOWER=7,DENOM=10;
float LFACT=10.85,RFACT=10.85,COAST=15.;
void main()
{
  int sfrob,efrob,pfrob;
  enable_encoder(RE);
  enable_encoder(LE);
  start_process(drive_control());
  start_process(drive_monitor());
  printf("Woof!\n");
  sleep(.5);
  while(choose_button()==escape_button())
  {
    printf("Wander\n");
    sleep(.5);
    if (dip_switch(1)&&choose_button()==escape_button())
    {
      printf("Light Sniff\n");
      sleep(.5);
    }
    if (dip_switch(2)&&choose_button()==escape_button())
    {
      printf("Table Edge\n");
      sleep(.5);
    }        
    if (dip_switch(3)&&choose_button()==escape_button())
    {  
      printf("Photo Phobia\n");
      sleep(.5);
    }
    while(choose_button()==escape_button() && dip_switch(4))
    {
      printf("Test Encoders\n");
      sleep(.05);
    }
  }
  while(choose_button()==!escape_button());

  if (dip_switch(4))
    test_shaft();
  
  if (dip_switch(1))
    sfrob=get_frob("SNIFF Offset",8);

  if (dip_switch(2))
    efrob=get_frob("EDGE Trigger",8);

  if (dip_switch(3))
    pfrob=get_frob("PHOBIA Offset",8);

  if (dip_switch(1))    
    start_process(check_sniff(sfrob));
    
  if (dip_switch(2))
    start_process(check_edge(efrob));    
    
  if (dip_switch(3))
    start_process(check_phobia(pfrob));
		  
  WANDER=1;
}

void check_sniff(int frob)
{
  SNIFF=1;
  printf("SNIFF %d ",frob);
  while(1)
  {
    if (analog(RTP)>analog(LTP)+frob)
      {
	RSNIFF=100;
	LSNIFF=0;
      }
    if (analog(LTP)>analog(RTP)+frob)
      {
	LSNIFF=100;
	RSNIFF=0;
      }
    if ((analog(LTP)<analog(RTP)+frob) && (analog(RTP)<analog(LTP)+frob))
      {
	RSNIFF=100;
	LSNIFF=100;
      }
  }
}

void check_edge(int frob)
{
  int rlast=analog(RBP),llast=analog(LBP);
  printf("EDGE %d ",frob);
  while(1)
  {
    while (analog(RBP)<rlast-frob || analog(RBP)>rlast+frob)
      {
	LEDGE=-100;
	REDGE=0;
	EDGE=1;
	sleep(.3);
      }
    EDGE=0;
    while (analog(LBP)<llast-frob || analog(LBP)>llast+frob)    
      {
	LEDGE=0;
	REDGE=-100;
	EDGE=1;
	sleep(.2);
      }
    EDGE=0;
    sleep(.05);
  }
}

void wait_for_turn(int deg)
{
  int counts=deg*100/180;
  reset_encoder(LE);
  reset_encoder(RE);
  while(read_encoder(RE)<counts && read_encoder(LE)<counts);
}

void check_phobia(int frob)
{
  PHOBIA=1;
  printf("PHOBIA %d",frob);
  while(1)
  { 
    if (analog(RTP)>analog(LTP)+frob)
      {
	LPHOBIA=100;
	RPHOBIA=0;
      }
    if (analog(LTP)>analog(RTP)+frob)
      {
	RPHOBIA=100;
	LPHOBIA=0;
      }
    if ((analog(LTP)<analog(RTP)+frob) && (analog(RTP)<analog(LTP)+frob))
      {
	RPHOBIA=100;
	LPHOBIA=100;
      }
  }
}

void test_shaft()
{
  while(1)
    {
      float dist=(float)get_frob("INCHES",2);
      reset_encoder(RE);
      reset_encoder(LE);
      WANDER=1;
      while(read_encoder(RE)<(int)(dist*RFACT-COAST));
      WANDER=0;
    }
}


void test_encoders()
{
  int speed;
  while(1)
    {
	speed=(int)((float)frob_knob()/2.55);
	printf("L Shaft: %d      R Shaft: %d\n",read_encoder(LE)
		,read_encoder(RE));
	sleep(.05);
	RSPEED=speed;
	LSPEED=speed;
    }
  RSPEED=0;
  LSPEED=0;

}

int get_frob(char message[], int denom)
{
  int frob;
  while(choose_button()==escape_button())
    {
      frob=frob_knob()/denom;
      printf("%s=%d\n",message,frob); 
      sleep(.05);
    }
  while(choose_button()!=escape_button());
  return frob;
}

void drive_monitor()
{
  while(1)
  {
    if (EDGE)
    {
      RSPEED=REDGE;
      LSPEED=LEDGE;
    }

    else if (SNIFF)
    {
      RSPEED=RSNIFF;
      LSPEED=LSNIFF;
    }

    else if (PHOBIA)
    {
      RSPEED=RPHOBIA;
      LSPEED=LPHOBIA;
    }
    
    else if (WANDER)
    {
      RSPEED=RWANDER;
      LSPEED=LWANDER;
    }
    else
    {
      RSPEED=0;
      LSPEED=0;
    }
  }
}


void drive_control()
{
  int lcount,rcount,rstart,lstart,rspeed,lspeed;
  while(1)
    {
       if (RSPEED==LSPEED)
	 {
	   lstart=read_encoder(LE);
	   rstart=read_encoder(RE);
	   while(RSPEED==LSPEED)
	     {
	       rcount=read_encoder(RE)-rstart;
	       lcount=read_encoder(LE)-lstart;
	       if (lcount < rcount)
		 {
		   rspeed=RSPEED*SLOWER/DENOM;
		   lspeed=LSPEED*FASTER/DENOM;
		 }
	       if (lcount > rcount)
		 {
		   rspeed=RSPEED*FASTER/DENOM;
		   lspeed=LSPEED*SLOWER/DENOM;
		 }
	       if (lcount==rcount)
		 {
		   rspeed=RSPEED;
		   lspeed=LSPEED;
		 }
	       if (rspeed>100)
		 rspeed=100;
	       if (lspeed>100)
		 lspeed=100;
	       if (rspeed<-100)
		 rspeed=-100;
	       if (lspeed<-100)
		 lspeed=-100;
	       motor(R,rspeed);
	       motor(L,lspeed);
	     }
	 }
       motor(R,RSPEED);
       motor(L,LSPEED);
    }
}

void drive(int speed, float distance)
{
  reset_encoder(0);
  reset_encoder(1);
  RSPEED=speed;
  LSPEED=speed;
  while(how_far()< distance && !digital(7));
  RSPEED=0;
  LSPEED=0;
}

float how_far()
{
  float left,right;
  left=(float)read_encoder(LE)*LFACT;
  right=(float)read_encoder(RE)*RFACT;
  return (left+right)/2.;
}

void turn(int speed, float time)
{
	if (speed>0)
		LSPEED=speed;
	else
		RSPEED=speed;
	sleep(time);
	LSPEED=0;
	RSPEED=0;
}

void spin(int speed, float time)
{
	LSPEED=speed;
	RSPEED=-speed;
	sleep(time);
	RSPEED=0;
	LSPEED=0;
}

