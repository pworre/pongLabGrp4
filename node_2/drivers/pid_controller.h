#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

typedef struct joystick JOYSTICK;


#include "sam.h"
#include "the_game.h"
#include "motor.h"
#include "encoder.h"
#include "can_controller.h"

typedef struct pid_controller{
    //TODO: go over the types
    float K_p;
    float K_i;
    float K_d;
    float T;
    uint32_t measurement;
    uint32_t reference;
    int32_t error;
    int32_t previous_error;
    float integral;
    float derivate;
    int32_t controller_output;
} PID_CONTROLLER;

void pid_init(PID_CONTROLLER *pid_ctrl, float K_p, float K_i, float K_d, float T);
void pid_update_referance(PID_CONTROLLER *pid_ctrl);
void pid_update_measurement(PID_CONTROLLER *pid_ctrl);
void pid_update_error(PID_CONTROLLER *pid_ctrl);
void pid_update_integral(PID_CONTROLLER *pid_ctrl);
void pid_update_derivate(PID_CONTROLLER *pid_ctrl);
void pid_calculate_controller_output(PID_CONTROLLER *pid_ctrl);
void pid_set_motor_power(PID_CONTROLLER *pid_ctrl);
void pid_use_controller(PID_CONTROLLER *pid_ctrl);


#endif