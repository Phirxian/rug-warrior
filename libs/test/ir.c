/**
 * always print the ir direction (received)
 */
void main()
{
    int ir_val;

    while(1)
    {
        ir_val = ir_detect();

        if(ir_val == 0b00)
            printf("                ");   /* Clear */
        else if(ir_val == 0b11)
            printf(" <<<<<====>>>>> ");   /* Both */
        else if(ir_val == 0b10)
            printf(" <<<<<          ");   /* Left */
        else
            printf("          >>>>> ");   /* Right */

        printf("IR Test\n");
        sleep(0.35);
    }
}
