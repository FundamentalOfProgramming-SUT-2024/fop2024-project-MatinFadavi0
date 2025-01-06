#include<ncurses.h>
#include "header.h"


int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

//    load_users();

    User p;
    Game g;
    g.difficulty = 0; g.player_color = 0;

    draw_menu();

    endwin();
    return 0;
}