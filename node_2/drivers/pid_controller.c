#include "pid_controller.h"

void pid_init(PID_CONTROLLER *pid_ctrl, uint32_t K_p, uint32_t K_i, uint32_t K_d){
    pid_ctrl->
}
void pid_get_referance(PID_CONTROLLER *pid_ctrl);
void pid_update_measurement(PID_CONTROLLER *pid_ctrl);
void pid_update_error(PID_CONTROLLER *pid_ctrl);
void pid_kalkulate_controller_output(PID_CONTROLLER *pid_ctrl);