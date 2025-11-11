#ifndef THE_GAME_H
#define THE_GAME_H

#include <stdio.h>
#include <stdint.h>
#include "can_controller.h"
#include "adc.h"
#include "pid_controller.h"
#include "timer_counter.h"

typedef enum  {
    MENU, 
    PLAY
} GAME_STATES;

// x x x x x x x S (1st byte)
// G G G G G G G G (2nd byte)
// T T T T T T T T (3rd byte)
typedef struct {
    GAME_STATES state;
    GAME_MODE mode;
    uint32_t goal;
    uint32_t elapsed_time;
} GAME;

typedef struct {
    union {
        uint8_t right;
        struct {
            uint8_t R1:1;
            uint8_t R2:1;
            uint8_t R3:1;
            uint8_t R4:1;
            uint8_t R5:1;
            uint8_t R6:1;
        };
    };
    union {
        uint8_t left;
        struct {
            uint8_t L1:1;
            uint8_t L2:1;
            uint8_t L3:1;
            uint8_t L4:1;
            uint8_t L5:1;
            uint8_t L6:1;
            uint8_t L7:1;
        };
    };
    union {
        uint8_t nav;
        struct {
            uint8_t NB:1;
            uint8_t NR:1;
            uint8_t ND:1;
            uint8_t NL:1;
            uint8_t NU:1;
        };
    };
} BUTTONS;


typedef struct {
    int32_t x_axis;
    int32_t y_axis;
    BUTTONS buttons;
} JOYSTICK;

void play_the_game(void);
void game_init(void);
void score_init(void);

#endif