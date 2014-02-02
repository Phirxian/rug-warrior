
/**
 * TODO En-tete
 */
void main()
{
    const int distance = 120; /* Distance in centimeters */
    
    printf("Running for\n%.2d meter", distance/100);
    
    init_motors();
    drive(100,0);
    sleep(feetToMotor(feetToCm(distance.), 100));
    stop();
}
