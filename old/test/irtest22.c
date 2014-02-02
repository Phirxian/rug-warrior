/*  This version for the rev2.2 board for receiving either IR signal */
/* By Matt Domsch Mon Jan 25 03:46:40 EST 1993 */
/* IR SENSOR in DIGITAL PORT 4 */

void main() {
  int color=1;
  alloff();
  led_out0(0);
  led_out1(0);
  servo_off();
  ir_transmit_off();
  num_ir_sensors=1;
  ir_receive_on();
  printf("IR RECEIVER in  DIGITAL PORT 4\n");
  while(! (choose_button() || escape_button()));
  while (choose_button() || escape_button());
  while (1) {
    if (color) {
      /* DIP_SWITCH(1) = 1 */
      printf("Recv WHITE/Grn  125 Hz  -45deg\n");
      ir_xmit_period= 10000;          /* Transmit at 100 Hz */
      ir_freq_rcv_select= 125;        /* Recieve 125 Hz */
    }
    else {
      /* DIP_SWITCH(1) = 0 */
      printf("Recv BLACK/Yel  100 Hz  +45deg\n");
      ir_xmit_period= 8000;           /* Transmit at 125 Hz */
      ir_freq_rcv_select= 100;        /* Recieve 100 Hz */
    }
    alloff();
    led_out0(0);
    led_out1(0);
    while (! (choose_button() || escape_button())){
      if (ir_counts(4) > 3)
	{
	  beep();
	  led_out1(1);
	  sleep(.05);
	  led_out1(0);
	  led_out0(1);
	  sleep(.05);
	  led_out0(0);
	  motor4_left(1);
	  sleep(.05);
	  motor4_left(0);
	  motor4_right(1);
	  sleep(.05);
	  motor4_right(0);
	  motor5_left(1);
	  sleep(.05);
	  motor5_left(0);
	  motor5_right(1);
	  sleep(.05);
	  motor5_right(0);
	}
      else {
	led_out0(0);
	led_out1(0);
	off(4);
	off(5);
      }
    }
    while (choose_button() || (escape_button()));
    color=!color;
  }
}

