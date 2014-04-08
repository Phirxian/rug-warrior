/**
 * always print all rug-warrior caractere
 */
void main()
{
    int j;
    int chr = 32;
    printf("\nLCD Test\n");
    sleep(1.5);

    while(1)
    {
        for(j = 1; j<= 31; j++)
        {
            printf("%c",chr);
            chr++;
        }

        sleep(1.5);
        printf(" \n");
    }
}
