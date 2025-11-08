#include "pid_controller.h"

void pid_init(PID_CONTROLLER *pid_ctrl, float K_p, float K_i, float K_d, float T){

    pid_ctrl->K_p = K_p;
    pid_ctrl->K_i = K_i;
    pid_ctrl->K_d = K_d;
    pid_ctrl->T = T;

    pid_ctrl->error = 0;
    pid_ctrl->previous_error = 0;

    pid_ctrl->measurement = read_encoder();
    pid_ctrl->reference = pid_ctrl->measurement;

    pid_ctrl->integral = 0;
    pid_ctrl->derivate = 0;
    pid_ctrl->controller_output = 0;
}

void pid_update_referance(PID_CONTROLLER *pid_ctrl){
    int8_t x_axis = can_message[0]
    //removes the stickdrift
    if (x_axis > 10){
        pid_ctrl->reference += (x_axis * ENCODER_MAX) / 2000; //can cange the reference 5% of the max value at a time step
    } else if( x_axis < -10){
        pid_ctrl->reference += (x_axis * ENCODER_MAX) / 2000;
    } else {
        return;
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
void pid_update_integral(PID_CONTROLER *pid_ctrl){
    pid_ctrl->integral += pid_ctrl->error;
}
void pid_update_derivate(PID_CONTROLER *pid_ctrl){
    //TODO: maybe we need a better derivate calculation
    pid_ctrl->derivate = (pid_ctrl->error - pid_ctrl->previous_error) / pid_ctrl->T;
}
void pid_calculate_controller_output(PID_CONTROLLER *pid_ctrl){
    pid_ctrl->controller_output = (uint32_t)((pid_ctrl->K_p * pid_ctrl->error) 
                                + (pid_ctrl->K_i * pid_ctrl->T * pid_ctrl->integral) 
                                + ((pid_ctrl->K_d / pid_ctrl->T) * pid_ctrl->derivate));
}
void pid_set_motor_power(PID_CONTROLLER *pid_ctrl){
    if (pid_ctrl->controller_output < 0){
        motor_setdir(LEFT);
        motor_setpower(pid_ctrl->controller_output);
    } else (pid->controller_output >= 0){
        motor_setdir(RIGHT);
        motor_setpower(pid_ctrl->controller_output);
    }
}
void pid_use_controller(PID_CONTROLLER *pid_ctrl){
    pid_update_referance(pid_ctrl);
    pid_update_measurement(pid_ctrl);
    pid_update_derivate(pid_ctrl);
    pid_update_derivate(pid_ctrl);
    pid_update_integral(pid_ctrl);
    pid_calculate_controller_output(pid_ctrl);
    pid_set_motor_power(pid_ctrl);
}