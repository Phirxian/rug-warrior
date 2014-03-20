/**
 * Calculate the difference between photos cells
 * @return -100 (absolute light in left, with absolute dark in the right)
 *                to 100  (absolute light in right, with absolute dark in the left)
 */
int light_diff()
{
    int pc_left, pc_right;
    pc_left  = analog(photo_left); /* 0 to 255 */
    pc_right = analog(photo_right); /* 0 to 255 */
    return (int)((float)(pc_right - pc_left) /2.55);
}

/**
 * Ajust the global variable "_light_environnement_" with the environnement light
 */
void light_environnement()
{
    int lightest = min(analog(photo_left), analog(photo_right)); /* 0 to 255 */

    if(lightest < 32)
    {
        _light_environnement_ = VLIGHT_ENVIRONNEMENT;
    }
    else if(lightest < 64)
    {
        _light_environnement_ = LIGHT_ENVIRONNEMENT;
    }
    else if(lightest < 128)
    {
        _light_environnement_ = DARK_ENVIRONNEMENT;
    }
    else
    {
        _light_environnement_ = VDARK_ENVIRONNEMENT;
    }
}

/**
 * Display values of photo cells
 * once time
 */
void display_info()
{
    int pc_left, pc_right;
    pc_left  = analog(photo_left);
    pc_right = analog(photo_right);
    /* Diff thermometer points toward brighter light */
    print_diff(pc_left - pc_right);
    printf("Photo L");
    p_byte(pc_left);
    printf(" R");
    p_byte(pc_right);
    printf(" \n");
    sleep(0.25); /* Avoid the aliasing */
}
