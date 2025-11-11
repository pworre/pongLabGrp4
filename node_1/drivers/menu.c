#include "menu.h"

volatile uint8_t high_scores[5] = {0, 0, 0, 0, 0};

void main_menu(void){
    oled_clear_buffer();
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
                if(new_game_menu()){
                    return;
                }
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
            // else if (buttons.ND == 1){
            //     main_menu_state = SETTINGS;
            // }
            else if (buttons.NR == 1){
                score_menu();
            }
            break;
/*
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
*/        
        default:
            break;
        }
    }
}  //flytter pila

uint8_t new_game_menu(void){
    draw_new_game_menu();
    oled_draw(ARROW, 2, 63);
    oled_update_screen();
    while(1){
        get_io_board_values();

        if (buttons.NB == 1){
                return 1;
            }

        else if (buttons.NL == 1){
                clear_submenu();
                return 0;
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
    clear_submenu();
    return;
} //flytter pila
/*
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

                settings_menu();
            get_io_board_values();

            if (buttons.NB == 1){
                //start calibration
            }
            else if (buttons.NL == 1){
                clear_submenu();
                return;
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
                clear_submenu();
                return;
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
                clear_submenu();
                return;
            }
            else if (buttons.NU == 1){
                settings_menu_state = BRIGHTNESS;
            }
            break;
        }
    }
} //flytter pila
*/

void draw_main_menu(void){
    clear_submenu();
    char* header = "MAIN  MENU";
    oled_write_inverted_string(header, 8, 0, 23);
    char* new_game_str = "New Game";
    oled_write_string(new_game_str, 5, 2, 8);
    char* Score_str = "Score";
    oled_write_string(Score_str, 5, 4, 8);
    // char* Settings_str = "Settings";
    // oled_write_string(Settings_str, 5, 6, 8);
    oled_update_screen();char* Settings_str = "Settings";
    // oled_write_string(Settings_str, 5, 6, 8);
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
        char line[10];
        char num_str[4];
        uint8_t score = high_scores[i];

        char place = '1' + i;

        // make string 
        snprintf(num_str, sizeof(num_str), "%d", score);
        snprintf(line, sizeof(line), "%c. %s", place, num_str);

        oled_write_string(line, 4, 2 + i, 63);
    }

    oled_update_screen();
}

/*
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
 */

void clear_submenu(void){
    for (uint8_t page = 2; page < 8; page++){
        for (int col = 63; col < 128; col++){ // OBS!!!
            oled_write_byte(page, col, 0x00);
        }
    }
}

void draw_gameplay(void) {
    oled_clear_buffer();
    char* timeHeader = "PLAYTIME:";
    oled_write_string(timeHeader, 5, 2, 25);
    char* timer = "0";
    oled_write_string(timer, 5, 2, 100);
    char* goalHeader = "GOALS SCORED:";
    oled_write_string(goalHeader, 5, 5, 25);
    char* goals = "0";
    oled_write_string(goals, 5, 5, 100);
    oled_update_screen();
} //tegner score-counter under spill

void update_gameplay(uint8_t timerCounter, uint8_t goals) {
    char timer_str[10];
    char goal_str[10];
    snprintf(timer_str, sizeof(timer_str), "%d", timerCounter);
    snprintf(goal_str, sizeof(goal_str), "%d", goals);
    oled_write_string(timer_str, 5, 2, 100);
    oled_write_string(goal_str, 5, 5, 100);
    oled_update_screen();
} //oppdatere score-counter under spill


void print_gameover(uint8_t timerCounter) {
    oled_clear_buffer();
    char* game_over = "GAME OVER!";
    char* final_time = "Final Score: ";
    char time_string[10];
    snprintf(time_string, sizeof(time_string), "%d", timerCounter);
    oled_write_string(game_over, 5, 3, 25);
    oled_write_string(final_time, 4, 5, 23);
    oled_write_string(time_string, 4, 5, 35);
    oled_update_screen();
}

void update_highscore(uint8_t score){
    high_scores[5] = score;
    for (int i = 0; i < 5 - 1; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (high_scores[j] > high_scores[i]) {
                uint8_t temp = high_scores[i];
                high_scores[i] = high_scores[j];
                high_scores[j] = temp;
            }
        }
    }
}