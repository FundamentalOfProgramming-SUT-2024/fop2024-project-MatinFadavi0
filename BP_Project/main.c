#include "header.h"

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();

    srand(time(NULL));
    
    User *p = (User *) malloc(sizeof(User));
    Game *g = (Game *) malloc(sizeof(Game));
   
    g->difficulty = 0; g->player_color = 0;
    // p->score = 0;
    // p->gold = 0;
    draw_menu(p,g);

    endwin();
    return 0;
}