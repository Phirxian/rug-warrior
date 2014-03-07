void main()
{
    init_motors();
    init_sonar();
    sonar_init_servo();
    printf("Runs until a wall is found 85cm\n");
    start_process(encoder_aux());
    move_behind(85.0, 1.0, 50);
}
