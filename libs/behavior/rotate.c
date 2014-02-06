void main()
{
    init_motors();
/*
    printf("center %d: %d\n", 15., 100);
    rotate(R_MOTOR | L_MOTOR, 15.,  100);
    sleep(1.);
*/
    printf("\nright %d: %d\n", 90, 100);
    rotate(R_MOTOR, 90.,  100);
    sleep(1.);
/*
    printf("\nleft %d: %d\n", 90, 100);
    rotate(L_MOTOR, 90.,  100);
    sleep(1.);
    printf("\nright %d: %d\n", 15, -100);
    rotate(R_MOTOR, 15., -100);
    sleep(1.);
    printf("\nleft %d: %d\n", 15, -100);
    rotate(L_MOTOR, 15., -100);
    sleep(1.);
    printf("\ncenter %d: %d\n", 15, -100);
    rotate(R_MOTOR | L_MOTOR, 15., -100);
*/
}
