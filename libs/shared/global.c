/**
 * Global variables used all along of ours programs
 */

/**
 * Motors used to run the robot
 * @see "motor.c"
 */
#define R_MOTOR 1
#define L_MOTOR 2
#define C_MOTOR (R_MOTOR | L_MOTOR)

/**
 * The position of the obstacle obtained by ir_detect()
 */
#define OBSTACLE_NONE  0
#define OBSTACLE_RIGHT 1
#define OBSTACLE_LEFT  2
#define OBSTACLE_FRONT 3

/**
 * The bumper touched obtained by bumper_detection()
 */
#define BMP_RIGHT       0b001
#define BMP_LEFT        0b010
#define BMP_FRONT       0b011
#define BMP_BACK        0b100

/**
 * Where goes the robot?
 */
#define GO_LIGHT 0
#define GO_DARK  1

/**
 * The light environnement, the robot can go more efficiently to the light
 */
#define VDARK_ENVIRONNEMENT 1
#define DARK_ENVIRONNEMENT 2
#define LIGHT_ENVIRONNEMENT 4
#define VLIGHT_ENVIRONNEMENT 8


int _right_enc_counts_ = 0;
int _left_enc_counts_ = 0;

/**
 * Motors variables used
 * @see "motor.c"
 */
int _motor_initial_speed_ = 0;  /* Pourcentage of the max speed */
int _motor_right_speed_ = 0; /* FORBIDDEN TO MODIFY THE VALUE */
int _motor_left_speed_ = 0; /* FORBIDDEN TO MODIFY THE VALUE */
int _motor_distance_ = 0; /* The distance traveled by the robot in CM*/

/**
 * Indicator of the running of the process
 * 1: running, 0: Not running or tell of killing process properly, -1: process killed properly
 */
int _running_process_running_ = 0; /* Associated with running() ,running_forever(), move_behind() process*/
int _move_behind_process_running_ = 0; /* Associated with move_behind(), detect_distance_sonar() process*/

/* The distance that it must be traveled to stop the process  */
float _move_behind_detected_distance_ = 0.0;
int _detect_distance_process_running_ = 0;

/* A temporisation used to save ressources w/o late the program*/
float _process_yield_time_ = 0.075;
float _check_enc_yield_time_ = 1.0;
/**
 * Fonctions that used photons cells
 */
int _go_light_detection_running_ = 0; /* Process of "go light detection" is running */
int _go_light_touch_running_ = 0; /* Process of "go light touch" is running */
int _go_light_running_ = 0; /* Process of "go light" is running */
int _go_light_stop_running_ = 0; /* Process of "go_light_stop is running */

/* Various light of environnement use light_environnement() to calculate it  */
int _light_environnement_ = LIGHT_ENVIRONNEMENT;

/* 0 => Go light; else => Go Dark*/
int _mode_light_ = 0;
