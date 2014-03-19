/**
 * always print the encoders values
 */
void main()
{
    while(1)
    {
        printf("Encoders Test   ");
        printf("L");

        if(left_shaft()) printf("*");        /* Left SE is ON */
        else             printf("%c",0xA5);  /* Left SE is OFF */

        printn(_left_enc_counts_,5);         /* Report Left clicks */
        printf(" R");

        if(right_shaft()) printf("*");       /* Right SE is ON */
        else              printf("%c",0xA5); /* Right SE is OFF */

        printn(_right_enc_counts_,5);        /* Report Right clicks */
        printf("\n");
        sleep(0.3);
    }
}
