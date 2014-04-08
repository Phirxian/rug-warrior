
void main()
{
    light_environnement();
    init_motors();
    _motor_initial_speed_ = 50;
    _mode_light_ = GO_LIGHT;
    go_light_touch();
}
