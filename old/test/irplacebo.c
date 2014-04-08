void main() {
  int color=1, mode=1;
  alloff();
  led_out0(0);
  led_out1(0);
  servo_off();
  ir_transmit_off();
  ir_receive_off();
  ir_transmit();
  return;
}

void ir_transmit() {
  int color=1;
  ir_transmit_on();
  ir_receive_off();
  while (1) {
    if (color) {
      /* DIP_SWITCH(1) = 1 */
      printf("Xmit CLEAR      100 Hz  +45deg\n");
      ir_xmit_period= 10000;          /* Transmit at 100 Hz */
      ir_freq_rcv_select= 125;        /* Recieve 125 Hz */
    }
    else {
      /* DIP_SWITCH(1) = 0 */
      printf("Xmit GREEN      125 Hz  -45deg\n");
      ir_xmit_period= 8000;           /* Transmit at 125 Hz */
      ir_freq_rcv_select= 100;        /* Recieve 100 Hz */
    }
    sleep(.1);
    while (! (choose_button() || (escape_button())));
    color=!color;
    while (choose_button() || (escape_button()));
  }
}

void ir_receive() {
  int color=1;
  ir_transmit_off();
  ir_receive_on();
  while (1) {
    if (color) {
      /* DIP_SWITCH(1) = 1 */
      printf("Recv GREEN      125 Hz  -45deg\n");
      ir_xmit_period= 10000;          /* Transmit at 100 Hz */
      ir_freq_rcv_select= 125;        /* Recieve 125 Hz */
    }
    else {
      /* DIP_SWITCH(1) = 0 */
      printf("Recv CLEAR      100 Hz  +45deg\n");
      ir_xmit_period= 8000;           /* Transmit at 125 Hz */
      ir_freq_rcv_select= 100;        /* Recieve 100 Hz */
    }
    sleep(.1);
    while (! (choose_button() || (escape_button()))){
      if (ir_counts(4)>3 || ir_counts(5)>3 || ir_counts(6)>3
	  || ir_counts(7)>3)
	{
	  beep();
	  led_out0(1);
	  sleep(.05);
	  led_out0(0);
	  led_out1(1);
	  sleep(.05);
	  led_out1(0);
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
    color=!color;
    while (choose_button() || (escape_button()));
  }
}








