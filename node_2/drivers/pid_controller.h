#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include "sam.h"
#include "motor.h"
#include "encoder.h"

typedef struct {
    //TODO: go over the types
    float K_p;
    float K_i;
    float K_d;
    float T;
    int32_t measurement;
    int32_t referance;
    int32_t error;
    int32_t previous_error;
    float integral;
    float derivate;
    float controller_output;
} PID_CONTROLER;

void pid_init(PID_CONTROLLER *pid_ctrl, uint32_t K_p, uint32_t K_i, uint32_t K_d);
void pid_update_referance(PID_CONTROLLER *pid_ctrl);
void pid_update_measurement(PID_CONTROLLER *pid_ctrl);
void pid_update_error(PID_CONTROLLER *pid_ctrl);
void pid_update_integral(PID_CONTROLER *pid_ctrl);
void pid_update_derivate(PID_CONTROLER *pid_ctrl);
void pid_calculate_controller_output(PID_CONTROLLER *pid_ctrl);
void pid_set_motor_power(PID_CONTROLLER *pid_ctrl);
void pid_use_controller(PID_CONTROLLER *pid_ctrl);


#endif