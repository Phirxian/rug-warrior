
/**
 * TODO
 */
void main()
{
    const int distance = 120; /* The distance in centimeters*/

    printf("running for\n%d meter(s) : ", distance/100);
    init_motors();
    drive(100,0);
    sleep(feetToMotor(feetToCm(distance.), 100));
    stop();
    printf("%.2d has been run", distance/100);
}
