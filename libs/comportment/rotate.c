/**
 * Do rotations with just left wheel in the order :
 * 45°/90°/180°/270°/360°/custom°
 */
void main()
{
    /* TODO à faire correspondre*/
    /* Rotation of 45° */
    /* Rotation of 90° */
    /* Rotation of 180° */
    /* Rotation of 270° */
    /* Rotation of 360° */
    /* Rotation of custom° */
    printf("center %d: %d\n", 15, 100);
    rotate(R_MOTOR | L_MOTOR, 15,  100);
    sleep(1.);
    printf("\nright %d: %d\n", 15, 100);
    rotate(R_MOTOR, 15,  100);
    sleep(1.);
    printf("\nleft %d: %d\n", 15, 100);
    rotate(L_MOTOR, 15,  100);
    sleep(1.);
    printf("\nright %d: %d\n", 15, -100);
    rotate(R_MOTOR, 15, -100);
    sleep(1.);
    printf("\nleft %d: %d\n", 15, -100);
    rotate(L_MOTOR, 15, -100);
    sleep(1.);
    printf("\ncenter %d: %d\n", 15, -100);
    rotate(R_MOTOR | L_MOTOR, 15, -100);
}
