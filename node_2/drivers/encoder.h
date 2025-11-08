#ifndef ENCODER_H
#define ENCODER_H

#define ENCODER_MAX 5612

#include <stdint.h>
#include"sam.h"
#include "motor.h"


void init_encoder(void);
uint32_t read_encoder(void);
void encoder_calibrate(void);

#endif