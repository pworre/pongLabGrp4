#define F_CPU 4915200UL
#include "oled.h"
#include "io_board.h"

#ifndef MENU_H
#define MENU_H

typedef enum  {
    NEW_GAME,
    SCORE,
    //SETTINGS,
} MAIN_MENU_STATES;

// typedef enum  {
//     CALIBRATION,
//     BRIGHTNESS,
//     DIFFICULTY,
// } SETTINGS_MENU_STATES;

volatile MAIN_MENU_STATES main_menu_state;

//volatile SETTINGS_MENU_STATES settings_menu_state;

void main_menu(void);  //moves the arrow

uint8_t new_game_menu(void); //moves the arrow

void score_menu(void); //moves the arrow

//void settings_menu(void); //moves the arrow

void draw_main_menu(void); //prints the main menu 

void draw_new_game_menu(void); //draws the new game menu down left

void draw_score_menu(void); //draws the scores down left

//void draw_settings_menu(void); //removed since we dont use it

void clear_submenu(void);

void draw_gameplay(void); //draws the playing game menu

void update_gameplay(uint8_t timerCounter, uint8_t goals); //updates the score and goals

void print_gameover(uint8_t timerCounter); 

void update_highscore(uint8_t score); //puts inn new score and sorts it high to low


#endif 