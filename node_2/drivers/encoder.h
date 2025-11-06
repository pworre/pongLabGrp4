#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>
#include"sam.h"

void init_encoder(void);
uint32_t read_encoder(void);

#endif