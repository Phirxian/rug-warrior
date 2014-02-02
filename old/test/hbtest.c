/*

	test program for Handy Board
	version 1.0 -- 26 nov 95
	Fred G. Martin (fredm@media.mit.edu)
	
*/

void
testmotors(void)
{
  while (1) {
    int i;
    for (i= 0; i< 4; i++) {
      fd(i); msleep(250L);
      bk(i); msleep(250L);
      off(i);
    }
  }
}

void
testdigitals(void)
{
  printf("Press START ...\n");
  while (peek(0x7fff)&0x80);
  beep();
  
  printf("Press STOP ...\n");
  while (peek(0x7fff)&0x40);
  beep();
  
  while (1) {
    int i;
    printf("Digital inputs: ");
    for (i=15; i>6; i--)
      if (digital(i)) printf("1");
      else printf("0");
    printf("\n");
    msleep(100L);
  }
}

void
testanalogs(void)
{
    printf("Press START to  test knob...\n");
    start_press();
    
    while (1) {
        printf("Turn knob; STOP to end -> %d\n", knob());
        msleep(100L);
        if (stop_button()) {
            beep();
            break;
        }
    }
    
    printf("Press START to  test analog ins\n");
    start_press();
        
    while (1) {
        int i;
        for (i= 6; i >=0; i--)
          printf("%d ", analog(i));
        printf("\n");
        msleep(100L);
        if (stop_button()) {
            beep();
            break;
        }
    }
    
    printf("\n");
    
}

  
