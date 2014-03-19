/**
 * escape from an obstacle (move back then rotate)
 * can be used with bumper or ir
 * Last update: 19:25, Mar. 7, 2014
 * please no running/runiing_forever process when you use this fonction
 * @version 1.1, utilisation des 2 moteurs pour accelerer la rotation par 2
 */
void escape(int escape_bumper)
{
    /* Drive back a bit */
    drive(-80,0);
    sleep(0.50);

    /* Stop a motor */
    if(escape_bumper == BMP_RIGHT)
    {
        /* Spin Left at 30° or 60° */
        printf("RIGHT\n");
        rotate(C_MOTOR, -30);
    }
    else if(escape_bumper == BMP_LEFT)
    {
        /* Spin Right at 30° or 60°*/
        printf("LEFT\n");
        rotate(C_MOTOR, 30);
    }
    else
    {
        /* Spin Right at 90° */
        printf("FRONT\n");
        rotate(C_MOTOR, 90);
    }

    /* Take a moment after to go foward(avoid overbalance frontly) */
    sleep(0.50);
}
