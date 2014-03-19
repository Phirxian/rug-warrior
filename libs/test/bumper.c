/**
 * always print bumper value
 */
void main()
{
    int bmpr;
    printf(" \n");

    while(1)
    {
        bmpr = bumper();
        printf("Bumper Test  ");
        printn(bmpr,3);

        if(bmpr == 0)
            printf("      none\n");
        else
        {
            if(bmpr & 0b010) printf("Left ");
            else printf("     ");

            if(bmpr & 0b100) printf("Back ");
            else printf("     ");

            if(bmpr & 0b001) printf("Right\n");
            else printf("     \n");
        }

        sleep(0.5);
    }
}
