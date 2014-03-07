void main()
{
    init_motor();
    start_process(encoder_aux());
    printf("Runs until a wall is found 85cm\n");
    move_behind(100.0, 1.0, 50);
}
