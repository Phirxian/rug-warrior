/* Track - Move a specified number of encoder clicks */

/* init_velocity must be called before calling track */

float stop_time = 0.035;   /* Approximately, one servo cycle */

/* Track - drive at selected velocity until CLICKS enocder clicks have
   occurred, then stop.  track works best with rot = 0, or trans = 0.
   Track works best with either trans_vel = 0, or rot_vel = 0.
 */

int track(int trans_vel, int rot_vel, int clicks)
{
    long time_out = mseconds() + 5000L;     /* Time out after 5 seconds */
    int l_tot_clks = 0;    /* Total clicks */
    int r_tot_clks = 0;
    int ave_clks = 0;
    get_left_clicks();     /* Reset clicks */
    get_right_clicks();    /* Reset clicks */
    driveb(trans_vel, rot_vel);   /* Turn motors on */

    while(1)
    {
        l_tot_clks = l_tot_clks + get_left_clicks();  /* Count up the clicks */
        r_tot_clks = r_tot_clks + get_right_clicks();
        ave_clks = min(l_tot_clks, r_tot_clks);       /* Trigger on ave_clicks */

        if((ave_clks >= clicks) || (mseconds() > time_out))
        {
            driveb((- trans_vel), (- rot_vel));   /* Try to kill motion */
            sleep(stop_time);
            stop();
            return (clicks - ave_clks);
        }
    }
}
