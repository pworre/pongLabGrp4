#include "menu.h"


void main_menu(void){
    main_menu_state = NEW_GAME; //skal initialiseres et annet sted
    draw_main_menu();
    while (1){
        switch (main_menu_state)
        {
        case NEW_GAME:
            oled_draw(figures[1], 2, 0); //drawing the correct arrow
            oled_write_string(' ', 8, 4, 0);
            oled_write_string(' ', 8, 6, 0);

            get_io_board_values();
            get_io_board_directions();

            switch (joystick_dir)
            {
            case DOWN:
                main_menu_state = SCORE;
            
                break;
            case RIGHT:
                new_game_menu();
                break;
            default:
                break;
            }
            break;

        case SCORE:
            oled_draw(figures[1], 4, 0); //drawing the correct arrow
            oled_write_string(' ', 8, 2, 0);
            oled_write_string(' ', 8, 6, 0);

            get_io_board_values();
            get_io_board_directions();

            switch (joystick_dir)
            {
            case UP:
                main_menu_state = NEW_GAME;
            
                break;
            case DOWN:
                main_menu_state = SETTINGS;
            
                break;
            case RIGHT:
                score_menu();
                break;
            default:
                break;
            }
            break;

        case SETTINGS:
            oled_draw(figures[1], 6, 0); //drawing the correct arrow
            oled_write_string(' ', 8, 2, 0);
            oled_write_string(' ', 8, 4, 0);

            get_io_board_values();
            get_io_board_directions();

            switch (joystick_dir)
            {
            case UP:
                main_menu_state = SCORE;
            
                break;
            case RIGHT:
                settings_menu();
                break;
            default:
                break;
            }
            break;
        
        default:
            break;
        }
    }
}  //flytter pila

void new_game_menu(void); //flytter pila

void score_menu(void); //flytter pila

void settings_menu(void); //flytter pila

void draw_main_menu(void){
    char* header = "MAIN  MENU";
    oled_write_inverted_string(header, 8, 0, 23);
    char* new_game_str = "New Game";
    oled_write_string(new_game_str, 5, 2, 8)
    char* Score_str = "Score";
    oled_write_string(Score_str, 5, 4, 8)
    char* Settings_str = "Settings";
    oled_write_string(Settings_str, 5, 6, 8)
} //tegne til hele skjermen

void draw_new_game_menu(void){
    char* start = "Start";
    oled_write_inverted_string(Start, 4, 2, 71);
} //tegne til undermeny i nedre høyre halvdel

void draw_score_menu(void){
    uint8_t page;
    char* place;
    char* score;
    for (uint8_t i = 0; i<5; i++){
        page = i+2;
        *place = i;
        score = int_to_str(high_scores[i]);
        oled_write_string(place, 4, page, 63);
        oled_write_string('.', 4, page, 67);
        oled_write_string(score, 4, page, 71);
    }
} //tegne til undermeny i nedre høyre halvdel

void draw_settings_menu(void){
    char* Calinration_str = "Calibration";
    oled_write_inverted_string(Calinration_str, 4, 2, 71);
    char* Brightness_str = "Brightness";
    oled_write_inverted_string(Brightness_str, 4, 4, 71);
    char* Difficulty_str = "Diffuculty";
    oled_write_inverted_string(Difficulty_str, 4, 6, 71);
} //tegne til undermeny i nedre høyre halvdel