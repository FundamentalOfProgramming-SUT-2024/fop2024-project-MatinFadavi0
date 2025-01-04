#include<ncurses.h>
#include "login_game_menu.h"


int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    load_users();
    draw_menu();

    endwin();
    return 0;
}