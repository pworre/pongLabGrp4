#define F_CPU 4915200UL
#include "oled.h"
#include "io_board.h"

#ifndef MENU_H
#define MENU_H

typedef enum  {
    NEW_GAME,
    SCORE,
    SETTINGS,
} MAIN_MENU_STATES;

typedef enum  {
    CALIBRATION,
    BRIGHTNESS,
    DIFFICULTY,
} SETTINGS_MENU_STATES;

volatile MAIN_MENU_STATES main_menu_state;

volatile SETTINGS_MENU_STATES settings_menu_state;

 //flyttes kanskje til main? eller slik at den kan oppdateres etter endt spill

void main_menu(void);  //flytter pila

void new_game_menu(void); //flytter pila

void score_menu(void); //flytter pila

void settings_menu(void); //flytter pila

void draw_main_menu(void); //tegne til hele skjermen

void draw_new_game_menu(void); //tegne til undermeny i nedre høyre halvdel

void draw_score_menu(void); //tegne til undermeny i nedre høyre halvdel

void draw_settings_menu(void); //tegne til undermeny i nedre høyre halvdel

void clear_submenu(void);

#endif 