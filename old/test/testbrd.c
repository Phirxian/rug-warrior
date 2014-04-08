/* testbrd.c
   A simple board tester.
   Also see diagnostic.c */

int c_hit=0,e_hit=0;

void button_nab()
{
  while(1)
    {
      if(escape_button())
	e_hit=1;
      if(choose_button())
	c_hit=1;
    }
}
void reset_wait()
{
  while(escape_button() || choose_button());
  c_hit=0;
  e_hit=0;
}
void main()
{
  int i;
  int nab_process;
  
  nab_process=start_process(button_nab());
  while(1){  /* Begin main loop */
    printf("Test Board      Press escape \n");
    while(!e_hit);
    reset_wait();
    
    printf("Test Board      Press choose \n");
    while(!c_hit);
    reset_wait();
    
    printf("Testing output...\n");
    sleep(.5);
    
    printf("Motor Test\n");
    while(!e_hit){
      for(i=1;i<4 && !e_hit;i++){
	bk(i-1);
	sleep(.15);
	bk(i);
	sleep(.15);
	off(i-1);
	if (i<2) {
	  fd(4);
	  fd(5);
	}
	else{
	  bk(4);
	  bk(5);
	}
      }
      for(i=3;i>0 && !e_hit;i--){
	fd(i);
	sleep(.15);
	fd(i-1);
	sleep(.15);
	off(i);
	if(i<2){
	  bk(4);
	  bk(5);
	}
	else{
	  fd(4);
	  fd(5);
	}
      }	
    }
    
    ao();
    reset_wait();
    
    printf("LED test\n");
    while(!(e_hit || c_hit))
      {
	led_out0(0);
	led_out1(1);
	sleep(.15);
	led_out0(1);
	led_out1(0);
	sleep(.15);
      }
    
    led_out0(0);
    led_out1(0);
    
    reset_wait();
    
    printf("Servo test\n");
    servo_on();
    while(!(e_hit || c_hit))
      {
	for(i=0;i<180 && !e_hit;i++)
	  servo_deg((float)i);
	for(i=180;i>0 && !e_hit;i--)
	  servo_deg((float)i);
      }
    servo_off();
    reset_wait();
    
    printf("IR out test\n");
    while(!(e_hit || c_hit))
      {
	if(i==0)
	  ir_transmit_off();
	else
	  ir_transmit_on();
	sleep(.15);
	i=!i;
      }
    ir_transmit_off();
    reset_wait();
    
    printf("Input test\n");
    sleep(.5);
    
    while(!(e_hit || c_hit))
      {
	printf("Digital 0-7     %d %d %d %d %d %d %d %d\n",digital(7),digital(6),digital(5),digital(4),digital(3),digital(2),digital(1),digital(0));
	sleep(.1);
      }
    reset_wait();
    
    while(!(e_hit || c_hit))
      {
	printf("Frob = %d\n",frob_knob());
	sleep(.1);
      }
    reset_wait();
    
    while(!(e_hit || c_hit))
      {
	printf("Dips = %b\n",dip_switches());
	sleep(.1);
      }
    reset_wait();
    
    for(i=12;i<28;i+=2)
      {
	while(!(c_hit || e_hit))
	  {
	    printf("Analog %d %d     %d %d \n",i,i+1,analog(i),analog(i+1));
	    sleep(.1);
	  }
	if(e_hit)
	  break;
	reset_wait();
      }	
    reset_wait();
  }
  kill_process(nab_process);
  
}	




