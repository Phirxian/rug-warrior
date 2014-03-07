void main()
{
    init_motors();
    printf("Runs until a wall is found 85cm\n");
    start_process(encoder_aux());
    move_behind(85.0, 1.0, 50);
}
