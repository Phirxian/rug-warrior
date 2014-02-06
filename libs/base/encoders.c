/* Encoders.c -- encoder code for ic for 6.270 board rev 2.2.  */
/* Shaft encoders on digital ports 0 and 1 should be used first
   because they are more efficient. */
/* Shaft encoders on digital ports 2 and 3 are less efficient and
   should only be used after 0 and 1 */
/* In order to use encoders, plug a digital shaft encoder into digital
   port 0 -> 3, and call enable_encoder() with the port number.
   Now you can call read_encoder() with the port number to read the
   number of shaft counter ticks since the last reset, and reset_encoder()
   to reset that number.  The counter overflows after around 32000 ticks */

void enable_encoder(int i)
{
    if(i==0)
    {
        bit_set(0x1022,1);
        port0_shaft_count=0;
    }
    else if(i==1)
    {
        bit_set(0x1022,2);
        port1_shaft_count=0;
    }
    else
        enable_cheesy_encoders(i);
}

void disable_encoder(int i)
{
    if(i==0)
    {
        bit_clear(0x1022,1);
    }
    else if(i==1)
    {
        bit_clear(0x1022,2);
    }
    else
        disable_cheesy_encoders(i);
}

void reset_encoder(int i)
{
    if(i==0)
        port0_shaft_count=0;
    else if(i==1)
        port1_shaft_count=0;
    else if(i==2)
        port2_shaft_count=0;
    else if(i==3)
        port3_shaft_count=0;
}

int read_encoder(int i)
{
    if(i==0)
        return(port0_shaft_count);
    else if(i==1)
        return(port1_shaft_count);
    else if(i==2)
        return(port2_shaft_count);
    else if(i==3)
        return(port3_shaft_count);

    return(0);
}
