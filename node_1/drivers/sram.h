#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef sram_H
#define sram_H

// BLOCKED (fonts) : SRAM 0x00 -> 0x64F

void sram_init();
void SRAM_test(void);

#endif