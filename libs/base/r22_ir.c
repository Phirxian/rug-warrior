/* routines to control infrared transmission */

void ir_transmit_on()
{
    bit_set(0x1022, 0b01000000);    /* enable interrupt */
}

void ir_transmit_off()
{
    bit_clear(0x1022, 0b01000000);  /* disable interrupt */
    bit_clear(0x1000, 0b01000000);  /* bit clear == IR off */
    bit_set(0x1026, 0x80);      /* make TOC1 be output */
}

void set_ir_transmit_period(int period)
{
    ir_xmit_period= period;
}

/* Added by Matt Domsch Tue Jan 11 11:24:22 EST 1994 */
void set_ir_transmit_frequency(int f)
{
    ir_xmit_period= (1000/f)*1000;
}

/* Added by Matt Domsch Tue Jan 11 11:24:22 EST 1994 */
void set_ir_receive_period(int period)
{
    ir_freq_rcv_select=(int)((float)1000/(float)period*(float)1000);
}

void set_ir_receive_frequency(int f)
{
    ir_freq_rcv_select= f;
}

int ir_counts(int p)
{
    if(p == 4) return ir_counts_zero;

    if(p == 5) return ir_counts_one;

    if(p == 6) return ir_counts_two;

    if(p == 7) return ir_counts_three;

    return -1;
}

void ir_receive_off()
{
    num_ir_sensors= 0;
}

void ir_receive_on()
{
    num_ir_sensors= 4;
}
