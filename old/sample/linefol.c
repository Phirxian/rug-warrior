/* Example of following a line on the floor.
   Two reflective light sensors (Lsen, Rsen) point down
   to see which side the line is on */

persistent int Rmot, Lmot, Rsen, Lsen, DIR, WHERE, Rlim, Llim;

void Input(){
  int low_l, high_l, low_r, high_r;

  while(!chosen_button()){
    printf("Both on white   L %d   R %d\n", low_l = analog(Lsen), 
	   low_r = analog(Rsen));
    sleep(.2);}
  sleep(1.);
                               /* I'm not using a black reading because */
                               /* this works fine.                      */
  printf("New Values:       L %d   R %d\n", Llim = low_l + 20,
	 Rlim = low_r + 20);
  sleep(1.);
  wait_button(CYCLE_B);
}

void Line_Follow(){
  while(!digital(7)) {
    
    while( analog(Rsen) < Rlim && analog(Lsen) < Llim 
	  && !digital(7) ) {
      motor(Rmot,DIR*80);
      motor(Lmot,DIR*80);}

    while( analog(Lsen) > Llim  && !digital(7) ){
      motor(Lmot,-(DIR)*15);
      motor(Rmot,DIR*40);   }
    

    while( analog(Rsen) > Rlim && !digital(7) ){
      motor(Rmot,-(DIR)*15);
      motor(Lmot,DIR*40);   }
  }
}


void main(){
  while(1){
    while(!chosen_button()){
     printf("Status: DIR = %d Sensor = %d\n",          /* DIR 0 => foward   */ 
	    dip_switch(2), dip_switch(3));             /* DIR 1 => backward */
    sleep(.2);}                              /* Sensor 0 -> sensor in fornt */
    if(dip_switch(2) == 1)                   /* Sensor 1 -> sensor in back  */
      DIR = -1;
    else 
      DIR = 1;
    beep();beep();
    printf("Direction %d",DIR);
    wait_button(CYCLE_B);

    if( !dip_switch(3) ) {                 /* Sensor placement determine left & */
      Rmot = 3;                            /* right in relation to movement.    */
      Lmot = 0;
      Rsen = 27;
      Lsen = 23;}
    else {
      Rmot = 0;
      Lmot = 3;
      Rsen = 23;
      Lsen = 27;}

    if(dip_switch(1))                      /* Calibrate?? */
      Input();

    printf("GO!!!!\n");
    Line_Follow();   
    ao();
  }
}    
      
      

