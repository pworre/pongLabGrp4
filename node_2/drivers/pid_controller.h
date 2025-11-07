#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "sam.h"

typedef struct {
    uint32_t K_p;
    uint32_t K_i;
    uint32_t K_d;
    uint32_t measurement;
    uint32_t previous_measurement;
    uint32_t referance;
    uint32_t error;
    uint32_t controller_output;
} PID_CONTROLER;

void pid_init(PID_CONTROLLER *pid_ctrl, uint32_t K_p, uint32_t K_i, uint32_t K_d);
void pid_get_referance(PID_CONTROLLER *pid_ctrl);
void pid_update_measurement(PID_CONTROLLER *pid_ctrl);
void pid_update_error(PID_CONTROLLER *pid_ctrl);
void pid_kalkulate_controller_output(PID_CONTROLLER *pid_ctrl);


#endif