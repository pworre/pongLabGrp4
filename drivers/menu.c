#include "menu.h"


void main_menu(void);  //flytter pila

void new_game_menu(void); //flytter pila

void score_menu(void); //flytter pila

void settings_menu(void); //flytter pila

void draw_main_menu(void){
    char* header = "MAIN  MENU";
    oled_write_inverted_string(header, 8, 0, 23);
} //tegne til hele skjermen

void draw_new_game_menu(void); //tegne til undermeny i nedre høyre halvdel

void draw_score_menu(void); //tegne til undermeny i nedre høyre halvdel

void draw_settings_menu(void); //tegne til undermeny i nedre høyre halvdel