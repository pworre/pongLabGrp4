#include "menu.h"

volatile uint8_t high_scores[5] = {76, 46, 23, 3, 1};

void main_menu(void){
    draw_main_menu();
    while (1){
        _delay_ms(50);
        switch (main_menu_state)
        {
        case NEW_GAME:
            oled_draw(ARROW, 2, 0); //drawing the correct arrow
            oled_draw(EMPTY, 4, 0);
            oled_draw(EMPTY, 6, 0);
            oled_update_screen();

            get_io_board_values();

            if (buttons.ND == 1){
                main_menu_state = SCORE;
            }

            else if (buttons.NR == 1){
                new_game_menu();
            }
            break;

        case SCORE:
            oled_draw(EMPTY, 2, 0); //drawing the correct arrow
            oled_draw(ARROW, 4, 0);
            oled_draw(EMPTY, 6, 0);
            oled_update_screen();

            get_io_board_values();

            
            if (buttons.NU == 1){
                main_menu_state = NEW_GAME;
            }
            else if (buttons.ND == 1){
                main_menu_state = SETTINGS;
            }
            else if (buttons.NR == 1){
                score_menu();
            }
            break;

        case SETTINGS:
            oled_draw(EMPTY, 2, 0); //drawing the correct arrow
            oled_draw(EMPTY, 4, 0);
            oled_draw(ARROW, 6, 0);
            oled_update_screen();

            get_io_board_values();

            if (buttons.NU == 1){
                main_menu_state = SCORE;
            }
            else if (buttons.NR == 1){
                settings_menu();
            }
            break;
        
        default:
            break;
        }
    }
}  //flytter pila

void new_game_menu(void){
    draw_new_game_menu();
    oled_draw(ARROW, 2, 63);
    oled_update_screen();
    while(1){
        get_io_board_values();

        if (buttons.NB == 1){
                //start new game
            }

        else if (buttons.NL == 1){
                main_menu();
            }
        
    }
} //flytter pila

void score_menu(void){
    draw_score_menu();
    get_io_board_values();
    while (buttons.NL != 1){
        get_io_board_values();
        _delay_us(2000);
    }
    main_menu();
} //flytter pila

void settings_menu(void){
    draw_settings_menu();

    while (1){
        _delay_ms(50);
        switch (settings_menu_state)
        {
        case CALIBRATION:
            oled_draw(ARROW, 2, 63); //drawing the correct arrow
            oled_draw(EMPTY, 4, 63);
            oled_draw(EMPTY, 6, 63);
            oled_update_screen();

            get_io_board_values();

            if (buttons.NB == 1){
                //start calibration
            }
            else if (buttons.NL == 1){
                main_menu();
            }
            else if (buttons.ND == 1){
                settings_menu_state = BRIGHTNESS;
            }
            break;

        case BRIGHTNESS:
            oled_draw(EMPTY, 2, 63); //drawing the correct arrow
            oled_draw(ARROW, 4, 63);
            oled_draw(EMPTY, 6, 63);
            oled_update_screen();

            get_io_board_values();

            if (buttons.NB == 1){
                //start britess elns
            }
            else if (buttons.NL == 1){
                main_menu();
            }
            else if (buttons.NU == 1){
                settings_menu_state = CALIBRATION;
            }
            else if (buttons.ND == 1){
                settings_menu_state = DIFFICULTY;
            }
            break;

        case DIFFICULTY:
            oled_draw(EMPTY, 2, 63); //drawing the correct arrow
            oled_draw(EMPTY, 4, 63);
            oled_draw(ARROW, 6, 63);
            oled_update_screen();

            get_io_board_values();

            if (buttons.NB == 1){
                //start difficulty elns
            }
            else if (buttons.NL == 1){
                main_menu();
            }
            else if (buttons.NU == 1){
                settings_menu_state = BRIGHTNESS;
            }
            break;
        }
    }
} //flytter pila

void draw_main_menu(void){
    clear_submenu();
    char* header = "MAIN  MENU";
    oled_write_inverted_string(header, 8, 0, 23);
    char* new_game_str = "New Game";
    oled_write_string(new_game_str, 5, 2, 8);
    char* Score_str = "Score";
    oled_write_string(Score_str, 5, 4, 8);
    char* Settings_str = "Settings";
    oled_write_string(Settings_str, 5, 6, 8);
    oled_update_screen();
} //tegne til hele skjermen

void draw_new_game_menu(void){
    char* header = "NEW GAME";
    oled_write_inverted_string(header, 8, 0, 23);
    char* start = "Start";
    oled_write_string(start, 4, 2, 71);
    oled_update_screen();
} //tegne til undermeny i nedre høyre halvdel

/*
void draw_score_menu(void){
    char* place = '1' - 32;
    char* score;
    char* dot = ".";
    uint8_t i = 0;

    for (uint8_t page = 2; page<7; page++){
        
        *score = int_to_str(high_scores[i]);
        oled_write_string(place, 4, page, 63);
        oled_write_string(dot, 4, page, 67);
        oled_write_string(score, 4, page, 71);
        *place ++;
        i++;
    }
    oled_update_screen();
} //tegne til undermeny i nedre høyre halvdel

*/

void draw_score_menu(void) {
    char* header = "HIGH SCORES";
    oled_write_inverted_string(header, 8, 0, 23);

    for (uint8_t i = 0; i < 5; i++) {
        char line[10];  // midlertidig buffer for teksten på hver linje
        char num_str[4];
        uint8_t score = high_scores[i];

        // lag streng for plassnummer (1-5)
        char place = '1' + i;

        // konverter score til tekst
        snprintf(num_str, sizeof(num_str), "%d", score);

        // lag linjetekst i formatet "1. 76"
        snprintf(line, sizeof(line), "%c. %s", place, num_str);

        // skriv til OLED, f.eks. én linje per side (page)
        oled_write_string(line, 4, 2 + i, 63);
    }

    oled_update_screen();
}


void draw_settings_menu(void){
    char* header = "SETTINGS";
    oled_write_inverted_string(header, 8, 0, 23);
    char* Calinration_str = "Calibration";
    oled_write_string(Calinration_str, 4, 2, 71);
    char* Brightness_str = "Brightness";
    oled_write_string(Brightness_str, 4, 4, 71);
    char* Difficulty_str = "Diffuculty";
    oled_write_string(Difficulty_str, 4, 6, 71);
    oled_update_screen();
} //tegne til undermeny i nedre høyre halvdel

void clear_submenu(void){
    for (uint8_t page = 2; page < 8; page++){
        for (int col = 63; col < 128; col++){ // OBS!!!
            oled_write_byte(page, col, 0x00);
        }
    }
}