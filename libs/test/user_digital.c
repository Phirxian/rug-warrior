/**
 * always print the reserved user-port value
 */
void main()
{
    int dig;

    while(1)
    {
        dig = peek(porta) & 0b00000110;
        printf("User     PA1 %d", digital(1));
        printf("  ");
        printf("Digital  PA2 %d", digital(2));
        printf(" \n");
        sleep(0.5);
    }
}
