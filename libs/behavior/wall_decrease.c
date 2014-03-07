void main()
{
    init_motor();
    init_sonar();
    sonar_init_servo();
    printf("Runs until a wall is found 85cm\n");
    move_behind(100.0, 1.0, 50);
}
