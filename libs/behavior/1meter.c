void main()
{
    printf("runing for\n1.2 meter");
    init_motors();
    drive(100,0);
    sleep(feetToMotor(feetToCm(120.), 100));
    stop();
}
