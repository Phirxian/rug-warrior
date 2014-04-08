int BALLBEAM = 19;
int ARMBEAM = 20;
int BALLLIGHT = 0;
int ARMLIGHT = 1;

float SHOTDELAY=.3,SHOTTIME=.2,BREAKTIME=.05;
int THRESHOLD=10;

void main()
{
  while(1){
    wrap("Break Beam Shooter Paused");
    while(!choose_button());
    while(choose_button());
    wrap("Break Beam Shooter Armed");
    while(!escape_button()) {
      while(!(broken(ARMBEAM,ARMLIGHT))) 
	{motor(0,-20);sleep(.02);motor(0,0);sleep(.13);}
      if(broken(ARMBEAM,ARMLIGHT) && broken(BALLBEAM,BALLLIGHT)){
	sleep(SHOTDELAY);
	motor(0,100);
	sleep(SHOTTIME);
	motor(0,-100);
	sleep(BREAKTIME);
	motor(0,0);
      }
    }
  }
}

int broken(int port, int led_num)
{
  int onval,offval;
  if (led_num==0){
    offval=analog(port);
    led_out0(1);
    sleep(.05);
    onval=analog(port);
    led_out0(0);
  }
  else{
    offval=analog(port);
    led_out1(1);
    sleep(.05);
    onval=analog(port);
    led_out1(0);
  }
  if ((offval-onval)>THRESHOLD)
    return 0;
  else
    return 1;
}

