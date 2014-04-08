void p_byte(int val)
{
    if(val < 10)
        printf("  %d", val);
    else if(val < 100)
        printf(" %d", val);
    else
        printf("%d", val);
}

/**
 * therm_diff is for displaying the difference between two quantities
 */
void therm_diff(int val)
{
    if(val == 0)
        printf("       []       "); /* Zero */
    else if(val < -8)
        printf("%c<<<<<<<]       ",127);    /* Less than -8 */
    else if(val < 0)
    {
        repeat_char(32,8 + val);  /* 32 = space */
        repeat_char(60,abs(val)); /* 60 = "<" */
        printf("]       ");
    }
    else if(val > 8)
        printf("       [>>>>>>>%c",126);    /* More than +8 */
    else
    {
        printf("       [");
        repeat_char(62,val);
        repeat_char(32,8 - val);
    }
}

/**
 * always print the photo state (left, rigth)
 * and the difference value
 */
void main()
{
    int pc_left, pc_right;

    while(1)
    {
        pc_left  = analog(photo_left);
        pc_right = analog(photo_right);
        /* Diff thermometer points toward brighter light */
        therm_diff(pc_left - pc_right);
        printf("Photo L");
        p_byte(pc_left);
        printf(" R");
        p_byte(pc_right);
        printf(" \n");
        sleep(0.25);
    }
}
