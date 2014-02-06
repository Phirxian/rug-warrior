void main()
{
    int pos;
    pos = -100;

    init_sonar();
    sonar_init_servo();

    while(pos <= 100)
    {
        printf("%d - ", pos);
        set_servo(pos);

        ping();
        printf("%d\n", range());

        pos += 10;
        sleep(1.);
    }

    sonar_servo_off();
}
