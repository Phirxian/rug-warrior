/**
 * make sure encoder process is running
 * increase _left_enc_counts_ and _rigth_enc_counts_
 */
int _process_encoder_ = start_process(encoder_aux());

/**
 * make sure check_encoder process is running
 * check _left_enc_counts_ and _rigth_enc_counts_
 * is change between two interval (_check_enc_yield_time_)
 * and close all thread (implemented) that use encoders
 */
/* int _process_encoder_check_ = start_process(check_encoder()); */

/**
 * notify the thread to be stopped
 * use exit variable like
 * _running_process_running_, _move_behind_process_running_, ... etc
 */
#define stop_process(x) x = 0;

/**
 * wait for a thread
 * use exit variable like
 * _running_process_running_, _move_behind_process_running_, ... etc
 */
#define wait_process(x) while(x != -1);
