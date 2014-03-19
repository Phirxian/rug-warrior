#define R_MOTOR 1
#define L_MOTOR 2
#define C_MOTOR (R_MOTOR | L_MOTOR)

#define OBSTACLE_NONE  0
#define OBSTACLE_RIGHT 1
#define OBSTACLE_LEFT  2
#define OBSTACLE_FRONT 3

#define BMP_RIGHT       0b001
#define BMP_LEFT        0b010
#define BMP_FRONT       0b011
#define BMP_BACK        0b100

/* The light environnement */
#define VDARK_ENVIRONNEMENT 1
#define DARK_ENVIRONNEMENT 2
#define LIGHT_ENVIRONNEMENT 4
#define VLIGHT_ENVIRONNEMENT 8 

int _motor_initial_speed_ = 0;
int _motor_right_speed_ = 0;
int _motor_left_speed_ = 0;
int _motor_distance_ = 0;

int _running_process_running_ = 0;
int _move_behind_process_running_ = 0;

float _move_behind_detected_distance_ = 0.;

int _detect_distance_process_running_ = 0;
float _process_yield_time_ = 0.075;

int _go_light_detection_running_ = 0; /* Process of "go light detection" is running */
int _go_light_touch_running_ = 0; /* Process of "go light touch" is running */
int _go_light_running_ = 0; /* Process of "go light" is running */
int _mode_light_ = 0; /* 0 => Go light; else => Go Dark*/
int _light_environnement_ = 4; /* Various light of environnement */
