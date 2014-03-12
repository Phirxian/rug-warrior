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
    return (int)((float)(pc_left - pc_right) /2.55);
}
