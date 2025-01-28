#include "header.h"

int power(int num, int n) {
    if(n==0) {
        return 1;
    }
    if(n==1) {
        return num;
    }
    for(int i=2; i<=n; i++) {
        num *= num;
    }
    return num;
}

const char *txt_format(char username[]) {
    char *file_name = (char *) malloc(50*sizeof(char));
    strcpy(file_name,username);
    file_name[strlen(username)] = '.'; file_name[strlen(username)+1] = 't';
    file_name[strlen(username)+2] = 'x'; file_name[strlen(username)+3] = 't'; file_name[strlen(username)+4] = 0;
    return file_name;
}

void GameLauncher(User *p, Game *g) {
    clear();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);

    if(g->difficulty) {
        g->MAX_health = 5;
    }
    else {
        g->MAX_health = 10;
    }
    g->players_health = g->MAX_health;
    g->players_score = 0;
    g->players_gold = 0;
    g->floor_number = 1;
    g->players_hungriness = 0;
    g->players_ordinary_food = 0;
    g->start_time = time(NULL);

    p->count_games++;

    FloorGenerator(p, g);
    

    clear();
}

void FloorGenerator(User *p, Game *g) {
    clear();

    if(g->floor_number == 1) {

        map(g,6);
    }
    else if(g->floor_number == 2) {

        map(g,check_room(g->rooms,g->player_pos.x,g->player_pos.y));
    }
    else if(g->floor_number == 3) {

        map(g,check_room(g->rooms,g->player_pos.x,g->player_pos.y));
    }
    else if(g->floor_number == 4) {

        map(g,check_room(g->rooms,g->player_pos.x,g->player_pos.y));
    }

    for(int k=0; k<6; k++) {
        g->rooms[k].pillar_seed.x = g->rooms[k].room_pos.x + power(-1,rand()%2)*(rand()%(g->rooms[k].room_size_h-3));
        g->rooms[k].pillar_seed.y = g->rooms[k].room_pos.y + power(-1,rand()%2)*(rand()%(g->rooms[k].room_size_v-3));
        g->rooms[k].traps_count = 1+rand()%3;
        g->rooms[k].gold = 1 + rand()%4;
        g->rooms[k].dark_gold = rand()%2 * rand()%2;
        g->rooms[k].ordinary_food = 1 + rand()%2;
    }

    for(int j=1; j<LINES; j++) {
        for(int i=0; i<COLS; i++) {
            for(int k=0; k<6; k++) {
                if(abs(j-g->rooms[k].room_pos.y) == g->rooms[k].room_size_v && abs(i-g->rooms[k].room_pos.x) <= g->rooms[k].room_size_h) {
                    mvprintw(j, i, "-");
                }
                else if(abs(i-g->rooms[k].room_pos.x) == g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v) {
                    mvprintw(j, i, "|");
                }
                else if(abs(i-g->rooms[k].room_pos.x) < g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v) {
                    mvprintw(j, i, ".");
                }
            }
        }
    }

    int k1 = rand()%6; int k2 = rand()%6;
    for(int k=0; k<6; k++) {
        int d1=1; int d2=1; int stairs = 0; int trap = 0; int gold = 0; int dark_gold = 0; int ordinary_food = 0;
        for(int j=1; j<LINES; j++) {
            for(int i=0; i<COLS; i++) {
                if(k == 0) {
                    if(j-g->rooms[k].room_pos.y == g->rooms[k].room_size_v && abs(i-g->rooms[k].room_pos.x) <= g->rooms[k].room_size_h && d1) {
                        int new_i = i+rand()%g->rooms[k].room_size_h;
                        mvprintw(j, new_i, "+"); d1 = 0;
                        g->rooms[k].doors[1].x = new_i; g->rooms[k].doors[1].y = j;
                    }
                    else if(i-g->rooms[k].room_pos.x == g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v && d2) {
                        int new_j = j+rand()%g->rooms[k].room_size_v;
                        mvprintw(new_j, i, "+"); d2 = 0;
                        g->rooms[k].doors[0].x = i; g->rooms[k].doors[0].y = new_j;
                    }
                }
                else if(k == 1) {
                    if(g->rooms[k].room_pos.y-j == g->rooms[k].room_size_v && abs(i-g->rooms[k].room_pos.x) <= g->rooms[k].room_size_h && d1) {
                        int new_i = i+rand()%g->rooms[k].room_size_h;
                        mvprintw(j, new_i, "+"); d1 = 0;
                        g->rooms[k].doors[0].x = new_i; g->rooms[k].doors[0].y = j;
                    }
                    else if(i-g->rooms[k].room_pos.x == g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v && d2) {
                        int new_j = j+rand()%g->rooms[k].room_size_v;
                        mvprintw(new_j, i, "+"); d2 = 0;
                        g->rooms[k].doors[1].x = i; g->rooms[k].doors[1].y = new_j;
                    }
                }
                else if(k == 2 || k == 3) {
                    if(g->rooms[k].room_pos.x-i == g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v && d1) {
                        int new_j = j+1;
                        mvprintw(new_j, i, "+"); d1 = 0;
                        g->rooms[k].doors[0].x = i; g->rooms[k].doors[0].y = new_j;
                    }
                    else if(i-g->rooms[k].room_pos.x == g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v && d2) {
                        int new_j = j+rand()%g->rooms[k].room_size_v;
                        mvprintw(new_j, i, "+"); d2 = 0;
                        g->rooms[k].doors[1].x = i; g->rooms[k].doors[1].y = new_j;
                    }
                }
                else if(k == 4) {
                    if(j-g->rooms[k].room_pos.y == g->rooms[k].room_size_v && abs(i-g->rooms[k].room_pos.x) <= g->rooms[k].room_size_h && d1) {
                        int new_i = i+rand()%g->rooms[k].room_size_h;
                        mvprintw(j, new_i, "+"); d1 = 0;
                        g->rooms[k].doors[1].x = new_i; g->rooms[k].doors[1].y = j;
                    }
                    else if(g->rooms[k].room_pos.x-i == g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v && d2) {
                        int new_j = j+rand()%g->rooms[k].room_size_v;
                        mvprintw(new_j, i, "+"); d2 = 0;
                        g->rooms[k].doors[0].x = i; g->rooms[k].doors[0].y = new_j;
                    }
                }
                else if(k == 5) {
                    if(g->rooms[k].room_pos.y-j == g->rooms[k].room_size_v && abs(i-g->rooms[k].room_pos.x) <= g->rooms[k].room_size_h && d1) {
                        int new_i = i+rand()%g->rooms[k].room_size_h;
                        mvprintw(j, new_i, "+"); d1 = 0;
                        g->rooms[k].doors[0].x = new_i; g->rooms[k].doors[0].y = j;
                    }
                    else if(g->rooms[k].room_pos.x-i == g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v && d2) {
                        int new_j = j+rand()%g->rooms[k].room_size_v;
                        mvprintw(new_j, i, "+"); d2 = 0;
                        g->rooms[k].doors[1].x = i; g->rooms[k].doors[1].y = new_j;
                    }
                }

                if(i==g->rooms[k].pillar_seed.x && j==g->rooms[k].pillar_seed.y) {
                    if(rand()%2) {
                        mvprintw(j, i, "O"); mvprintw(j, i-1, "O"); mvprintw(j, i-2, "O"); mvprintw(j, i+1, "O"); mvprintw(j, i+2, "O");
                    }
                    else {
                        mvprintw(j, i, "O"); mvprintw(j-1, i, "O"); mvprintw(j+1, i, "O"); mvprintw(j+2, i, "O");
                    }
                }
                
                
                if(abs(i-g->rooms[k].room_pos.x) < g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v) {
                    if(g->floor_number != 4) {
                        if((k == k1 || k == k2) && k!=check_room(g->rooms,g->player_pos.x,g->player_pos.y)) {
                            if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && stairs<1) {
                                mvprintw(j, i, "<");
                                stairs++;
                            }
                        }
                    }
                    if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && trap<g->rooms[k].traps_count) {
                        g->rooms[k].traps[trap].x = i; g->rooms[k].traps[trap].y = j;
                        trap++;
                    }
                    if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && mvinch(j,i) == '.' && gold<g->rooms[k].gold) {
                        mvaddstr(j,i,"o");
                        i--;
                        gold++;
                    }
                    if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && mvinch(j,i) == '.' && dark_gold<g->rooms[k].dark_gold) {
                        mvprintw(j,i,"$");
                        dark_gold++;
                    }
                    if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && mvinch(j,i) == '.' && ordinary_food<g->rooms[k].ordinary_food) {
                        mvprintw(j,i,"f");
                        ordinary_food++;
                    }
                }
            }
        }
    }

    Corridor('v',g->rooms[0].doors[1],g->rooms[1].doors[0]);
    Corridor('h',g->rooms[0].doors[0],g->rooms[2].doors[0]);
    Corridor('h',g->rooms[2].doors[1],g->rooms[4].doors[0]);
    Corridor('h',g->rooms[1].doors[1],g->rooms[3].doors[0]);
    Corridor('h',g->rooms[3].doors[1],g->rooms[5].doors[1]);
    Corridor('v',g->rooms[4].doors[1],g->rooms[5].doors[0]);

    char **screen = (char **) malloc(COLS*sizeof(char*));
    for(int i=0; i<COLS; i++) {
        *(screen+i) = (char *) malloc(LINES*sizeof(char));
    }
    for(int j=1; j<LINES; j++) {
        for(int i=0; i<COLS; i++) {
            chtype character = mvinch(j,i);
            screen[i][j] = character;
        }
    }
    clear();

    int **visited = (int **) malloc(COLS*sizeof(int *));
    for(int i=0; i<COLS; i++) {
        *(visited+i) = (int *) calloc(LINES,sizeof(int));
    }
    int k = check_room(g->rooms,g->player_pos.x,g->player_pos.y);
    for(int i=g->rooms[k].room_pos.x-g->rooms[k].room_size_h-2; i<=g->rooms[k].room_pos.x+g->rooms[k].room_size_h+2; i++) {
        for(int j=g->rooms[k].room_pos.y-g->rooms[k].room_size_v-2; j<=g->rooms[k].room_pos.y+g->rooms[k].room_size_v+2; j++) {
            visited[i][j] = 1;
        }
    }

    int display_whole = 0;
    ShowScreen("hidden",visited,screen);
    while(1) {
        ShowMessage("Welcome to the game!", g->floor_number, g->players_score, g->players_gold);
        ShowHealth(g);
        time_t current_time = time(NULL);
        double elapsed_time = difftime(current_time, g->start_time);
        if(elapsed_time > 30) {
            if(g->players_hungriness <= 8) {
                g->players_hungriness += 2;
            }
            g->start_time = time(NULL);
            if(g->players_hungriness >= 6) {
                g->players_health -= g->players_hungriness/5;
            }
        }
        if(g->players_health == 0) {
            EndGame(p, g);
        }
        if(display_whole%2 == 1) {
            ShowScreen("view",visited,screen);
        }
        else {
            ShowScreen("hidden",visited,screen);
        }
        attron(COLOR_PAIR(g->player_color));
        mvprintw(g->player_pos.y, g->player_pos.x, "@");
        attroff(COLOR_PAIR(g->player_color));

        int ch = getch();
        if(ch == 'q') {
            Save_Game(p,g,screen);
            save_screen(p,g);
        }
        else if(ch == 'm') {
            display_whole++;
            continue;
        }
        else if(ch == 'e') {
            food_screen(g);
        }
        switch(movement(screen, visited, ch, g)) {
            case 1:
                g->floor_number += 1;
                FloorGenerator(p,g);
                break;
        }
    }
}

void map(Game *g, int initial_room) {
    int index = 0;
    for(int i=0; i<3; i++) {
        for(int j=0; j<2; j++) {
            if(index!=initial_room) {
                g->rooms[index].room_size_v = 4 + rand()%3;
                g->rooms[index].room_size_h = 8 + rand()%4;
                while(g->rooms[index].room_pos.x+g->rooms[index].room_size_h>=COLS || g->rooms[index].room_pos.x-g->rooms[index].room_size_h<=0 ||
                g->rooms[index].room_pos.y+g->rooms[index].room_size_v>=LINES || g->rooms[index].room_pos.y-g->rooms[index].room_size_v<=0) {
                    g->rooms[index].room_pos.x = rand() % ((COLS/6) + 1) + i*(COLS/3);
                    g->rooms[index].room_pos.y = rand() % ((LINES/4) + 1) + 1.5*j*(LINES/2);
                }
            }

            index++;
        }
    }
    if(initial_room == 6) {
        g->player_pos.x = g->rooms[5].room_pos.x;
        g->player_pos.y = g->rooms[5].room_pos.y;
    }

}

int movement(char **screen, int **visited, int ch, Game *g) {
    int count_dots = 0;
    char character = mvinch(g->player_pos.y-1, g->player_pos.x) & A_CHARTEXT; if(character == '.') {count_dots++;}
    character = mvinch(g->player_pos.y+1, g->player_pos.x) & A_CHARTEXT; if(character == '.') {count_dots++;}
    character = mvinch(g->player_pos.y, g->player_pos.x-1) & A_CHARTEXT; if(character == '.') {count_dots++;}
    character = mvinch(g->player_pos.y, g->player_pos.x+1) & A_CHARTEXT; if(character == '.') {count_dots++;}
    if(count_dots >= 2) {
        mvprintw(g->player_pos.y, g->player_pos.x, ".");
    }
    else if(count_dots == 1) {
        mvprintw(g->player_pos.y, g->player_pos.x, "+");
    }
    else {
        mvprintw(g->player_pos.y, g->player_pos.x, "#");
        if(abs(g->player_pos.x-g->rooms[0].room_pos.x) <= g->rooms[0].room_size_h+2 && abs(g->player_pos.y-g->rooms[0].room_pos.y) <= g->rooms[0].room_size_v+2) {
            for(int i=g->rooms[0].room_pos.x-g->rooms[0].room_size_h-2; i<=g->rooms[0].room_pos.x+g->rooms[0].room_size_h+2; i++) {
                for(int j=g->rooms[0].room_pos.y-g->rooms[0].room_size_v-2; j<=g->rooms[0].room_pos.y+g->rooms[0].room_size_v+2; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[1].room_pos.x) <= g->rooms[1].room_size_h+2 && abs(g->player_pos.y-g->rooms[1].room_pos.y) <= g->rooms[1].room_size_v+2) {
            for(int i=g->rooms[1].room_pos.x-g->rooms[1].room_size_h-2; i<=g->rooms[1].room_pos.x+g->rooms[1].room_size_h+2; i++) {
                for(int j=g->rooms[1].room_pos.y-g->rooms[1].room_size_v-2; j<=g->rooms[1].room_pos.y+g->rooms[1].room_size_v+2; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[2].room_pos.x) <= g->rooms[2].room_size_h+2 && abs(g->player_pos.y-g->rooms[2].room_pos.y) <= g->rooms[2].room_size_v+2) {
            for(int i=g->rooms[2].room_pos.x-g->rooms[2].room_size_h-2; i<=g->rooms[2].room_pos.x+g->rooms[2].room_size_h+2; i++) {
                for(int j=g->rooms[2].room_pos.y-g->rooms[2].room_size_v-2; j<=g->rooms[2].room_pos.y+g->rooms[2].room_size_v+2; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[3].room_pos.x) <= g->rooms[3].room_size_h+2 && abs(g->player_pos.y-g->rooms[3].room_pos.y) <= g->rooms[3].room_size_v+2) {
            for(int i=g->rooms[3].room_pos.x-g->rooms[3].room_size_h-2; i<=g->rooms[3].room_pos.x+g->rooms[3].room_size_h+2; i++) {
                for(int j=g->rooms[3].room_pos.y-g->rooms[3].room_size_v-2; j<=g->rooms[3].room_pos.y+g->rooms[3].room_size_v+2; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[4].room_pos.x) <= g->rooms[4].room_size_h+2 && abs(g->player_pos.y-g->rooms[4].room_pos.y) <= g->rooms[4].room_size_v+2) {
            for(int i=g->rooms[4].room_pos.x-g->rooms[4].room_size_h-2; i<=g->rooms[4].room_pos.x+g->rooms[4].room_size_h+2; i++) {
                for(int j=g->rooms[4].room_pos.y-g->rooms[4].room_size_v-2; j<=g->rooms[4].room_pos.y+g->rooms[4].room_size_v+2; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[5].room_pos.x) <= g->rooms[5].room_size_h+2 && abs(g->player_pos.y-g->rooms[5].room_pos.y) <= g->rooms[5].room_size_v+2) {
            for(int i=g->rooms[5].room_pos.x-g->rooms[5].room_size_h-2; i<=g->rooms[5].room_pos.x+g->rooms[5].room_size_h+2; i++) {
                for(int j=g->rooms[5].room_pos.y-g->rooms[5].room_size_v-2; j<=g->rooms[5].room_pos.y+g->rooms[5].room_size_v+2; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        if(ch == '8' && mvinch(g->player_pos.y-1, g->player_pos.x) != ' ') {
            for(int j=1; j<=3; j++) {
                visited[g->player_pos.x][g->player_pos.y-j] = 1;
                visited[g->player_pos.x+1][g->player_pos.y-j] = 1;
                visited[g->player_pos.x-1][g->player_pos.y-j] = 1;
            }
        }
        else if(ch == '4' && mvinch(g->player_pos.y, g->player_pos.x-1) != ' ') {
            for(int i=1; i<=3; i++) {
                visited[g->player_pos.x-i][g->player_pos.y] = 1;
                visited[g->player_pos.x-i][g->player_pos.y+1] = 1;
                visited[g->player_pos.x-i][g->player_pos.y-1] = 1;
            }
        }
        else if(ch == '2' && mvinch(g->player_pos.y+1, g->player_pos.x) != ' ') {
            for(int j=1; j<=3; j++) {
                visited[g->player_pos.x][g->player_pos.y+j] = 1;
                visited[g->player_pos.x+1][g->player_pos.y+j] = 1;
                visited[g->player_pos.x-1][g->player_pos.y+j] = 1;
            }
        }
        else if(ch == '6' && mvinch(g->player_pos.y, g->player_pos.x+1) != ' ') {
            for(int i=1; i<=3; i++) {
                visited[g->player_pos.x+i][g->player_pos.y] = 1;
                visited[g->player_pos.x+i][g->player_pos.y-1] = 1;
                visited[g->player_pos.x+i][g->player_pos.y+1] = 1;
            }
        }
    }



    switch (ch) {
        case '8':
            character = mvinch(g->player_pos.y-1, g->player_pos.x) & A_CHARTEXT;
            if(character == '<') { return 1; }
            if(character == 'o') { g->players_gold += 5; g->players_score +=10; screen[g->player_pos.x][g->player_pos.y-1]='.'; g->player_pos.y--; return 0; }
            if(character == '$') { g->players_gold += 25; g->players_score +=50; screen[g->player_pos.x][g->player_pos.y-1]='.'; g->player_pos.y--; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x][g->player_pos.y-1]='.'; g->player_pos.y--; return 0; }
            if(Trap_Check(g->rooms,g->player_pos.x,g->player_pos.y-1)) { screen[g->player_pos.x][g->player_pos.y-1]='^'; g->players_health -= 1; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y--; }
            break;
        case '2':
            character = mvinch(g->player_pos.y+1, g->player_pos.x) & A_CHARTEXT;
            if(character == '<') { return 1; }
            if(character == 'o') { g->players_gold += 5; g->players_score +=10; screen[g->player_pos.x][g->player_pos.y+1]='.'; g->player_pos.y++; return 0; }
            if(character == '$') { g->players_gold += 25; g->players_score +=50; screen[g->player_pos.x][g->player_pos.y+1]='.'; g->player_pos.y++; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x][g->player_pos.y+1]='.'; g->player_pos.y++; return 0; }
            if(Trap_Check(g->rooms,g->player_pos.x,g->player_pos.y+1)) { screen[g->player_pos.x][g->player_pos.y+1]='^'; g->players_health -= 1; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y++; }
            break;
        case '4':
            character = mvinch(g->player_pos.y, g->player_pos.x-1) & A_CHARTEXT;
            if(character == '<') { return 1; }
            if(character == 'o') { g->players_gold += 5; g->players_score +=10; screen[g->player_pos.x-1][g->player_pos.y]='.'; g->player_pos.x--; return 0; }
            if(character == '$') { g->players_gold += 25; g->players_score +=50; screen[g->player_pos.x-1][g->player_pos.y]='.'; g->player_pos.x--; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x-1][g->player_pos.y]='.'; g->player_pos.x--; return 0; }
            if(Trap_Check(g->rooms,g->player_pos.x-1,g->player_pos.y)) { screen[g->player_pos.x-1][g->player_pos.y]='^'; g->players_health -= 1; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.x--; }
            break;
        case '6':
            character = mvinch(g->player_pos.y, g->player_pos.x+1) & A_CHARTEXT;
            if(character == '<') { return 1; }
            if(character == 'o') { g->players_gold += 5; g->players_score +=10; screen[g->player_pos.x+1][g->player_pos.y]='.'; g->player_pos.x++; return 0; }
            if(character == '$') { g->players_gold += 25; g->players_score +=50; screen[g->player_pos.x+1][g->player_pos.y]='.'; g->player_pos.x++; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x+1][g->player_pos.y]='.'; g->player_pos.x++; return 0; }
            if(Trap_Check(g->rooms,g->player_pos.x+1,g->player_pos.y)) { screen[g->player_pos.x+1][g->player_pos.y]='^'; g->players_health -= 1; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.x++; }
            break;
        case '9':
            character = mvinch(g->player_pos.y-1, g->player_pos.x+1) & A_CHARTEXT;
            if(character == '<') { return 1; }
            if(character == 'o') { g->players_gold += 5; g->players_score +=10; screen[g->player_pos.x+1][g->player_pos.y-1]='.'; g->player_pos.y--; g->player_pos.x++; return 0; }
            if(character == '$') { g->players_gold += 25; g->players_score +=50; screen[g->player_pos.x+1][g->player_pos.y]='.'; g->player_pos.y--; g->player_pos.x++; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x+1][g->player_pos.y]='.'; g->player_pos.y--; g->player_pos.x++; return 0; }
            if(Trap_Check(g->rooms,g->player_pos.x+1,g->player_pos.y-1)) { screen[g->player_pos.x+1][g->player_pos.y-1]='^'; g->players_health -= 1; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y--; g->player_pos.x++; }
            break;
        case '7':
            character = mvinch(g->player_pos.y-1, g->player_pos.x-1) & A_CHARTEXT;
            if(character == '<') { return 1; }
            if(character == 'o') { g->players_gold += 5; g->players_score +=10; screen[g->player_pos.x-1][g->player_pos.y-1]='.'; g->player_pos.y--; g->player_pos.x--; return 0; }
            if(character == '$') { g->players_gold += 25; g->players_score +=50; screen[g->player_pos.x-1][g->player_pos.y-1]='.'; g->player_pos.y--; g->player_pos.x--; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x-1][g->player_pos.y-1]='.'; g->player_pos.y--; g->player_pos.x--; return 0; }
            if(Trap_Check(g->rooms,g->player_pos.x-1,g->player_pos.y-1)) { screen[g->player_pos.x-1][g->player_pos.y-1]='^'; g->players_health -= 1; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y--; g->player_pos.x--; }
            break;
        case '3':
            character = mvinch(g->player_pos.y+1, g->player_pos.x+1) & A_CHARTEXT;
            if(character == '<') { return 1; }
            if(character == 'o') { g->players_gold += 5; g->players_score +=10; screen[g->player_pos.x+1][g->player_pos.y+1]='.'; g->player_pos.y++; g->player_pos.x++; return 0; }
            if(character == '$') { g->players_gold += 25; g->players_score +=50; screen[g->player_pos.x+1][g->player_pos.y+1]='.'; g->player_pos.y++; g->player_pos.x++; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x+1][g->player_pos.y+1]='.'; g->player_pos.y++; g->player_pos.x++; return 0; }
            if(Trap_Check(g->rooms,g->player_pos.x+1,g->player_pos.y+1)) { screen[g->player_pos.x+1][g->player_pos.y+1]='^'; g->players_health -= 1; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y++; g->player_pos.x++; }
            break;
        case '1':
            character = mvinch(g->player_pos.y+1, g->player_pos.x-1) & A_CHARTEXT;
            if(character == '<') { return 1; }
            if(character == 'o') { g->players_gold += 5; g->players_score +=10; screen[g->player_pos.x-1][g->player_pos.y+1]='.'; g->player_pos.y++; g->player_pos.x--; return 0; }
            if(character == '$') { g->players_gold += 25; g->players_score +=50; screen[g->player_pos.x-1][g->player_pos.y+1]='.'; g->player_pos.y++; g->player_pos.x--; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x-1][g->player_pos.y+1]='.'; g->player_pos.y++; g->player_pos.x--; return 0; }
            if(Trap_Check(g->rooms,g->player_pos.x-1,g->player_pos.y+1)) { screen[g->player_pos.x-1][g->player_pos.y+1]='^'; g->players_health -= 1; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y++; g->player_pos.x--; }
            break;
        case 'f':
            ch = getch();
            if(ch == '8') {
                character = mvinch(g->player_pos.y-1, g->player_pos.x);
                while(character == '.' || character == '+' || character == '#') {
                    g->player_pos.y--;
                    character = mvinch(g->player_pos.y-1, g->player_pos.x);
                }
            }
            else if(ch == '2') {
                character = mvinch(g->player_pos.y+1, g->player_pos.x);
                while(character == '.' || character == '+' || character == '#') {
                    g->player_pos.y++;
                    character = mvinch(g->player_pos.y+1, g->player_pos.x);
                }
            }
            else if(ch == '4') {
                character = mvinch(g->player_pos.y, g->player_pos.x-1);
                while(character == '.' || character == '+' || character == '#') {
                    g->player_pos.x--;
                    character = mvinch(g->player_pos.y, g->player_pos.x-1);
                }
            }
            else if(ch == '6') {
                character = mvinch(g->player_pos.y, g->player_pos.x+1);
                while(character == '.' || character == '+' || character == '#') {
                    g->player_pos.x++;
                    character = mvinch(g->player_pos.y, g->player_pos.x+1);
                }
            }
            break;
    }
    return 0;
}

void ShowHealth(Game *g) {
    mvprintw(0, COLS-2*g->MAX_health-10, "                              ");
    mvprintw(0, COLS-2*g->MAX_health-10, "Health: ");
    mvprintw(0, COLS-2*g->MAX_health-2, "[");
    int count_hearts = g->players_health;
    attron(COLOR_PAIR(2));
    for(int i=0; i<count_hearts; i++) {
        mvprintw(0, COLS-2*g->MAX_health-1+2*i, "<3");
    }
    attroff(COLOR_PAIR(2));
    mvprintw(0, COLS-1, "]");

}

void Corridor(char direction, Pos door1, Pos door2) {
    if(direction == 'v') {
        int num = 1 + rand()%(door2.y-door1.y-1);
        int j=1;
        for(j; j<=num; j++) {
            mvprintw(door1.y+j,door1.x,"#");
        }
        j--;
        if(door1.x>door2.x) {
            for(int i=1; i<door1.x-door2.x; i++) {
                mvprintw(door1.y+j,door1.x-i,"#");
            }
        }
        else {
            for(int i=1; i<door2.x-door1.x; i++) {
                mvprintw(door1.y+j,door1.x+i,"#");
            }
        }
        for(j; j<door2.y-door1.y; j++) {
            mvprintw(door1.y+j,door2.x,"#");
        }
    }
    else if(direction == 'h') {
        int num = 1 + rand()%(door2.x-door1.x-1);
        int i=1;
        for(i; i<=num; i++) {
            mvprintw(door1.y,door1.x+i,"#");
        }
        i--;
        if(door1.y>door2.y) {
            for(int j=1; j<door1.y-door2.y; j++) {
                mvprintw(door1.y-j,door1.x+i,"#");
            }
        }
        else {
            for(int j=1; j<door2.y-door1.y; j++) {
                mvprintw(door1.y+j,door1.x+i,"#");
            }
        }
        for(i; i<door2.x-door1.x; i++) {
            mvprintw(door2.y,door1.x+i,"#");
        }
    }
  
}

void ShowScreen(char mode[], int **visited, char **screen) {
    if(strcmp(mode,"hidden") == 0) {
        for(int j=1; j<LINES; j++) {
            for(int i=0; i<COLS; i++) {
                if(visited[i][j]) {
                    if(screen[i][j] == '^') {
                        attron(COLOR_PAIR(2));
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(2));
                        continue;
                    }
                    else if(screen[i][j] == 'o') {
                        attron(COLOR_PAIR(4));
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(4));
                        continue;
                    }
                    else if(screen[i][j] == '$') {
                        attron(COLOR_PAIR(5));
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(5));
                        continue;
                    }
                    else if(screen[i][j] == 'f') {
                        attron(COLOR_PAIR(3)); attron(A_REVERSE);
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(3)); attroff(A_REVERSE);
                        continue;
                    }
                    mvprintw(j,i,"%c",screen[i][j]);
                }
                else {
                    mvprintw(j,i," ");
                }
            }
        }
    }
    else {
        for(int j=1; j<LINES; j++) {
            for(int i=0; i<COLS; i++) {
                if(screen[i][j] == '^') {
                    attron(COLOR_PAIR(2));
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(2));
                    continue;
                }
                else if(screen[i][j] == 'o') {
                    attron(COLOR_PAIR(4));
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(4));
                    continue;
                }
                else if(screen[i][j] == '$') {
                    attron(COLOR_PAIR(5));
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(5));
                    continue;
                }
                else if(screen[i][j] == 'f') {
                    attron(COLOR_PAIR(3)); attron(A_REVERSE);
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(3)); attroff(A_REVERSE);
                    continue;
                }
                mvprintw(j,i,"%c",screen[i][j]);
            }
        }
    }   
}

int check_room(Room *rooms, int i, int j) {
    for(int k=0; k<6; k++) {
        if(abs(i-rooms[k].room_pos.x) <= rooms[k].room_size_h && abs(j-rooms[k].room_pos.y) <= rooms[k].room_size_v) {
            return k;
        }
    }
}

void ShowMessage(char message[], int floor, int score, int gold) {
    mvprintw(0,0,"%s FLOOR:%d   SCORE:%d    GOLD:%d", message, floor, score, gold);
}

int Trap_Check(Room *rooms, int i, int j) {
    for(int k=0; k<6; k++) {
        for(int l=0; l<rooms[k].traps_count; l++) {
            if(i == rooms[k].traps[l].x && j == rooms[k].traps[l].y) {
                return 1;
            }
        }
    }
    return 0;
}

void EndGame(User *p, Game *g) {
    p->score += g->players_score;
    p->gold += g->players_gold;
    FILE *accounts = fopen("users.txt", "r");
    FILE *temp_file = fopen("temp.txt", "w");
    char line[50];
    while(fgets(line, 50, accounts)) {
        line[strcspn(line,"\n")] = 0;
        if(strcmp(line,p->username) == 0) {
            fprintf(temp_file, "%s\n", line);
            fgets(line, 50, accounts); line[strcspn(line,"\n")] = 0; fprintf(temp_file, "%s\n", line);
            fgets(line, 50, accounts); line[strcspn(line,"\n")] = 0; fprintf(temp_file, "%s\n", line);
            fgets(line, 50, accounts); fprintf(temp_file, "%d\n", p->score);
            fgets(line, 50, accounts); fprintf(temp_file, "%d\n", p->gold);
            fgets(line, 50, accounts); fprintf(temp_file, "%d\n", p->count_games);
        }
        else {
            fprintf(temp_file, "%s\n", line);
        }
    }
    fclose(accounts);
    fclose(temp_file);
    remove("users.txt");
    rename("temp.txt","users.txt");

    ExitScreen(p);
}

void ExitScreen(User *p) {
    clear();

    init_pair(1, COLOR_BLACK, COLOR_RED);
    attron(COLOR_PAIR(1));
    mvprintw(LINES/2,COLS/2-4,"GAME OVER!");
    attroff(COLOR_PAIR(1));

    mvprintw(LINES/2+2,COLS/2-20,"Press Q to EXIT or S to view the SCOREBOARD");

    int ch = getch();

    if(ch == 'q') {
        endwin();
        exit(0);
    }
    else if(ch == 's') {
        display_leaderboard(p);
    }
    ExitScreen(p);
}

void Save_Game(User *p, Game *g, char **screen) {
    if(fopen(txt_format(p->username),"r")) {
        remove(txt_format(p->username));
    }
}

void save_screen(User *p , Game *g) {
    clear();

    init_pair(1, COLOR_BLACK, COLOR_GREEN);
    attron(COLOR_PAIR(1));
    mvprintw(LINES/2,COLS/2-16,"GAME HAS BEEN SAVED SUCCESSFULLY!");
    attroff(COLOR_PAIR(1));
    mvprintw(LINES/2+3,COLS/2-10,"Press any key to EXIT");

    int ch = getch();
    pre_game_menu(p,g);
    exit(0);
}

void not_saved_screen() {
    clear();

    mvprintw(LINES/2,COLS/2-14,"THERE ARE NO SAVED GAMES :(");
    mvprintw(LINES/2+2,COLS/2-14,"Press any key to RETURN");

    int ch = getch();
}

void food_screen(Game *g) {
    clear();

    attron(COLOR_PAIR(3));
    mvprintw(0, 1, "FOOD MENU");
    attroff(COLOR_PAIR(3));

    mvprintw(0, 20, "Choose a food to consume it");    

    const char *foods[] = {"Ordinary FOODS", "Super FOODS", "Magical FOODS", "Rotten FOODS", "RETURN"};

    int choose = 0;
    while (1) {
        ShowHealth(g);
        mvprintw(10, 8, "                                           ");
        mvprintw(10, 8, "Hungriness: [");
        attron(COLOR_PAIR(2));
        for(int i=0; i<g->players_hungriness; i++) {
            mvprintw(10, 21+2*i, "**");
        }
        attroff(COLOR_PAIR(2));
        mvprintw(10, 41, "]");
        for (int i=0; i<5; i++) {
            if (i == choose)
                attron(A_REVERSE);
            if(i == 0) {
                mvprintw(12+i, 1, "%s (%d)", foods[i], g->players_ordinary_food);
            }
            else {
                mvprintw(12+i, 1, "%s", foods[i]);
            }
            if (i == choose)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP && choose != 0)
            choose--;
        else if (ch == KEY_DOWN && choose != 4)
            choose++;
        else if (ch == 10) {
            if(choose == 0 && g->players_ordinary_food != 0 && g->players_hungriness > 0) {
                g->players_ordinary_food -= 1;
                g->players_hungriness -= 1;
            }
            else if(choose == 0 && g->players_ordinary_food != 0 && g->players_hungriness == 0 && g->players_health != g->MAX_health) {
                g->players_ordinary_food -= 1;
                g->players_health += 1;
            }
            if(choose == 4) {
                break;
            }
        }

    }
    clear();
}