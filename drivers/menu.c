#include "menu.h"

volatile uint8_t high_scores[5] = {0};

void main_menu(void){
    printf("I Main menu");
    draw_main_menu();
    while (1){
        _delay_us(6000);
        switch (main_menu_state)
        {
        case NEW_GAME:
            oled_draw(ARROW, 2, 0); //drawing the correct arrow
            oled_draw(EMPTY, 4, 0);
            oled_draw(EMPTY, 6, 0);
            oled_update_screen();

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
            oled_draw(EMPTY, 2, 0); //drawing the correct arrow
            oled_draw(ARROW, 4, 0);
            oled_draw(EMPTY, 6, 0);
            oled_update_screen();

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
            oled_draw(EMPTY, 2, 0); //drawing the correct arrow
            oled_draw(EMPTY, 4, 0);
            oled_draw(ARROW, 6, 0);
            oled_update_screen();

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

void new_game_menu(void){
    draw_new_game_menu();
    oled_draw(ARROW, 2, 63);
    oled_update_screen();
    while(1){
        get_io_board_values();
        get_io_board_directions();
        switch (joystick_dir)
        {
        case RIGHT:
            // start a new pong game!
            break;
        case LEFT:
            // go back to main menu
            main_menu();
            break;
        default:
            break;
        }
    }
} //flytter pila

void score_menu(void){
    draw_score_menu();
    get_io_board_values();
    get_io_board_directions();
    while (joystick_dir != LEFT){
        get_io_board_values();
        get_io_board_directions();
        _delay_us(2000);
    }
    main_menu();
} //flytter pila

void settings_menu(void){
    draw_settings_menu();

    while (1){
        _delay_us(6000);
        switch (settings_menu_state)
        {
        case CALIBRATION:
            oled_draw(ARROW, 2, 63); //drawing the correct arrow
            oled_draw(EMPTY, 4, 63);
            oled_draw(EMPTY, 6, 63);
            oled_update_screen();

            get_io_board_values();
            get_io_board_directions();

            switch (joystick_dir)
            {
            case DOWN:
                settings_menu_state = BRIGHTNESS;
                break;
            case RIGHT:
                //start calibartion !
                break;
            case LEFT:
                main_menu();
                break;
            default:
                break;
            }
            break;

        case BRIGHTNESS:
            oled_draw(EMPTY, 2, 63); //drawing the correct arrow
            oled_draw(ARROW, 4, 63);
            oled_draw(EMPTY, 6, 63);
            oled_update_screen();

            get_io_board_values();
            get_io_board_directions();

            switch (joystick_dir)
            {
            case UP:
                settings_menu_state = CALIBRATION;
                break;
            case DOWN:
                settings_menu_state = DIFFICULTY;
                break;
            case RIGHT:
                // toggle brightness?
                break;
            case LEFT:
                main_menu();
                break;
            default:
                break;
            }
            break;

        case DIFFICULTY:
            oled_draw(EMPTY, 2, 63); //drawing the correct arrow
            oled_draw(EMPTY, 4, 63);
            oled_draw(ARROW, 6, 63);
            oled_update_screen();

            get_io_board_values();
            get_io_board_directions();

            switch (joystick_dir)
            {
            case UP:
                settings_menu_state = BRIGHTNESS;
                break;
            case RIGHT:
                //toggle difficulty??
                break;
            case LEFT:
                main_menu();
                break;
            default:
                break;
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
    char* start = "Start";
    oled_write_string(start, 4, 2, 71);
    oled_update_screen();
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
    oled_update_screen();
} //tegne til undermeny i nedre høyre halvdel

void draw_settings_menu(void){
    char* Calinration_str = "Calibration";
    oled_write_string(Calinration_str, 4, 2, 71);
    char* Brightness_str = "Brightness";
    oled_write_string(Brightness_str, 4, 4, 71);
    char* Difficulty_str = "Diffuculty";
    oled_write_string(Difficulty_str, 4, 6, 71);
    oled_update_screen();
} //tegne til undermeny i nedre høyre halvdel

void clear_submenu(void){
    printf("FJERNER SUBMENY!");
    for (uint8_t page = 2; page < 8; page++){
        for (int col = 63; col < 128; col++){ // OBS!!!
            oled_write_byte(page, col, 0x00);
        }
    }
}