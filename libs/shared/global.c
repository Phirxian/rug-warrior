#define R_MOTOR 1
#define L_MOTOR 2
#define C_MOTOR (R_MOTOR | L_MOTOR)

int _motor_initial_speed_ = 0;
int _motor_right_speed_ = 0;
int _motor_left_speed_ = 0;
int _motor_distance_ = 0;

int _running_process_running_ = 0;
int _move_behind_process_running_ = 0;

float _move_behind_detected_distance_ = 0.;

int _detect_distance_process_running_ = 0;
float _process_yield_time_ = 0.075;
