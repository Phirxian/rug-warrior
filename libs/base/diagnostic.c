int view_average_port(int port,int samples)
{
    int i,accum;
    printf("Press button to average\n");
    sleep(.7);

    while(chosen_button()==NEITHER_B)
    {
        printf("Port %d=%d\n",port,analog(port));
        sleep(.1);
    }

    if(chosen_button()==ESCAPE_B)
        return(-1);

    for(i=0; i<samples; i++)
        accum+=analog(port);

    samples=(accum/samples);
    printf("Port %d average=%d\n",port,samples);
    return(samples);
}

void view_inputs()
{
    char a[6][14]= {"Analog Port ","Digital Port ","Frob knob",
                    "Dip Switches","Motor Force","Quit"
                   };
    int sel,port,index;

    while(1)
    {
        if((index=select_string(a,6))==-1)
            return;

        if(index>-1 && index<2)
            port=select_int_value(a[index],0,27);

        if(index==0)  /* analog */
        {
            while(chosen_button()!=ESCAPE_B)
            {
                port=select_int_value(a[index],0,27);

                while(chosen_button()==NEITHER_B)
                {
                    printf("Port %d=%d\n",port,analog(port));
                    sleep(.1);
                }
            }

            wait_button(UP_B);
        }
        else if(index==1) /* digital */
            while(chosen_button()==NEITHER_B)
            {
                printf("Port %d=%d\n",port,digital(port));
                sleep(.1);
            }
        else if(index==2) /* frob knob */
        {
            if(select_int_value("Frob knob ",0,255)==-1)
                return;
        }
        else if(index==3) /* dip switches */
        {
            while(chosen_button()==NEITHER_B)
            {
                printf("Dips=%b\n",dip_switches());
                sleep(.1);
            }

            if(chosen_button()==ESCAPE_B)
                return;
        }
        else if(index==4)  /* motor force */
        {
            if((port=select_int_value("Motor ",0,3))==-1)
                return;

            motor(port,100);

            while(chosen_button()==NEITHER_B)
            {
                printf("Force=%d\n",motor_force(port));
                sleep(.1);
            }

            motor(port,0);

            if(chosen_button()==ESCAPE_B)
                return;
        }
        else  /* Quit */
            return;

        if(chosen_button()==ESCAPE_B)
            return;
    }
}

void frob_outputs()
{
    char a[6][10]= {"Motors","LEDs","Servo","IR on","IR off","Quit"};
    int sel,port,index;

    while(1)
    {
        if((index=select_string(a,5))==-1)
            return;

        if(index==0) /* motors */
            while(1)
            {
                if((port=select_int_value("Motor ",0,5))==-1)
                    break;

                while(chosen_button()==NEITHER_B)
                {
                    sel=frob_knob()-128;

                    if(sel>100)
                        sel=100;
                    else if(sel<-100)
                        sel=-100;

                    motor(port,sel);
                }

                off(port);

                if(chosen_button()==ESCAPE_B)
                    break;
            }
        else if(index==1)  /* LEDS */
        {
            char ch[5][10]= {"Led 0 off","Led 0 on","Led 1 off",
                             "Led 1 on","Quit"
                            };

            while(1)
            {
                if((sel=select_string(ch,5))==-1)
                    break;

                if(sel==0) led_out0(0);
                else if(sel==1) led_out0(1);
                else if(sel==2) led_out1(0);
                else if(sel==3) led_out1(1);
                else
                    return;
            }
        }
        else if(index==2) /* servo */
        {
            servo_on();

            while(chosen_button()==NEITHER_B)
            {
                sel=(frob_knob()*10)+700;
                servo(sel);
            }

            servo_off();

            if(chosen_button()==ESCAPE_B)
                break;
        }
        else if(index==3) /* IR on */
            ir_transmit_on();
        else if(index==4) /* IR off */
            ir_transmit_off();
        else   /* Quit */
            return;
    }
}

int control_choose()
{
    char a[4][16]= {"View Inputs","Frob outputs","Quit"};
    return(select_string(a,3));
}

void control_panel()
{
    int index;

    while(1)
    {
        if((index=control_choose())==-1)
            return;

        if(index==0) /* view inputs */
            view_inputs();
        else if(index==1) /* frob outputs */
            frob_outputs();
        else /* Quit */
            return;
    }
}
