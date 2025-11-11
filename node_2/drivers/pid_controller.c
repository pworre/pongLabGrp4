#include "pid_controller.h"

CAN_MESSAGE can_message;

void pid_init(PID_CONTROLLER *pid_ctrl, float K_p, float K_i, float K_d, float T){

    pid_ctrl->K_p = K_p;
    pid_ctrl->K_i = K_i;
    pid_ctrl->K_d = K_d;
    pid_ctrl->T = T;

    pid_ctrl->error = 0;
    pid_ctrl->previous_error = 0;

    pid_ctrl->measurement = read_encoder();
    pid_ctrl->reference = ENCODER_MAX/2;

    pid_ctrl->integral = 0;
    pid_ctrl->derivate = 0;
    pid_ctrl->controller_output = 0;
}

void pid_update_referance(PID_CONTROLLER *pid_ctrl, JOYSTICK *joystick){
    //removes the stickdrift
    if (joystick->x_axis < 10){
        pid_ctrl->reference -= (x_axis / 2);
    } else if(joystick->x_axis > -10){
        pid_ctrl->reference -= (x_axis / 2);
    } 
    if (pid_ctrl->reference < 5){
        pid_ctrl->reference = 5;
    } else if (abs(pid_ctrl->reference) > ENCODER_MAX){
        pid_ctrl->reference = ENCODER_MAX;
    }
}
void pid_update_measurement(PID_CONTROLLER *pid_ctrl){
    //the measurement should be between 0 and ENCODER_MAX, if encoder_calibrate have been run
    pid_ctrl->measurement = read_encoder();
}
void pid_update_error(PID_CONTROLLER *pid_ctrl){
    pid_ctrl->previous_error = pid_ctrl->error;
    pid_ctrl->error = pid_ctrl->reference - pid_ctrl->measurement;
} 
void pid_update_integral(PID_CONTROLLER *pid_ctrl){
    pid_ctrl->integral += pid_ctrl->error;
}
void pid_update_derivate(PID_CONTROLLER *pid_ctrl){
    //TODO: maybe we need a better derivate calculation
    pid_ctrl->derivate = (pid_ctrl->error - pid_ctrl->previous_error) / pid_ctrl->T;
}
void pid_calculate_controller_output(PID_CONTROLLER *pid_ctrl){
    pid_ctrl->controller_output = (int32_t)((pid_ctrl->K_p * pid_ctrl->error) 
                                + (pid_ctrl->K_i * pid_ctrl->T * pid_ctrl->integral) 
                                + ((pid_ctrl->K_d / pid_ctrl->T) * pid_ctrl->derivate));
}
void pid_set_motor_power(PID_CONTROLLER *pid_ctrl){
    if (pid_ctrl->controller_output < 0){
        motor_setdir(RIGHT);
        motor_setpower(abs(pid_ctrl->controller_output));
    } else {
        motor_setdir(LEFT);
        motor_setpower(abs(pid_ctrl->controller_output));
    }
}

void pid_use_controller(PID_CONTROLLER *pid_ctrl, JOYSTICK *joystick){
    pid_update_referance(pid_ctrl, joystick);
    pid_update_measurement(pid_ctrl);
    pid_update_error(pid_ctrl);
    pid_update_derivate(pid_ctrl);
    pid_update_integral(pid_ctrl);
    pid_calculate_controller_output(pid_ctrl);
    pid_set_motor_power(pid_ctrl);
}