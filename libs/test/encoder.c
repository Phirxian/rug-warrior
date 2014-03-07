int _l_enc_counts = 0;
int _r_enc_counts = 0;

/* Local click counter */
void encoder_aux()
{
   /* Trigger on Hi -> Low transition */
    int l_old, r_old, l_new, r_new;

    while(1)
    {
        l_new = left_shaft();
        r_new = right_shaft();

        /* (old and not new) => 1 -> 0 transition */
        if(l_old & ~l_new)
            _l_enc_counts++;

        if(r_old & ~r_new)
            _r_enc_counts++;

        /* Update transition detector */
        l_old = l_new;
        r_old = r_new;
    }
}

void main()
{
    while(1)
    {
        printf("Encoders Test   ");
        printf("L");

        if(left_shaft()) printf("*");        /* Left SE is ON */
        else             printf("%c",0xA5);  /* Left SE is OFF */

        printn(_l_enc_counts,5);             /* Report Left clicks */
        printf(" R");

        if(right_shaft()) printf("*");       /* Right SE is ON */
        else              printf("%c",0xA5); /* Right SE is OFF */

        printn(_r_enc_counts,5);             /* Report Right clicks */
        printf("\n");
        sleep(0.3);
    }
}
