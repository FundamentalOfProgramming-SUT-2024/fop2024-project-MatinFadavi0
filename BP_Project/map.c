#include "libs.h"
#include "player.h"
#include "map.h"
#include "game_menu.h"
#include "funcs.h"

//////////////////////////////////////////////////////// Bar /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Bar /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Bar /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Bar /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Bar /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Bar /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Bar /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Bar /////////////////////////////////////////////////////////////////////////

char message[100] = "";

void MessageBar(Game *g, int floor, int score, int gold) {
    mvprintw(0,0,"%s FLOOR:%d   SCORE:%d    GOLD:%d                         ", message, floor, score, gold);
}

void HEALTH_BAR(Game *g) {
    mvprintw(0, COLS-2*g->MAX_health-10, "                              ");
    mvprintw(0, COLS-2*g->MAX_health-10, "    Hp  ");
    mvprintw(0, COLS-2*g->MAX_health-2, "|");
    int count_hearts = g->players_health;
    attron(COLOR_PAIR(2));
    int i=0;
    for(i; i<count_hearts; i++) {
        mvprintw(0, COLS-2*g->MAX_health-1+2*i, "\u2764");
    }
    attroff(COLOR_PAIR(2));
    int count_extra_hearts = g->players_extra_health;
    attron(COLOR_PAIR(3));
    int j = 0;
    for(i; j<count_extra_hearts; i++, j++) {
        mvprintw(0, COLS-2*g->MAX_health-1+2*i, "\u2764");
    }
    attroff(COLOR_PAIR(3));
    mvprintw(0, COLS-2, "|");

}

void Food_Bar(Game*g) {
        
        mvprintw(0, (COLS - 40) / 2 +4, "Hunger   |");
        for(int i = 0; i < g->players_hunger; i++) {
            mvprintw(0, COLS/ 2 -6 + 2 * i, "🍕");
        }
        mvprintw(0, COLS/ 2 + 14, "|");
}

void Show_Password(int password) {
    strcpy(message, "password: ");
    message[10] = (password/1000)+48; message[11] = (password/100 - (password/1000 * 10))+48; message[12] = ((password%100 - password%10)/10)+48; message[13] = (password%10)+48;
    message[14] = 0;
}

//////////////////////////////////////////////////////// In Game Tabs /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// In Game Tabs /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// In Game Tabs /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// In Game Tabs /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// In Game Tabs /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// In Game Tabs /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// In Game Tabs /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// In Game Tabs /////////////////////////////////////////////////////////////////////////



void FoodTab(Game *g) {
    clear();

    attron(COLOR_PAIR(4));
    attron(A_BOLD);
    mvprintw(2,3, "FOOD MENU");
    attroff(COLOR_PAIR(4));
    attroff(A_BOLD);
    const char *foods[] = {"FOODS", "RETURN"};

    int choose = 0;
    while (1) {
        HEALTH_BAR(g);
        mvprintw(10, (COLS - 40) / 2 +4, "Hunger   |");
        attron(COLOR_PAIR(2));
        for(int i = 0; i < g->players_hunger; i++) {
            mvprintw(10, COLS/ 2 -6 + 2 * i, "🍕");
        }
        attroff(COLOR_PAIR(2));
        mvprintw(10, COLS/ 2 + 14, "|");
        
        for (int i = 0; i < 2; i++) {
            if (i == choose)
                attron(A_REVERSE);
            if (i == 0) {
                mvprintw(12 + i, (COLS - 20) / 2, "%s  %d ", foods[i], g->players_ordinary_food);
            } else {
                mvprintw(12 + i, (COLS - 20) / 2, "%s", foods[i]);
            }
            if (i == choose)
                attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP && choose != 0)
            choose--;
        else if (ch == KEY_DOWN && choose != 1)
            choose++;
        else if (ch == 10) {
            if (choose == 0 && g->players_ordinary_food != 0 && g->players_hunger < 10) {
                g->players_ordinary_food -= 1;
                g->players_hunger += 1;
            } else if (choose == 0 && g->players_ordinary_food != 0 && g->players_hunger == 10 && g->players_health != g->MAX_health) {
                g->players_ordinary_food -= 1;
                g->players_health += 1;
            }
            if (choose == 1) {
                break;
            }
        }
        else if ( ch == 'f') break;
    }
    clear();
}


void WeaponTab(Game *g) {
    clear();
    
    const char *weapons[] = {"Mace", "Sword                ==>", "Dagger                ~", "Magic Wand            /", "Normal Arrow          }", "RETURN"};
    int weapon_counts[] = {g->players_mace, g->players_sword, g->players_dagger, g->players_magic_wand, g->players_arrow};
    int damages[] = {5, 10, 12, 15, 5};
    int ranges[] = {0, 0, 5, 10, 5};
    char message[100] = "Select a weapon";
    int choose = 0;

    while (1) {
        clear();
        attron(COLOR_PAIR(1));
        attron(A_BOLD);
        mvprintw(2, 3, "WEAPONS MENU");
        attroff(COLOR_PAIR(1));
        attroff(A_BOLD);
    const char *ascii_art[] = {
    "                        ____________",
    "                      .~      ,   . ~.",
    "                     /                \\",
    "                    /      /~\\/~\\   ,  \\",
    "                   |   .   \\    /   '   |",
    "                   |         \\/         |",
    "          XX       |  /~~\\        /~~\\  |       XX",
    "        XX  X      | |  o  \\    /  o  | |      X  XX",
    "      XX     X     |  \\____/    \\____/  |     X     XX",
    " XXXXX     XX      \\         /\\        ,/      XX     XXXXX",
    "X        XX%;;@      \\      /  \\     ,/      @%%;XX        X",
    "X       X  @%%;;@     |           '  |     @%%;;@  X       X",
    "X      X     @%%;;@   |. ` ; ; ; ;  ,|   @%%;;@     X      X",
    " X    X        @%%;;@                  @%%;;@        X    X",
    "  X   X          @%%;;@              @%%;;@          X   X",
    "   X  X            @%%;;@          @%%;;@            X  X",
    "    XX X             @%%;;@      @%%;;@             X XX",
    "      XXX              @%%;;@  @%%;;@              XXX",
    "                         @%%;;%%;;@",
    "                           @%%;;@",
    "                         @%%;;@..@@",
    "                          @@@  @@@"
    };
    for (int i = 0; i < 22; i++) {
        attron(COLOR_PAIR(2));
        mvprintw(5 + i, 2, "%s", ascii_art[i]);
    }
        attroff(COLOR_PAIR(2));

        mvprintw(6, (COLS - 20) / 2, "Melee Weapons");
        mvprintw(14, (COLS - 20) / 2, "Ranged Weapons");
        
        mvprintw(2, COLS - 20, "Ammo | Damage");
        for (int i = 0; i < 5; i++) {
            mvprintw(3 + i, COLS - 20, "%5d | %6d", weapon_counts[i], damages[i]);
        }
        
        for (int i = 0; i < 6; i++) {
            if (i == choose) attron(A_REVERSE);
            if (i < 2) {
                if ( i == 1) attron(COLOR_PAIR(6));

                mvprintw(8 + i * 2, (COLS - 30) / 2, "%s", weapons[i]);
            } else if (i < 5) {
                attroff(COLOR_PAIR(6));
                if (i == 3) attron(COLOR_PAIR(5));
                else if ( i == 2 && i == 4) attroff(COLOR_PAIR(5));
                else if (i == 4) attron(COLOR_PAIR(7));
                else attroff(COLOR_PAIR(7));
                mvprintw(16 + (i - 2) * 2, (COLS - 30) / 2, "%s", weapons[i]);
            } else {
                attroff(COLOR_PAIR(7));
                mvprintw(22, (COLS - 10) / 2 - 1, "%s", weapons[i]);
            }
            if (i == choose) attroff(A_REVERSE);
        }

        mvprintw(25, (COLS - strlen(message)) / 2 - 2, "%s", message);
        
        int ch = getch();
        if (ch == 'w') {
            if (g->players_weapon != -1) {
                g->players_weapon = -1;
                strcpy(message, "Weapon unequipped. Select a new one.");
            } else {
                strcpy(message, "You have no weapon equipped.");
            }
        } else if (ch == KEY_UP && choose > 0) {
            choose--;
        } else if (ch == KEY_DOWN && choose < 5) {
            choose++;
        } else if (ch == 10) {  // Enter key
            if (choose == 5) {
                break; // Exit menu when RETURN is selected
            }
            if (g->players_weapon == -1) {
                if (weapon_counts[choose] > 0) {
                    g->players_weapon = choose;
                    break;
                } else {
                    sprintf(message, "You have no %s", weapons[choose]);
                }
            } else {
                strcpy(message, "Unequip your current weapon first using 'W'");
            }
        }
        else if( ch == 'i') break;
    }
    clear();
}


void SpellTab(Game *g) {
    clear();

    attron(COLOR_PAIR(5));
    attron(A_BOLD);
    mvprintw(2, 2, "SPELLS");
    attroff(COLOR_PAIR(5));
    attroff(A_BOLD);
    
    const char *spells[] = {"Health Potion", "Speed Potion", "Damage Potion", "RETURN"};

    int choose = 0;
    while (1) {
        clear();
        attron(COLOR_PAIR(5));
        attron(A_BOLD);
        mvprintw(2,2, "SPELLS");
        attroff(COLOR_PAIR(5));
        attroff(A_BOLD);
        
        for (int i = 0; i < 4; i++) {
            int row = (LINES - 4) / 2 + i * 2; 
            int col = (COLS - 20) / 2; 
            
            if (i == choose) attron(A_REVERSE);
            
            if (i == 0) {
                attron(COLOR_PAIR(3));
                mvprintw(row, col, "%s  %d", spells[i], g->players_health_potion);
                attroff(COLOR_PAIR(3));
            } else if (i == 1) {
                attron(COLOR_PAIR(6));
                mvprintw(row, col, "%s   %d", spells[i], g->players_speed_potion);
                attroff(COLOR_PAIR(6));
            } else if (i == 2) {
                attron(COLOR_PAIR(2));
                mvprintw(row, col, "%s  %d", spells[i], g->players_damage_potion);
                attroff(COLOR_PAIR(2));
            } else {
                mvprintw(row, col, "%s", spells[i]);
            }
            
            if (i == choose) attroff(A_REVERSE);
        }

        int ch = getch();
        if (ch == KEY_UP && choose > 0) choose--;
        else if (ch == KEY_DOWN && choose < 3) choose++;
        else if (ch == 10) {
            if (choose == 0 && g->players_health_potion > 0 && g->players_health != 10) {
                g->players_health_step = g->players_steps;
                g->players_extra_health++;
                g->players_health_potion--;
                break;
            } else if (choose == 1 && g->players_speed_potion > 0) {
                g->players_speed_step = g->players_steps;
                g->players_speed = 2;
                g->players_speed_potion--;
                break;
            } else if (choose == 2 && g->players_damage_potion > 0) {
                g->players_damage_step = g->players_steps;
                g->players_damage_rate = 2;
                g->players_damage_potion--;
                break;
            } else if (choose == 3) {
                break;
            }
        }
              else if( ch = 'p') return;
    }
    clear();
}

void show_key_guide() {
    initscr();            
    noecho();              
    curs_set(0);           
    keypad(stdscr, TRUE); 

    int mid_y = LINES / 2 - 8;
    int mid_x = COLS / 2 - 10;

    clear();
    mvprintw(mid_y - 2, COLS / 2 - 6, "KEY GUIDE");

    mvprintw(mid_y,     mid_x, "M    Show Full Map");
    mvprintw(mid_y + 1, mid_x, "I    Weapon Tab");
    mvprintw(mid_y + 2, mid_x, "P    Spell Tab");
    mvprintw(mid_y + 3, mid_x, "F    Food Tab");
    mvprintw(mid_y + 4, mid_x, "K    Key Guide Tab");
    mvprintw(mid_y + 5, mid_x, "8    Move Up");
    mvprintw(mid_y + 6, mid_x, "2    Move Down");
    mvprintw(mid_y + 7, mid_x, "4    Move Left");
    mvprintw(mid_y + 8, mid_x, "6    Move Right");
    mvprintw(mid_y + 9, mid_x, "1    Move Down Left");
    mvprintw(mid_y + 10, mid_x, "3    Move Down Right");
    mvprintw(mid_y + 11, mid_x, "7    Move Up Left");
    mvprintw(mid_y + 12, mid_x, "9    Move Up Right");
    mvprintw(mid_y + 13, mid_x, "Q    Pause Menu");
    mvprintw(mid_y + 14, mid_x -2, "Space  Shoot/Attack");
    mvprintw(mid_y + 15, mid_x, "W    Unequip Weapon");
    mvprintw(mid_y + 16, mid_x, "Z    Equip Mace");
    mvprintw(mid_y + 17, mid_x, "X    Equip Sword");
    mvprintw(mid_y + 18, mid_x, "C    Equip Dagger");
    mvprintw(mid_y + 19, mid_x, "V    Equip Magic Wand");
    mvprintw(mid_y + 20, mid_x, "B    Equip Normal Arrow");
    
    mvprintw(mid_y + 22, COLS / 2 - 12, "   Press 'K' to return");
    refresh();

    while(1){
       int  ch = getch();
        if ( ch == 'k') break;
    }
}

//////////////////////////////////////////////////////// Map Generation /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Map Generation /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Map Generation /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Map Generation /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Map Generation /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Map Generation /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Map Generation /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Map Generation /////////////////////////////////////////////////////////////////////////


void FloorGenerator(User *p, Game *g) {
    srand(time(NULL));
    clear();

    for(int k=0; k<6; k++) {
        g->rooms[k].type = 0;
        g->rooms[k].locked = 0;
    }

    if(g->floor_number == 1) {
        strcpy(message, "Wellcome to the 1st floor!");
        map(g,6);
        for(int k=0; k<5; k++) {
            g->rooms[k].monsters_count = rand() % 2;
            for(int i=0; i<g->rooms[k].monsters_count; i++) {
                g->rooms[k].monsters[i].type = 1+rand()%4; g->rooms[k].monsters[i].alive = 1; g->rooms[k].monsters[i].on = 0; g->rooms[k].monsters[i].room = k;
                if(g->rooms[k].monsters[i].type == 1) {
                    g->rooms[k].monsters[i].radius = 1; g->rooms[k].monsters[i].health = 5; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 0;}
                else if(g->rooms[k].monsters[i].type == 2) {
                    g->rooms[k].monsters[i].radius = 3; g->rooms[k].monsters[i].health = 10; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 3;}
                else if(g->rooms[k].monsters[i].type == 3) {
                    g->rooms[k].monsters[i].radius = 4; g->rooms[k].monsters[i].health = 15; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 5;}
                else if(g->rooms[k].monsters[i].type == 4) {
                    g->rooms[k].monsters[i].radius = 5; g->rooms[k].monsters[i].health = 20; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 100;}
            }
        }
    }
    else if(g->floor_number == 2) {
        strcpy(message, "You're now in the 2nd floor!");
        map(g,RoomChecking(g->rooms,g->player_pos.x,g->player_pos.y));
        for(int k=0; k<6; k++) {
            g->rooms[k].monsters_count = 1 + rand() % 2;
            for(int i=0; i<g->rooms[k].monsters_count; i++) {
                g->rooms[k].monsters[i].type = 1+rand()%4; g->rooms[k].monsters[i].alive = 1; g->rooms[k].monsters[i].on = 0; g->rooms[k].monsters[i].room = k;
                if(g->rooms[k].monsters[i].type == 1) {g->rooms[k].monsters[i].radius = 1; g->rooms[k].monsters[i].health = 5; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 0;}
                else if(g->rooms[k].monsters[i].type == 2) {g->rooms[k].monsters[i].radius = 3; g->rooms[k].monsters[i].health = 10; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 3;}
                else if(g->rooms[k].monsters[i].type == 3) {g->rooms[k].monsters[i].radius = 4; g->rooms[k].monsters[i].health = 15; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 5;}
                else if(g->rooms[k].monsters[i].type == 4) {g->rooms[k].monsters[i].radius = 5; g->rooms[k].monsters[i].health = 20; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 100;}
            }
        }
    }
    else if(g->floor_number == 3) {
        strcpy(message, "You're now in the 3rd floor!");
        map(g,RoomChecking(g->rooms,g->player_pos.x,g->player_pos.y));
        for(int k=0; k<6; k++) {
            g->rooms[k].monsters_count = 1+rand()%2;
            for(int i=0; i<g->rooms[k].monsters_count; i++) {
                g->rooms[k].monsters[i].type = 1+rand()%4; g->rooms[k].monsters[i].alive = 1; g->rooms[k].monsters[i].on = 0; g->rooms[k].monsters[i].room = k;
                if(g->rooms[k].monsters[i].type == 1) {g->rooms[k].monsters[i].radius = 1; g->rooms[k].monsters[i].health = 5; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 0;}
                else if(g->rooms[k].monsters[i].type == 2) {g->rooms[k].monsters[i].radius = 3; g->rooms[k].monsters[i].health = 10; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 3;}
                else if(g->rooms[k].monsters[i].type == 3) {g->rooms[k].monsters[i].radius = 4; g->rooms[k].monsters[i].health = 15; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 5;}
                else if(g->rooms[k].monsters[i].type == 4) {g->rooms[k].monsters[i].radius = 5; g->rooms[k].monsters[i].health = 20; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 100;}
            }
        }
    }
    else if(g->floor_number == 4) {
        strcpy(message, "You're now in the last floor!");
        map(g,RoomChecking(g->rooms,g->player_pos.x,g->player_pos.y));
        int count_treasure_rooms = 0; double avg_size = 0;
        for(int k=0; k<6; k++) { avg_size += (g->rooms[k].room_size_h+g->rooms[k].room_size_v); }
        avg_size /= 6;

        for(int k=0; k<6; k++) {
            if(g->rooms[k].type == 1) {
                g->rooms[k].monsters_count = 10;
            }
            else {
                g->rooms[k].monsters_count = 2+rand()%2;
                for(int i=0; i<g->rooms[k].monsters_count; i++) {
                    g->rooms[k].monsters[i].type = 1+rand()%5; g->rooms[k].monsters[i].alive = 1; g->rooms[k].monsters[i].on = 0; g->rooms[k].monsters[i].room = k;
                    if(g->rooms[k].monsters[i].type == 1) {g->rooms[k].monsters[i].radius = 1; g->rooms[k].monsters[i].health = 5; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 0;}
                    else if(g->rooms[k].monsters[i].type == 2) {g->rooms[k].monsters[i].radius = 3; g->rooms[k].monsters[i].health = 10; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 3;}
                    else if(g->rooms[k].monsters[i].type == 3) {g->rooms[k].monsters[i].radius = 4; g->rooms[k].monsters[i].health = 15; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 5;}
                    else if(g->rooms[k].monsters[i].type == 4) {g->rooms[k].monsters[i].radius = 5; g->rooms[k].monsters[i].health = 20; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 100;}
                    else if(g->rooms[k].monsters[i].type == 5) {g->rooms[k].monsters[i].radius = 10; g->rooms[k].monsters[i].health = 30; g->rooms[k].monsters[i].damage = 2; g->rooms[k].monsters[i].haunt = 8;}
                }
            }
        }
    }

    for(int k=0; k<6; k++) {
            g->rooms[k].pillar_seed.x = g->rooms[k].room_pos.x + power(-1,rand()%2)*(rand()%(g->rooms[k].room_size_h-3));
            g->rooms[k].pillar_seed.y = g->rooms[k].room_pos.y + power(-1,rand()%2)*(rand()%(g->rooms[k].room_size_v-3));
            g->rooms[k].traps_count = 1 + rand()%3;
            g->rooms[k].gold = 1 + rand()%4;
            g->rooms[k].dark_gold = rand()%2 *rand()%2;
            g->rooms[k].ordinary_food = 1 + rand()%2;
            g->rooms[k].potion = rand()%2;
    }

    for(int j=1; j<LINES; j++) {
        for(int i=0; i<COLS; i++) {
            for(int k=0; k<6; k++) {
                if(abs(j-g->rooms[k].room_pos.y) == g->rooms[k].room_size_v && abs(i-g->rooms[k].room_pos.x) <= g->rooms[k].room_size_h) {
                    attron(COLOR_PAIR(7));
                    mvprintw(j, i, "-");
                    attroff(COLOR_PAIR(7));
                }
                else if(abs(i-g->rooms[k].room_pos.x) == g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v) {
                    attron(COLOR_PAIR(7));
                    mvprintw(j, i, "|");
                    attroff(COLOR_PAIR(7));
                }
                else if(abs(i-g->rooms[k].room_pos.x) < g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v) {
                    mvprintw(j, i, ".");
                }
            }
        }
    }

    int k_stair_1; do { k_stair_1 = rand()%6; } while(k_stair_1 == RoomChecking(g->rooms,g->player_pos.x,g->player_pos.y)); int k_stair_2 = rand()%6; g->k_lock = rand()%6; g->rooms[g->k_lock].locked = 1; g->rooms[g->k_lock].password = password();  int k_secret_door_1 = rand()%6; int k_secret_door_2 = rand()%6; g->secret_doors_count = 0;
    int k_dagger = rand()%6; int k_magic_wand = rand()%6; int k_arrow = rand()%6; int k_sword = rand()%6; 
    for(int k=0; k<6; k++) {
        if(g->rooms[k].locked == 1) {
            if(rand()%2) {
                mvprintw(g->rooms[k].room_pos.y+g->rooms[k].room_size_v-1, g->rooms[k].room_pos.x+g->rooms[k].room_size_h-1, "&");
            }
            else {
                mvprintw(g->rooms[k].room_pos.y-g->rooms[k].room_size_v+1, g->rooms[k].room_pos.x-g->rooms[k].room_size_h+1, "&");
            }
        }
        int d1=1; int d2=1; int stairs = 0; int trap = 0; int gold = 0; int dark_gold = 0; int ordinary_food = 0; int secret_door = 0; int potion = 0;
        int dagger = 0; int magic_wand = 0; int arrow = 0; int sword = 0; int monster = 0;int treasure =0;
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
                        if(k == g->k_lock) {
                            mvprintw(new_j, i, "@"); d2 = 0;
                        }
                        else {
                            mvprintw(new_j, i, "+"); d2 = 0;
                        }
                        g->rooms[k].doors[0].x = i; g->rooms[k].doors[0].y = new_j;
                    }
                }
                else if(k == 1) {
                    if(g->rooms[k].room_pos.y-j == g->rooms[k].room_size_v && abs(i-g->rooms[k].room_pos.x) <= g->rooms[k].room_size_h && d1) {
                        int new_i = i+rand()%g->rooms[k].room_size_h;
                        if(k == g->k_lock) {
                            mvprintw(j, new_i, "@"); d1 = 0;
                        }
                        else {
                            mvprintw(j, new_i, "+"); d1 = 0;
                        }
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
                        if(k == g->k_lock) {
                            mvprintw(new_j, i, "@"); d2 = 0;
                        }
                        else {
                            mvprintw(new_j, i, "+"); d2 = 0;
                        }
                        g->rooms[k].doors[1].x = i; g->rooms[k].doors[1].y = new_j;
                    }
                }
                else if(k == 4) {
                    if(j-g->rooms[k].room_pos.y == g->rooms[k].room_size_v && abs(i-g->rooms[k].room_pos.x) <= g->rooms[k].room_size_h && d1) {
                        int new_i = i+rand()%g->rooms[k].room_size_h;
                        if(k == g->k_lock) {
                            mvprintw(j, new_i, "@"); d1 = 0;
                        }
                        else {
                            mvprintw(j, new_i, "+"); d1 = 0;
                        }
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
                        if (k == g->k_lock) {
                            mvprintw(new_j, i, "@"); d2 = 0;
                        }
                        else {
                            mvprintw(new_j, i, "+"); d2 = 0;
                        }
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
                    if(g->rooms[k].type == 1) {
                        if(rand()%10 == 1 && (mvinch(j,i) & A_CHARTEXT) == '.' && dark_gold<g->rooms[k].dark_gold) {
                            mvprintw(j,i,"$");
                            dark_gold++;
                        }
                        if(rand()%10 == 1 && (mvinch(j,i) & A_CHARTEXT) == '.' && trap<g->rooms[k].traps_count) {
                            g->rooms[k].traps[trap].x = i; g->rooms[k].traps[trap].y = j;
                            trap++;
                        }
                    }
                    else {
                        if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT)  == '.' && dark_gold<g->rooms[k].dark_gold) {
                            mvprintw(j,i,"$");
                            dark_gold++;
                        }
                        if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && trap<g->rooms[k].traps_count) {
                            g->rooms[k].traps[trap].x = i; g->rooms[k].traps[trap].y = j;
                            trap++;
                        }
                    }
                    
                    if(g->floor_number != 4) {
                        if((k == k_stair_1 || k == k_stair_2) && k!=RoomChecking(g->rooms,g->player_pos.x,g->player_pos.y)) {
                            if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && stairs<1) {
                                mvprintw(j, i, "<");
                                stairs++;
                            }
                        }
                        if(k == k_secret_door_1 || k == k_secret_door_2) {
                            if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && secret_door<1) {
                                g->secret_doors[g->secret_doors_count].x = i; g->secret_doors[g->secret_doors_count].y = j;
                                secret_door++;
                                g->secret_doors_count++;
                            }
                        }  
                    }
                    else {
                        if(k == k_sword) {
                            if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && sword<1) {
                                attron(COLOR_PAIR(6));mvprintw(j,i,"="); mvprintw(j,i+1,"="); mvprintw(j,i+2,">"); attroff(COLOR_PAIR(6));
                                sword++;
                            }
                            else if((mvinch(j,i) & A_CHARTEXT) == '.' && treasure<1) {
                                attron(COLOR_PAIR(6));attron(A_REVERSE);mvprintw(j,i,"T");attroff(COLOR_PAIR(6));attroff(A_REVERSE);
                                treasure++;
                            }
                        }
                    }
                    if(k == k_dagger) {
                        if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && !CheckTrap(g->rooms,i,j) && !CheckEnchantDoor(g->secret_doors_count,g->secret_doors,i,j) && dagger<1) {
                            mvprintw(j,i,"~");
                            g->weapons[0].x = i;  g->weapons[0].y = j;
                            dagger++;
                        }
                    }
                    if(k == k_magic_wand) {
                        if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && !CheckTrap(g->rooms,i,j) && !CheckEnchantDoor(g->secret_doors_count,g->secret_doors,i,j) && magic_wand<1) {
                            attron(COLOR_PAIR(5)); mvprintw(j,i,"/"); attroff(COLOR_PAIR(5));
                            g->weapons[1].x = i;  g->weapons[1].y = j;
                            magic_wand++;
                        }
                    }
                    if(k == k_arrow) {
                        if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && !CheckTrap(g->rooms,i,j) && !CheckEnchantDoor(g->secret_doors_count,g->secret_doors,i,j) && arrow<1) {
                            attron(COLOR_PAIR(7)); mvprintw(j,i,"}"); attroff(COLOR_PAIR(7));
                            g->weapons[2].x = i;  g->weapons[2].y = j;
                            arrow++;
                        }
                    }
                    if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && !CheckTrap(g->rooms,i,j) && !CheckEnchantDoor(g->secret_doors_count,g->secret_doors,i,j) && gold<g->rooms[k].gold) {
                        mvprintw(j,i,"*");
                        gold++;
                    }                    
                    if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && !CheckTrap(g->rooms,i,j) && !CheckEnchantDoor(g->secret_doors_count,g->secret_doors,i,j) && ordinary_food<g->rooms[k].ordinary_food) {
                        mvprintw(j,i,"f");
                        ordinary_food++;
                    }
                    if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && monster<g->rooms[k].monsters_count) {
                        g->rooms[k].monsters[monster].position.x = i; g->rooms[k].monsters[monster].position.y = j;
                        monster++;
                    }
                    if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && !CheckTrap(g->rooms,i,j) && !CheckEnchantDoor(g->secret_doors_count,g->secret_doors,i,j) && potion<g->rooms[k].potion) {
                        int num = rand()%3;
                        if(num%3 == 0) {
                            attron(COLOR_PAIR(3));attron(A_REVERSE);
                            mvprintw(j, i,"!");
                            attroff(COLOR_PAIR(3));attroff(A_REVERSE);
                        }
                        else if(num%3 == 1) {
                            attron(COLOR_PAIR(1));
                            mvprintw(j, i,"o");
                            attroff(COLOR_PAIR(1));
                        }
                        else if(num%3 == 2) {
                            attron(COLOR_PAIR(2));
                            mvprintw(j, i,"%");
                            attroff(COLOR_PAIR(2));
                        }
                        potion++;
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

    chtype **screen = (chtype **) malloc(COLS*sizeof(chtype*));
    for(int i=0; i<COLS; i++) {
        *(screen+i) = (chtype *) malloc(LINES*sizeof(chtype));
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
    int k = RoomChecking(g->rooms,g->player_pos.x,g->player_pos.y);
    for(int i=g->rooms[k].room_pos.x-g->rooms[k].room_size_h-2; i<=g->rooms[k].room_pos.x+g->rooms[k].room_size_h+2; i++) {
        for(int j=g->rooms[k].room_pos.y-g->rooms[k].room_size_v-2; j<=g->rooms[k].room_pos.y+g->rooms[k].room_size_v+2; j++) {
            if((k == 0 && i<g->rooms[k].room_pos.x-g->rooms[k].room_size_h) || (k == 0 && j<g->rooms[k].room_pos.y-g->rooms[k].room_size_v)) { continue; }
            if((k == 1 && i<g->rooms[k].room_pos.x-g->rooms[k].room_size_h) || (k == 1 && j>g->rooms[k].room_pos.y+g->rooms[k].room_size_v)) { continue; }
            if((k == 2 && j<g->rooms[k].room_pos.y-g->rooms[k].room_size_v) || (k == 3 && j>g->rooms[k].room_pos.y+g->rooms[k].room_size_v)) { continue; }
            if((k == 4 && i>g->rooms[k].room_pos.x+g->rooms[k].room_size_h) || (k == 4 && j<g->rooms[k].room_pos.y-g->rooms[k].room_size_v)) { continue; }
            if((k == 5 && i>g->rooms[k].room_pos.x+g->rooms[k].room_size_h) || (k == 5 && j>g->rooms[k].room_pos.y+g->rooms[k].room_size_v)) { continue; }
            
            visited[i][j] = 1;
        }
    }

    int display_whole = 0;
    ShowScreen(g,"hidden",visited,screen);
    while(1) {
        MessageBar(g, g->floor_number, g->players_score, g->players_gold);
        HEALTH_BAR(g);
        Food_Bar(g);
        time_t current_time = time(NULL);
        double elapsed_time = difftime(current_time, g->start_time);
        double password_time = difftime(current_time, g->password_start_time);

        if(elapsed_time > 30) {
            if(g->players_hunger == 10 && g->players_health < 9) {
                g->players_health += 1;
            }
            if(g->players_hunger > 0 ) {
                g->players_hunger -= g->hungriness_rate;
            }
            g->start_time = time(NULL);
            if(g->players_hunger <= 5) {
                g->players_health -= (10 - g->players_hunger)/5;
            }
        }
        if(g->players_steps - g->players_speed_step >= 15) {g->players_speed = 1; g->players_speed_step = -15;}
        if(g->players_steps - g->players_health_step >= 20) {g->players_extra_health = 0; g->players_health_step = -20;}
        if(g->players_steps - g->players_damage_step >= 5) {g->players_damage_rate = 1; g->players_damage_step = -5;}
        if(g->players_health + g->players_extra_health <= 0) {
            terminate_game(0, p, g);
        }
        if(display_whole%2 == 1) {
            ShowScreen(g,"view",visited,screen);
        }
        else {
            ShowScreen(g,"hidden",visited,screen);
        }
        attron(COLOR_PAIR(g->player_color));
        mvprintw(g->player_pos.y, g->player_pos.x, "@");
        attroff(COLOR_PAIR(g->player_color));

        int ch = getch();
        if(ch == 'q') {
            if ( !p->guest ){
            Pause_Screen(p,g,screen,visited);
            }
            else{
                pre_game_menu(p,g);
            }
        }
        else if(ch == 'm') {
            display_whole++;
            continue;
        }
        else if(ch == 'f') {
            FoodTab(g);
        }
        else if(ch == 'i') {
            WeaponTab(g);
        }
        else if(ch == 'p') {
            SpellTab(g);
        }
        else if ( ch == 'k'){
            show_key_guide();
        }

        handle_monsters(g, screen);
        Monsters_Movement(g, screen);

        Active_Weapons(g, ch, screen);

        HEALTH_BAR(g);

        for(int i = g->player_pos.x-1; i <= g->player_pos.x+1; i++) {
            for(int j = g->player_pos.y-1; j <= g->player_pos.y+1; j++) {
                if(ch == 's') {
                    if(CheckTrap(g->rooms,i,j)) {
                        screen[i][j] = '^';
                    }
                }
                if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,i,j)) {
                    screen[i][j] = '?';
                }
            }
        }
        switch(Movement(screen, visited, ch, g,p)) {
            case 1:
                g->floor_number += 1;
                FloorGenerator(p,g);
                break;
            case 2:
                if(rand()%10 == 9) {
                    g->rooms[g->k_lock].password = password();
                }
                Show_Password(g->rooms[g->k_lock].password);
                break;
            case 3:
                password_screen(g);
                break;
            case 4:
                g->rooms[0].room_pos.x = COLS/2; g->rooms[0].room_pos.y = LINES/2; g->rooms[0].room_size_v = 12; g->rooms[0].room_size_h = 35;
                for(int i=0; i<6; i++) {g->rooms[i].monsters_count = 0;}
                break;
            case 5:
                Pos saved_pos; saved_pos.x = g->player_pos.x; saved_pos.y = g->player_pos.y; int saved_secret_doors_count = g->secret_doors_count; Pos saved_first_door; saved_first_door.x = g->secret_doors[0].x; saved_first_door.y = g->secret_doors[0].y;
                int monsters_count[6]; for(int i=0; i<6; i++) { monsters_count[i] = g->rooms[i].monsters_count; g->rooms[i].monsters_count = 0;}
                g->enchant_start_time = time(NULL);
                EnchantRoom(p, g);
                g->player_pos.x = saved_pos.x; g->player_pos.y = saved_pos.y; g->secret_doors_count = saved_secret_doors_count; g->secret_doors[0].x = saved_first_door.x; g->secret_doors[0].y = saved_first_door.y;
                for(int i=0; i<6; i++) { g->rooms[i].monsters_count = monsters_count[i];}
                break;
            case 6:
                saved_pos.x = g->player_pos.x; saved_pos.y = g->player_pos.y; Pos room_0_pos; room_0_pos.x = g->rooms[0].room_pos.x; room_0_pos.y = g->rooms[0].room_pos.y;
                int size_v = g->rooms[0].room_size_v; int size_h = g->rooms[0].room_size_h;
                g->rooms[0].room_pos.x = COLS/2; g->rooms[0].room_pos.y = LINES/2; g->rooms[0].room_size_v = 12; g->rooms[0].room_size_h = 35;
                for(int i=0; i<6; i++) { monsters_count[i] = g->rooms[i].monsters_count; g->rooms[i].monsters_count = 0;}
                g->player_pos.x = saved_pos.x; g->player_pos.y = saved_pos.y;
                g->rooms[0].room_pos.x = room_0_pos.x; g->rooms[0].room_pos.y = room_0_pos.y; g->rooms[0].room_size_v = size_v; g->rooms[0].room_size_h = size_h;
                for(int i=0; i<6; i++) { g->rooms[i].monsters_count = monsters_count[i];}
                break;
            case 7:
                 saved_pos; saved_pos.x = g->player_pos.x; saved_pos.y = g->player_pos.y;
                 monsters_count[6]; for(int i=0; i<6; i++) { monsters_count[i] = g->rooms[i].monsters_count; g->rooms[i].monsters_count = 0;}
                g->treasure_start_time = time(NULL);
                TreasureRoom(p, g);
                g->player_pos.x = saved_pos.x; g->player_pos.y = saved_pos.y;
                for(int i=0; i<6; i++) { g->rooms[i].monsters_count = monsters_count[i];}
                break;                             
        }
    }
}

void Corridor(char direction, Pos door1, Pos door2) {
    attron(COLOR_PAIR(7));
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
      attroff(COLOR_PAIR(7));
}

void map(Game *g, int init_room) {
    int index = 0;
    for(int i=0; i<3; i++) {
        for(int j=0; j<2; j++) {
            if(index!=init_room) {
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
    if(init_room == 6) {
        g->player_pos.x = g->rooms[5].room_pos.x;
        g->player_pos.y = g->rooms[5].room_pos.y;
    }

}

int Movement(chtype **screen, int **visited, int ch, Game *g , User *p) {
    int count_dots = 0;
    chtype character = mvinch(g->player_pos.y-1, g->player_pos.x) & A_CHARTEXT; if(character == '.') {count_dots++;}
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
            for(int i=g->rooms[0].room_pos.x-g->rooms[0].room_size_h; i<=g->rooms[0].room_pos.x+g->rooms[0].room_size_h+2; i++) {
                for(int j=g->rooms[0].room_pos.y-g->rooms[0].room_size_v; j<=g->rooms[0].room_pos.y+g->rooms[0].room_size_v+2; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[1].room_pos.x) <= g->rooms[1].room_size_h+2 && abs(g->player_pos.y-g->rooms[1].room_pos.y) <= g->rooms[1].room_size_v+2) {
            for(int i=g->rooms[1].room_pos.x-g->rooms[1].room_size_h; i<=g->rooms[1].room_pos.x+g->rooms[1].room_size_h+2; i++) {
                for(int j=g->rooms[1].room_pos.y-g->rooms[1].room_size_v-2; j<=g->rooms[1].room_pos.y+g->rooms[1].room_size_v; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[2].room_pos.x) <= g->rooms[2].room_size_h+2 && abs(g->player_pos.y-g->rooms[2].room_pos.y) <= g->rooms[2].room_size_v+2) {
            for(int i=g->rooms[2].room_pos.x-g->rooms[2].room_size_h-2; i<=g->rooms[2].room_pos.x+g->rooms[2].room_size_h+2; i++) {
                for(int j=g->rooms[2].room_pos.y-g->rooms[2].room_size_v; j<=g->rooms[2].room_pos.y+g->rooms[2].room_size_v+2; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[3].room_pos.x) <= g->rooms[3].room_size_h+2 && abs(g->player_pos.y-g->rooms[3].room_pos.y) <= g->rooms[3].room_size_v+2) {
            for(int i=g->rooms[3].room_pos.x-g->rooms[3].room_size_h-2; i<=g->rooms[3].room_pos.x+g->rooms[3].room_size_h+2; i++) {
                for(int j=g->rooms[3].room_pos.y-g->rooms[3].room_size_v-2; j<=g->rooms[3].room_pos.y+g->rooms[3].room_size_v; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[4].room_pos.x) <= g->rooms[4].room_size_h+2 && abs(g->player_pos.y-g->rooms[4].room_pos.y) <= g->rooms[4].room_size_v+2) {
            for(int i=g->rooms[4].room_pos.x-g->rooms[4].room_size_h-2; i<=g->rooms[4].room_pos.x+g->rooms[4].room_size_h; i++) {
                for(int j=g->rooms[4].room_pos.y-g->rooms[4].room_size_v; j<=g->rooms[4].room_pos.y+g->rooms[4].room_size_v+2; j++) {
                    visited[i][j] = 1;
                }
            }
        }
        else if(abs(g->player_pos.x-g->rooms[5].room_pos.x) <= g->rooms[5].room_size_h+2 && abs(g->player_pos.y-g->rooms[5].room_pos.y) <= g->rooms[5].room_size_v+2) {
            for(int i=g->rooms[5].room_pos.x-g->rooms[5].room_size_h-2; i<=g->rooms[5].room_pos.x+g->rooms[5].room_size_h; i++) {
                for(int j=g->rooms[5].room_pos.y-g->rooms[5].room_size_v-2; j<=g->rooms[5].room_pos.y+g->rooms[5].room_size_v; j++) {
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
            character = mvinch(g->player_pos.y-g->players_speed, g->player_pos.x) & A_CHARTEXT;
            g->players_steps += 1;
            if(character == '<') { return 1; }
            if(character == '_') { return 4; }
            if(character == 'T') { return 7;}
            if(character == '*') { g->players_gold += 8; g->players_score +=15; screen[g->player_pos.x][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '$') { g->players_gold += 36; g->players_score +=40; screen[g->player_pos.x][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; return 0; }
            if(CheckTrap(g->rooms,g->player_pos.x,g->player_pos.y-g->players_speed)) { screen[g->player_pos.x][g->player_pos.y-g->players_speed]='^'; if(rand()%2) {return 6;} else {g->players_health -= 1; return 0;}}
            if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,g->player_pos.x,g->player_pos.y-g->players_speed)) { screen[g->player_pos.x][g->player_pos.y-g->players_speed]='?'; return 5;}
            if(character == '&' && g->rooms[g->k_lock].locked == 1) { g->password_start_time = time(NULL); g->player_pos.y -= g->players_speed; return 2; }
            if(character == '@' && g->rooms[g->k_lock].locked == 1) { return 3; }
            else if(character == '@' && g->rooms[g->k_lock].locked == 0) { g->player_pos.y -= g->players_speed; return 0; }
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y -= g->players_speed; }
            if(character == '!') { g->players_health_potion += 1; screen[g->player_pos.x][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; return 0; }
            if(character == 'o') { g->players_speed_potion += 1; screen[g->player_pos.x][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '6') { g->players_damage_potion += 1; screen[g->player_pos.x][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '~') { if(CheckWeapons(g->weapons,g->player_pos.x,g->player_pos.y-g->players_speed)) {g->players_dagger += 10;} else {g->players_dagger += 1;} screen[g->player_pos.x][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '/') { if(CheckWeapons(g->weapons,g->player_pos.x,g->player_pos.y-g->players_speed)) {g->players_magic_wand += 8;} else {g->players_magic_wand += 1;} screen[g->player_pos.x][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '}') { if(CheckWeapons(g->weapons,g->player_pos.x,g->player_pos.y-g->players_speed)) {g->players_arrow += 20;} else {g->players_arrow += 1;} screen[g->player_pos.x][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '=') { g->players_sword += 1; for(int i=-g->players_speed; i<=3; i++) {screen[g->player_pos.x+i][g->player_pos.y-g->players_speed]='.';} g->player_pos.y -= g->players_speed; return 0; }
            break;
        case '2':
            character = mvinch(g->player_pos.y+g->players_speed, g->player_pos.x) & A_CHARTEXT;
            g->players_steps += 1;
            if(character == 'T') { return 7;}
            if(character == '<') { return 1; }
            if(character == '_') { return 4; }
            if(character == '*') { g->players_gold += 8; g->players_score +=15; screen[g->player_pos.x][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; return 0; }
            if(character == '$') { g->players_gold += 36; g->players_score +=40; screen[g->player_pos.x][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; return 0; }
            if(CheckTrap(g->rooms,g->player_pos.x,g->player_pos.y+g->players_speed)) { screen[g->player_pos.x][g->player_pos.y+g->players_speed]='^'; if(rand()%2) {return 6;} else {g->players_health -= 1; return 0;}}
            if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,g->player_pos.x,g->player_pos.y+g->players_speed)) { screen[g->player_pos.x][g->player_pos.y+g->players_speed]='?'; return 5;}
            if(character == '&' && g->rooms[g->k_lock].locked == 1) { g->password_start_time = time(NULL); g->player_pos.y += g->players_speed; return 2; }
            if(character == '@' && g->rooms[g->k_lock].locked == 1) { return 3; }
            else if(character == '@' && g->rooms[g->k_lock].locked == 0) { g->player_pos.y += g->players_speed; return 0; }
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y += g->players_speed; }
            if(character == '!') { g->players_health_potion += 1; screen[g->player_pos.x][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; return 0; }
            if(character == 'o') { g->players_speed_potion += 1; screen[g->player_pos.x][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; return 0; }
            if(character == '6') { g->players_damage_potion += 1; screen[g->player_pos.x][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; return 0; }
            if(character == '~') { if(CheckWeapons(g->weapons,g->player_pos.x,g->player_pos.y+g->players_speed)) {g->players_dagger += 10;} else {g->players_dagger += 1;} screen[g->player_pos.x][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; return 0; }
            if(character == '/') { if(CheckWeapons(g->weapons,g->player_pos.x,g->player_pos.y+g->players_speed)) {g->players_magic_wand += 8;} else {g->players_magic_wand += 1;} screen[g->player_pos.x][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; return 0; }
            if(character == '}') { if(CheckWeapons(g->weapons,g->player_pos.x,g->player_pos.y+g->players_speed)) {g->players_arrow += 20;} else {g->players_arrow += 1;} screen[g->player_pos.x][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; return 0; }
            if(character == '=') { g->players_sword += 1; for(int i=-g->players_speed; i<=3; i++) {screen[g->player_pos.x+i][g->player_pos.y+g->players_speed]='.';} g->player_pos.y += g->players_speed; return 0; }
            break;
        case '4':
            if(character == 'T') { return 7;}
            character = mvinch(g->player_pos.y, g->player_pos.x-g->players_speed) & A_CHARTEXT;
            g->players_steps += 1;
            if(character == '<') { return 1; }
            if(character == '_') { return 4; }
            if(character == '*') { g->players_gold += 8; g->players_score +=15; screen[g->player_pos.x-g->players_speed][g->player_pos.y]='.'; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '$') { g->players_gold += 36; g->players_score +=40; screen[g->player_pos.x-g->players_speed][g->player_pos.y]='.'; g->player_pos.x -= g->players_speed; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y]='.'; g->player_pos.x -= g->players_speed; return 0; }
            if(CheckTrap(g->rooms,g->player_pos.x-g->players_speed,g->player_pos.y)) { screen[g->player_pos.x-g->players_speed][g->player_pos.y]='^'; if(rand()%2) {return 6;} else {g->players_health -= 1; return 0;}}
            if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,g->player_pos.x-g->players_speed,g->player_pos.y)) { screen[g->player_pos.x-g->players_speed][g->player_pos.y]='?'; return 5;}
            if(character == '&' && g->rooms[g->k_lock].locked == 1) { g->password_start_time = time(NULL); g->player_pos.x -= g->players_speed; return 2; }
            if(character == '@' && g->rooms[g->k_lock].locked == 1) { return 3; }
            else if(character == '@' && g->rooms[g->k_lock].locked == 0) { g->player_pos.x -= g->players_speed; return 0; }
            if (character == '.' || character == '+' || character == '#') { g->player_pos.x -= g->players_speed; }
            if(character == '!') { g->players_health_potion += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y]='.'; g->player_pos.x -= g->players_speed; return 0; }
            if(character == 'o') { g->players_speed_potion += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y]='.'; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '6') { g->players_damage_potion += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y]='.'; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '~') { if(CheckWeapons(g->weapons,g->player_pos.x-g->players_speed,g->player_pos.y)) {g->players_dagger += 10;} else {g->players_dagger += 1;} screen[g->player_pos.x-g->players_speed][g->player_pos.y]='.'; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '/') { if(CheckWeapons(g->weapons,g->player_pos.x-g->players_speed,g->player_pos.y)) {g->players_magic_wand += 8;} else {g->players_magic_wand += 1;} screen[g->player_pos.x-g->players_speed][g->player_pos.y]='.'; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '}') { if(CheckWeapons(g->weapons,g->player_pos.x-g->players_speed,g->player_pos.y)) {g->players_arrow += 20;} else {g->players_arrow += 1;} screen[g->player_pos.x-g->players_speed][g->player_pos.y]='.'; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '=') { g->players_sword += 1; for(int i=-g->players_speed; i<=3; i++) {screen[g->player_pos.x+i][g->player_pos.y+g->players_speed]='.';} g->player_pos.y += g->players_speed; return 0; }
            break;
        case '6':
            if(character == 'T') { return 7;}
            character = mvinch(g->player_pos.y, g->player_pos.x+g->players_speed) & A_CHARTEXT;
            g->players_steps += 1;
            if(character == '<') { return 1; }
            if(character == '_') { return 4; }
            if(character == '*') { g->players_gold += 8; g->players_score +=15; screen[g->player_pos.x+g->players_speed][g->player_pos.y]='.'; g->player_pos.x += g->players_speed; return 0; }
            if(character == '$') { g->players_gold += 36; g->players_score +=40; screen[g->player_pos.x+g->players_speed][g->player_pos.y]='.'; g->player_pos.x += g->players_speed; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y]='.'; g->player_pos.x += g->players_speed; return 0; }
            if(CheckTrap(g->rooms,g->player_pos.x+g->players_speed,g->player_pos.y)) { screen[g->player_pos.x+g->players_speed][g->player_pos.y]='^'; if(rand()%2) {return 6;} else {g->players_health -= 1; return 0;}}
            if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,g->player_pos.x+g->players_speed,g->player_pos.y)) { screen[g->player_pos.x+g->players_speed][g->player_pos.y]='?'; return 5;}
            if(character == '&' && g->rooms[g->k_lock].locked == 1) { g->password_start_time = time(NULL); g->player_pos.x += g->players_speed; return 2;}
            if(character == '@' && g->rooms[g->k_lock].locked == 1) { return 3; }
            else if(character == '@' && g->rooms[g->k_lock].locked == 0) { g->player_pos.x += g->players_speed; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.x += g->players_speed; }
            if(character == '!') { g->players_health_potion += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y]='.'; g->player_pos.x += g->players_speed; return 0; }
            if(character == 'o') { g->players_speed_potion += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y]='.'; g->player_pos.x += g->players_speed; return 0; }
            if(character == '6') { g->players_damage_potion += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y]='.'; g->player_pos.x += g->players_speed; return 0; }
            if(character == '~') { if(CheckWeapons(g->weapons,g->player_pos.x+g->players_speed,g->player_pos.y)) {g->players_dagger += 10;} else {g->players_dagger += 1;} screen[g->player_pos.x+g->players_speed][g->player_pos.y]='.'; g->player_pos.x += g->players_speed; return 0; }
            if(character == '/') { if(CheckWeapons(g->weapons,g->player_pos.x+g->players_speed,g->player_pos.y)) {g->players_magic_wand += 8;} else {g->players_magic_wand += 1;} screen[g->player_pos.x+g->players_speed][g->player_pos.y]='.'; g->player_pos.x += g->players_speed; return 0; }
            if(character == '}') { if(CheckWeapons(g->weapons,g->player_pos.x+g->players_speed,g->player_pos.y)) {g->players_arrow += 20;} else {g->players_arrow += 1;} screen[g->player_pos.x+g->players_speed][g->player_pos.y]='.'; g->player_pos.x += g->players_speed; return 0; }
            if(character == '=') { g->players_sword += 1; for(int i=-g->players_speed; i<=3; i++) {screen[g->player_pos.x+i][g->player_pos.y+g->players_speed]='.';} g->player_pos.y += g->players_speed; return 0; }
            break;
        case '9':
            if(character == 'T') { return 7;}
            character = mvinch(g->player_pos.y-g->players_speed, g->player_pos.x+g->players_speed) & A_CHARTEXT;
            g->players_steps += 1;
            if(character == '<') { return 1; }
            if(character == '_') { return 4; }
            if(character == '*') { g->players_gold += 8; g->players_score +=15; screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == '$') { g->players_gold += 36; g->players_score +=40; screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(CheckTrap(g->rooms,g->player_pos.x+g->players_speed,g->player_pos.y-g->players_speed)) { screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='^'; if(rand()%2) {return 6;} else {g->players_health -= 1; return 0;}}
            if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,g->player_pos.x+g->players_speed,g->player_pos.y-g->players_speed)) { screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='?'; return 5;}
            if(character == '&' && g->rooms[g->k_lock].locked == 1) { g->password_start_time = time(NULL); g->player_pos.x += g->players_speed; g->player_pos.y -= g->players_speed; return 2;}
            if(character == '@' && g->rooms[g->k_lock].locked == 1) { return 3; }
            else if(character == '@' && g->rooms[g->k_lock].locked == 0) { g->player_pos.x += g->players_speed; g->player_pos.y -= g->players_speed; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y -= g->players_speed; g->player_pos.x += g->players_speed; }
            if(character == '!') { g->players_health_potion += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == 'o') { g->players_speed_potion += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == '6') { g->players_damage_potion += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == '~') { if(CheckWeapons(g->weapons,g->player_pos.x+g->players_speed,g->player_pos.y-g->players_speed)) {g->players_dagger += 10;} else {g->players_dagger += 1;} screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.x += g->players_speed; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '/') { if(CheckWeapons(g->weapons,g->player_pos.x+g->players_speed,g->player_pos.y-g->players_speed)) {g->players_magic_wand += 8;} else {g->players_magic_wand += 1;} screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.x += g->players_speed; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '}') { if(CheckWeapons(g->weapons,g->player_pos.x+g->players_speed,g->player_pos.y-g->players_speed)) {g->players_arrow += 20;} else {g->players_arrow += 1;} screen[g->player_pos.x+g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.x += g->players_speed; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '=') { g->players_sword += 1; for(int i=-g->players_speed; i<=3; i++) {screen[g->player_pos.x+g->players_speed+i][g->player_pos.y-g->players_speed]='.';} g->player_pos.x += g->players_speed; g->player_pos.y -= g->players_speed; return 0; }
            break;
        case '7':
            if(character == 'T') { return 7;}
            character = mvinch(g->player_pos.y-g->players_speed, g->player_pos.x-g->players_speed) & A_CHARTEXT;
            g->players_steps += 1;
            if(character == '<') { return 1; }
            if(character == '_') { return 4; }
            if(character == '*') { g->players_gold += 8; g->players_score +=15; screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '$') { g->players_gold += 36; g->players_score +=40; screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(CheckTrap(g->rooms,g->player_pos.x-g->players_speed,g->player_pos.y-g->players_speed)) { screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='^'; if(rand()%2) {return 6;} else {g->players_health -= 1; return 0;}}
            if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,g->player_pos.x-g->players_speed,g->player_pos.y-g->players_speed)) { screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='?'; return 5;}
            if(character == '&' && g->rooms[g->k_lock].locked == 1) { g->password_start_time = time(NULL); g->player_pos.x -= g->players_speed; g->player_pos.y -= g->players_speed; return 2;}
            if(character == '@' && g->rooms[g->k_lock].locked == 1) { return 3; }
            else if(character == '@' && g->rooms[g->k_lock].locked == 0) { g->player_pos.x -= g->players_speed; g->player_pos.y -= g->players_speed; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y -= g->players_speed; g->player_pos.x -= g->players_speed; }
            if(character == '!') { g->players_health_potion += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == 'o') { g->players_speed_potion += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '6') { g->players_damage_potion += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.y -= g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '~') { if(CheckWeapons(g->weapons,g->player_pos.x-g->players_speed,g->player_pos.y-g->players_speed)) {g->players_dagger += 10;} else {g->players_dagger += 1;} screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.x -= g->players_speed; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '/') { if(CheckWeapons(g->weapons,g->player_pos.x-g->players_speed,g->player_pos.y-g->players_speed)) {g->players_magic_wand += 8;} else {g->players_magic_wand += 1;} screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.x -= g->players_speed; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '}') { if(CheckWeapons(g->weapons,g->player_pos.x-g->players_speed,g->player_pos.y-g->players_speed)) {g->players_arrow += 20;} else {g->players_arrow += 1;} screen[g->player_pos.x-g->players_speed][g->player_pos.y-g->players_speed]='.'; g->player_pos.x -= g->players_speed; g->player_pos.y -= g->players_speed; return 0; }
            if(character == '=') { g->players_sword += 1; for(int i=-g->players_speed; i<=3; i++) {screen[g->player_pos.x-g->players_speed+i][g->player_pos.y-g->players_speed]='.';} g->player_pos.x -= g->players_speed; g->player_pos.y -= g->players_speed; return 0; }
            break;
        case '3':
            if(character == 'T') { return 7;}
            character = mvinch(g->player_pos.y+g->players_speed, g->player_pos.x+g->players_speed) & A_CHARTEXT;
            g->players_steps += 1;
            if(character == '<') { return 1; }
            if(character == '_') { return 4; }
            if(character == '*') { g->players_gold += 8; g->players_score +=15; screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == '$') { g->players_gold += 36; g->players_score +=40; screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(CheckTrap(g->rooms,g->player_pos.x+g->players_speed,g->player_pos.y+g->players_speed)) { screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='^'; if(rand()%2) {return 6;} else {g->players_health -= 1; return 0;}}
            if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,g->player_pos.x+g->players_speed,g->player_pos.y+g->players_speed)) { screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='?'; return 5;}
            if(character == '&' && g->rooms[g->k_lock].locked == 1) { g->password_start_time = time(NULL); g->player_pos.x += g->players_speed; g->player_pos.y += g->players_speed; return 2;}
            if(character == '@' && g->rooms[g->k_lock].locked == 1) { return 3; }
            else if(character == '@' && g->rooms[g->k_lock].locked == 0) { g->player_pos.x += g->players_speed; g->player_pos.y += g->players_speed; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y += g->players_speed; g->player_pos.x += g->players_speed; }
            if(character == '!') { g->players_health_potion += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == 'o') { g->players_speed_potion += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == '6') { g->players_damage_potion += 1; screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x += g->players_speed; return 0; }
            if(character == '~') { if(CheckWeapons(g->weapons,g->player_pos.x+g->players_speed,g->player_pos.y+g->players_speed)) {g->players_dagger += 10;} else {g->players_dagger += 1;} screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.x += g->players_speed; g->player_pos.y += g->players_speed; return 0; }
            if(character == '/') { if(CheckWeapons(g->weapons,g->player_pos.x+g->players_speed,g->player_pos.y+g->players_speed)) {g->players_magic_wand += 8;} else {g->players_magic_wand += 1;} screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.x += g->players_speed; g->player_pos.y += g->players_speed; return 0; }
            if(character == '}') { if(CheckWeapons(g->weapons,g->player_pos.x+g->players_speed,g->player_pos.y+g->players_speed)) {g->players_arrow += 20;} else {g->players_arrow += 1;} screen[g->player_pos.x+g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.x += g->players_speed; g->player_pos.y += g->players_speed; return 0; }
            if(character == '=') { g->players_sword += 1; for(int i=-g->players_speed; i<=3; i++) {screen[g->player_pos.x+g->players_speed+i][g->player_pos.y+g->players_speed]='.';} g->player_pos.x += g->players_speed; g->player_pos.y += g->players_speed; return 0; }
            break;
        case '1':
            if(character == 'T') { return 7;}
            character = mvinch(g->player_pos.y+g->players_speed, g->player_pos.x-g->players_speed) & A_CHARTEXT;
            g->players_steps += 1;
            if(character == '<') { return 1; }
            if(character == '_') { return 4; }
            if(character == '*') { g->players_gold += 8; g->players_score +=15; screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '$') { g->players_gold += 36; g->players_score +=40; screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == 'f') { g->players_ordinary_food += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(CheckTrap(g->rooms,g->player_pos.x-g->players_speed,g->player_pos.y+g->players_speed)) { screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='^'; if(rand()%2) {return 6;} else {g->players_health -= 1; return 0;}}
            if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,g->player_pos.x-g->players_speed,g->player_pos.y+g->players_speed)) { screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='?'; return 5;}
            if(character == '&' && g->rooms[g->k_lock].locked == 1) { g->password_start_time = time(NULL); g->player_pos.x -= g->players_speed; g->player_pos.y += g->players_speed; return 2;}
            if(character == '@' && g->rooms[g->k_lock].locked == 1) { return 3; }
            else if(character == '@' && g->rooms[g->k_lock].locked == 0) { g->player_pos.x -= g->players_speed; g->player_pos.y += g->players_speed; return 0;}
            if (character == '.' || character == '+' || character == '#') { g->player_pos.y += g->players_speed; g->player_pos.x -= g->players_speed; }
            if(character == '!') { g->players_health_potion += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == 'o') { g->players_speed_potion += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '6') { g->players_damage_potion += 1; screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.y += g->players_speed; g->player_pos.x -= g->players_speed; return 0; }
            if(character == '~') { if(CheckWeapons(g->weapons,g->player_pos.x-g->players_speed,g->player_pos.y+g->players_speed)) {g->players_dagger += 10;} else {g->players_dagger += 1;} screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.x -= g->players_speed; g->player_pos.y += g->players_speed; return 0; }
            if(character == '/') { if(CheckWeapons(g->weapons,g->player_pos.x-g->players_speed,g->player_pos.y+g->players_speed)) {g->players_magic_wand += 8;} else {g->players_magic_wand += 1;} screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.x -= g->players_speed; g->player_pos.y += g->players_speed; return 0; }
            if(character == '}') { if(CheckWeapons(g->weapons,g->player_pos.x-g->players_speed,g->player_pos.y+g->players_speed)) {g->players_arrow += 20;} else {g->players_arrow += 1;} screen[g->player_pos.x-g->players_speed][g->player_pos.y+g->players_speed]='.'; g->player_pos.x -= g->players_speed; g->player_pos.y += g->players_speed; return 0; }
            if(character == '=') { g->players_sword += 1; for(int i=-g->players_speed; i<=3; i++) {screen[g->player_pos.x-g->players_speed+i][g->player_pos.y+g->players_speed]='.';} g->player_pos.x -= g->players_speed; g->player_pos.y += g->players_speed; return 0; }
            break;
    }
    return 0;
}

void ShowScreen(Game *g, char mode[], int **visited, chtype **screen) {
    if(strcmp(mode,"hidden") == 0) {
        for(int j=1; j<LINES; j++) {
            for(int i=0; i<COLS; i++) {
                if(visited[i][j]) {
                    if(CheckMonsters(g, i, j)) {
                        int boolean = 0;
                        int room = RoomChecking(g->rooms, i, j);
                        for(int m=0; m<g->rooms[room].monsters_count; m++) {
                            if(g->rooms[room].monsters[m].position.x == i && g->rooms[room].monsters[m].position.y == j) {
                                if(g->rooms[room].monsters[m].alive) {
                                    switch(g->rooms[room].monsters[m].type) {
                                        case 1:
                                            mvprintw(j,i,"D");
                                            boolean = 1;
                                            break;
                                        case 2:
                                            attron(COLOR_PAIR(7));
                                            mvprintw(j,i,"F");
                                            attroff(COLOR_PAIR(7));
                                            boolean = 1;
                                            break;
                                        case 3:
                                            attron(COLOR_PAIR(5));
                                            mvprintw(j,i,"G");
                                            attroff(COLOR_PAIR(5));
                                            boolean = 1;
                                            break;
                                        case 4:
                                            attron(COLOR_PAIR(3));
                                            mvprintw(j,i,"S");
                                            attroff(COLOR_PAIR(3));
                                            boolean = 1;
                                            break;
                                        case 5:
                                            attron(COLOR_PAIR(2));
                                            mvprintw(j,i,"U");
                                            attroff(COLOR_PAIR(2));
                                            boolean = 1;
                                        break;
                                    }
                                }
                            }
                        }
                        if(boolean) {continue;}
                    }
                    if(screen[i][j] == '@' && g->rooms[RoomChecking(g->rooms,i,j)].locked == 1) {
                        attron(COLOR_PAIR(2));
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(2));
                        continue;
                    }
                    else if(screen[i][j] == '@' && g->rooms[RoomChecking(g->rooms,i,j)].locked == 0) {
                        attron(COLOR_PAIR(3));
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(3));
                        continue;
                    }
                    else if(screen[i][j] == '&' || screen[i][j] == 'o') {
                        attron(COLOR_PAIR(1));
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(1));
                        continue;
                    }
                    else if(screen[i][j] == '^' || screen[i][j] == '6') {
                        attron(COLOR_PAIR(2));
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(2));
                        continue;
                    }
                    else if(screen[i][j] == '*') {
                        attron(COLOR_PAIR(4));
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(4));
                        continue;
                    }
                    else if(screen[i][j] == '$' || screen[i][j] == '/') {
                        attron(COLOR_PAIR(5));
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(5));
                        continue;
                    }
                    else if(screen[i][j] == 'f') {
                        attron(COLOR_PAIR(4)); attron(A_REVERSE);
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(4)); attroff(A_REVERSE);
                        continue;
                    }
                    else if(screen[i][j] == '!') {
                    attron(COLOR_PAIR(11));attron(A_REVERSE);
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(11));attroff(A_REVERSE);
                        continue;
                    }
                    else if(screen[i][j] == '?') {
                        attron(COLOR_PAIR(5)); attron(A_REVERSE);
                        mvprintw(j,i,"%c",screen[i][j]);
                        attroff(COLOR_PAIR(5)); attroff(A_REVERSE);
                        continue;
                    }                    
                    mvaddch(j,i,screen[i][j]);
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
                if(CheckMonsters(g, i, j)) {
                    int boolean = 0;
                    int room = RoomChecking(g->rooms, i, j);
                    for(int m=0; m<g->rooms[room].monsters_count; m++) {
                        if(g->rooms[room].monsters[m].position.x == i && g->rooms[room].monsters[m].position.y == j) {
                            if(g->rooms[room].monsters[m].alive) {
                                switch(g->rooms[room].monsters[m].type) {
                                    case 1:
                                        mvprintw(j,i,"D");
                                        boolean = 1;
                                        break;
                                    case 2:
                                        attron(COLOR_PAIR(7));
                                        mvprintw(j,i,"F");
                                        attroff(COLOR_PAIR(7));
                                        boolean = 1;
                                        break;
                                    case 3:
                                        attron(COLOR_PAIR(5));
                                        mvprintw(j,i,"G");
                                        attroff(COLOR_PAIR(5));
                                        boolean = 1;
                                        break;
                                    case 4:
                                        attron(COLOR_PAIR(3));
                                        mvprintw(j,i,"S");
                                        attroff(COLOR_PAIR(3));
                                        boolean = 1;
                                        break;
                                    case 5:
                                        attron(COLOR_PAIR(2));
                                        mvprintw(j,i,"U");
                                        attroff(COLOR_PAIR(2));
                                        boolean = 1;
                                       break;
                                }
                            }
                        }
                    }
                    if(boolean) {continue;}
                }
                if(screen[i][j] == '@' && g->rooms[RoomChecking(g->rooms,i,j)].locked == 1) {
                    attron(COLOR_PAIR(2));
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(2));
                    continue;
                }
                else if(screen[i][j] == '@' && g->rooms[RoomChecking(g->rooms,i,j)].locked == 0) {
                    attron(COLOR_PAIR(3));
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(3));
                    continue;
                }
                else if(screen[i][j] == '&' || screen[i][j] == 'o') {
                    attron(COLOR_PAIR(1));
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(1));
                    continue;
                }
                else if(screen[i][j] == '^' || screen[i][j] == '6') {
                    attron(COLOR_PAIR(2));
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(2));
                    continue;
                }
                else if(screen[i][j] == '*') {
                    attron(COLOR_PAIR(4));
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(4));
                    continue;
                }
                else if(screen[i][j] == '$' || screen[i][j] == '/') {
                    attron(COLOR_PAIR(5));
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(5));
                    continue;
                }
                else if(screen[i][j] == 'f') {
                    attron(COLOR_PAIR(4)); attron(A_REVERSE);
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(4)); attroff(A_REVERSE);
                    continue;
                }
                else if(screen[i][j] == '!') {
                    attron(COLOR_PAIR(11));attron(A_REVERSE);
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(11));attroff(A_REVERSE);
                    continue;
                }
                else if(screen[i][j] == '?') {
                    attron(COLOR_PAIR(5)); attron(A_REVERSE);
                    mvprintw(j,i,"%c",screen[i][j]);
                    attroff(COLOR_PAIR(5)); attroff(A_REVERSE);
                    continue;
                }        
                mvaddch(j,i,screen[i][j]);
            }
        }
    }   
}

int RoomChecking(Room *rooms, int i, int j) {
    for(int k=0; k<6; k++) {
        if(abs(i-rooms[k].room_pos.x) <= rooms[k].room_size_h && abs(j-rooms[k].room_pos.y) <= rooms[k].room_size_v) {
            return k;
        }
    }
}

int CheckTrap(Room *rooms, int i, int j) {
    for(int k=0; k<6; k++) {
        for(int l=0; l<rooms[k].traps_count; l++) {
            if(i == rooms[k].traps[l].x && j == rooms[k].traps[l].y) {
                return 1;
            }
        }
    }
    return 0;
}

int CheckEnchantDoor(int count, Pos *secret_doors, int i, int j) {
    for(int k=0; k<count; k++) {
        if(i == secret_doors[k].x && j == secret_doors[k].y) {
            return 1;
        }
    }
    return 0;
}

void EnchantRoom(User *p, Game *g) {
    clear();

    play_music("/home/matin/Desktop/fop2024-project-MatinFadavi0/BP_Project/music/track3.mp3");

    strcpy(message, "You've Entered The ENCHANT ROOM!");

    int gold = 1 + rand()%2;
    int ordinary_food = 1 + rand()%2;
    int potion = 3 + rand()%4;

    int x_center = COLS/2;
    int y_center = LINES/2;

    Pos secret_door;
    mvprintw(4, x_center, "|");
    mvprintw(44, x_center, "|");
    mvprintw(4, x_center+1, "|");
    mvprintw(44, x_center+1, "|");
    int m=0;
    for(m; m<20; m++) {
        mvprintw(5+m, x_center-m-1, "|");
        for(int n=0; n<2*m+2; n++) {
            mvprintw(5+m, x_center-m+n, ".");
        }
        mvprintw(5+m, x_center+m+2, "|");
    }
    for(m = 19; m >= 0; m--) {
        mvprintw(43 - m, x_center - m - 1, "|");
        for(int n = 0; n < 2 * m + 2; n++) {
            mvprintw(43 - m, x_center - m + n, ".");
        }
        mvprintw(43 - m, x_center + m + 2, "|");
    }

    g->player_pos.x = x_center;
    g->player_pos.y = 5 + rand()%20;

    int count_gold = 0;
    int count_ordinary_food = 0;
    int count_potion = 0;

    m = 0;
    for(m; m<20; m++) {
        for(int n=0; n<2*m+2; n++) {
            if(rand()%20 == 0 && mvinch(5+m,x_center-m+n) == '.' && count_gold<gold) {
                mvprintw(5+m,x_center-m+n,"*");
                mvprintw( 43 - m , x_center - m + n, "*");
                count_gold++;
            }
            if(rand()%20 == 0 && mvinch(5+m,x_center-m+n) == '.' && count_ordinary_food<ordinary_food) {
                mvprintw(5+m, x_center-m+n,"f");
                mvprintw( 43 - m , x_center - m + n, "f");
                count_ordinary_food++;
            }
            if(rand()%20 == 0 && mvinch(5+m,x_center-m+n) == '.' && count_potion<potion) {
                int num = rand()%2 + rand()%2;
                if(num%3 == 0) {
                    attron(COLOR_PAIR(3));attron(A_REVERSE);
                    mvprintw(5+m, x_center-m+n,"!");
                    mvprintw( 43 - m , x_center - m + n, "!");
                    attroff(COLOR_PAIR(3));attroff(A_REVERSE);
                }
                else if(num%3 == 1) {
                    attron(COLOR_PAIR(1));
                    mvprintw(5+m, x_center-m+n,"o");
                    mvprintw( 43 - m , x_center - m + n, "o");   
                    attroff(COLOR_PAIR(1));
                }
                else if(num%3 == 2) {
                    attron(COLOR_PAIR(2));
                    mvprintw(5+m, x_center-m+n,"6");
                    mvprintw( 43 - m , x_center - m + n, "6");
                    attroff(COLOR_PAIR(2));
                }
                count_potion++;
            }
        }
    }

    count_gold = 0; count_ordinary_food = 0; int count_secret_door = 0; count_potion = 0;
    for(m; m>=0; m--) {
        for(int n=0; n<2*m+2; n++) {
            if(rand()%20 == 0 && mvinch(5+m,x_center-m+n) == '.' && count_gold<gold) {
                mvprintw( 43 - m , x_center - m + n, "*");
                count_gold++;
            }
            if(rand()%20 == 0 && mvinch(5+m,x_center-m+n) == '.' && count_ordinary_food<ordinary_food) {
                mvprintw( 43 - m , x_center - m + n, "f");
                count_ordinary_food++;
            }
            if(rand()%20 == 0 && mvinch(5+m,x_center-m+n) == '.' && count_potion<potion) {
                int num = rand()%3 + rand()%3;
                if(num%3 == 0) {
                    attron(COLOR_PAIR(3));attron(A_REVERSE);
                    mvprintw( 43 - m , x_center - m + n, "!");
                    attroff(COLOR_PAIR(3));attroff(A_REVERSE);
                }
                else if(num%3 == 1) {
                    attron(COLOR_PAIR(1));
                    mvprintw( 43 - m , x_center - m + n, "o");   
                    attroff(COLOR_PAIR(1));
                }
                else if(num%3 == 2) {
                    attron(COLOR_PAIR(2));
                    mvprintw( 43 - m , x_center - m + n, "6");
                    attroff(COLOR_PAIR(2));
                }
                count_potion++;
            }
            if(rand()%20 == 0 && mvinch(5+m,x_center-m+n) == '.' && count_secret_door<1) {
                g->secret_doors_count = 1;
                g->secret_doors[0].x = x_center-m+n; g->secret_doors[0].y = 5+m;
                mvprintw(5+m, x_center-m+n,"?");
                count_secret_door++;
            }
        }
    }


    chtype **screen = (chtype **) malloc(COLS*sizeof(chtype*));
    for(int i=0; i<COLS; i++) {
        *(screen+i) = (chtype *) malloc(LINES*sizeof(chtype));
    }
    for(int j=1; j<LINES; j++) {
        for(int i=0; i<COLS; i++) {
            chtype character = mvinch(j,i);
            screen[i][j] = character;
        }
    }

    int **visited = (int **) malloc(COLS*sizeof(int *));
    for(int i=0; i<COLS; i++) {
        *(visited+i) = (int *) calloc(LINES,sizeof(int));
    }

    for(int j=1; j<LINES; j++) {
        for(int i=0; i<COLS; i++) {
            visited[i][j] = 1;
        }
    }

    int num = 0; int exit = 0;
    ShowScreen(g,"view",visited,screen);
    int health_loss = 2;
    while(1) {
        MessageBar(g, g->floor_number, g->players_score, g->players_gold);
        HEALTH_BAR(g);
        time_t current_time = time(NULL);
        double elapsed_time = difftime(current_time, g->enchant_start_time);

        if(elapsed_time >= 4) {
            g->enchant_start_time = time(NULL);
            num++;
        }
 
        if(num == 4) {
            g->players_health -= health_loss;
            health_loss ++;
            num = 0;
        }

        if(g->players_steps - g->players_speed_step >= 15) {g->players_speed = 1; g->players_speed_step = -15;}
        if(g->players_steps - g->players_health_step >= 20) {g->players_extra_health = 0; g->players_health_step = -20;}
        if(g->players_steps - g->players_damage_step >= 5) {g->players_damage_step = -5;}

        if(g->players_health + g->players_extra_health <= 0) {
            terminate_game(0, p, g);
        }

        ShowScreen(g,"view",visited,screen);

        attron(COLOR_PAIR(g->player_color));
        mvprintw(g->player_pos.y, g->player_pos.x, "@");
        attroff(COLOR_PAIR(g->player_color));

        int ch = getch();

        if(ch == 'f') {
            FoodTab(g);
        }
        else if(ch == 'i') {
            WeaponTab(g);
        }
        else if(ch == 'p') {
            SpellTab(g);
        }
        else if (ch == 'k'){
            show_key_guide();
        }
        switch(Movement(screen, visited, ch, g,p)) {
            case 5:
                     exit = 1;
                break;
        }
        if(exit) {
            stop_music();
            break;
        }
    }
}

void TreasureRoom(User *p, Game *g) {
    clear();

    play_music("/home/matin/Desktop/fop2024-project-MatinFadavi0/BP_Project/music/track4.mp3");

    strcpy(message, "You've Entered The TEASURE ROOM!");

    int dark_gold = 10 + rand() % 5;
    int traps_count = 10 + rand() % 5;
    int room_size = 53;

    int x_center = COLS/2;
    int y_center = LINES/2;

    for (int i = x_center - room_size/2; i <= x_center + room_size/2; i++) {
        mvprintw(y_center - room_size/2, i, "_"); // دیوار بالا
        mvprintw((y_center + room_size/2)/2, i, "_"); // دیوار پایین
    }
    for (int j = y_center - room_size/2; j <= (y_center + room_size/2)/2 -1; j++) {
        mvprintw(j+1, x_center - room_size/2, "|"); // دیوار چپ
        mvprintw(j+1, x_center + room_size/2, "|"); // دیوار راست
    }

    for (int i = x_center - room_size/2 + 1; i < x_center + room_size/2; i++) {
        for (int j = y_center - room_size/2 + 1; j < (y_center + room_size/2)/2; j++) {
            mvprintw(j, i, ".");
        }
    }

    g->player_pos.x = x_center;
    g->player_pos.y = 5 + rand()%20;

    int count_gold = 0;

    g->player_pos.x = x_center;
    g->player_pos.y = 5 + rand() % 20;


    for (int i = 0; i < dark_gold; i++) {
        int x, y;
        do {
            x = x_center - room_size / 2 + 1 + rand() % (room_size - 2);
            y = y_center - room_size / 2 + 1 + rand() % ((room_size / 2) - 2);
        } while (mvinch(y, x) != '.');

        mvprintw(y, x, "$");
    }


    chtype **screen = (chtype **) malloc(COLS*sizeof(chtype*));
    for(int i=0; i<COLS; i++) {
        *(screen+i) = (chtype *) malloc(LINES*sizeof(chtype));
    }
    for(int j=1; j<LINES; j++) {
        for(int i=0; i<COLS; i++) {
            chtype character = mvinch(j,i);
            screen[i][j] = character;
        }
    }

    int **visited = (int **) malloc(COLS*sizeof(int *));
    for(int i=0; i<COLS; i++) {
        *(visited+i) = (int *) calloc(LINES,sizeof(int));
    }

    for(int j=1; j<LINES; j++) {
        for(int i=0; i<COLS; i++) {
            visited[i][j] = 1;
        }
    }

    int exit = 0;
    ShowScreen(g,"view",visited,screen);
    int health_loss = 1;
    while(1) {
        MessageBar(g, g->floor_number, g->players_score, g->players_gold);
        HEALTH_BAR(g);
        time_t current_time = time(NULL);
        double elapsed_time = difftime(current_time, g->treasure_start_time);

        if(elapsed_time >= 25) {
            g->treasure_start_time = time(NULL);
            g->players_health -= 10 * health_loss;
        }

        if ( g->players_health <= 0 && elapsed_time < 24){
            terminate_game(0,p,g); 
        }
        if ( g->players_health <= 0 && elapsed_time >= 25 ){
            terminate_game(1,p,g);
        }

        ShowScreen(g,"view",visited,screen);

        attron(COLOR_PAIR(g->player_color));
        mvprintw(g->player_pos.y, g->player_pos.x, "@");
        attroff(COLOR_PAIR(g->player_color));

        int ch = getch();

        if(ch == 'f') {
            FoodTab(g);
        }
        else if(ch == 'i') {
            WeaponTab(g);
        }
        else if(ch == 'p') {
            SpellTab(g);
        }
        else if (ch == 'k'){
            show_key_guide();
        }
        switch(Movement(screen, visited, ch, g,p)) {
            case 7:
                     exit = 1;
                break;
        }
        if(exit) {
            stop_music();
            break;
        }
    }
}

void password_screen(Game *g) {
    clear();
    echo();
    curs_set(TRUE);

    int try = 0;
    char password[5];
    int reverse = (rand() % 5 == 3) ? 1 : 0;
    int highlight = 0;
    int row, col;

    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 15) / 2, "Password Screen");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        char *options[] = {"Enter Password", "Quit"};
        for (int i = 0; i < 2; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 20) / 2, options[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        int key = getch();
        if (key == KEY_UP && highlight > 0) {
            highlight--;
        } else if (key == KEY_DOWN && highlight < 1) {
            highlight++;
        } else if (key == 10) {  // Enter key
            if (highlight == 1) {  // Quit
                curs_set(FALSE);
                noecho();
                return;
            } else {
                break;  // Enter Password
            }
        }
    }

    while (try < 3) {
        mvprintw(row / 2, (col - 18) / 2, "Enter the password: ");
        clrtoeol();
        getnstr(password, 4);

        int len = strlen(password);
        if (reverse && len > 0) {
            char temp_password[5];
            for (int i = 0; i < len; i++) {
                temp_password[i] = password[len - i - 1];
            }
            temp_password[len] = '\0';
            strcpy(password, temp_password);
        }

        if (str_to_num(password) == g->rooms[g->k_lock].password) {
            g->rooms[g->k_lock].locked = 0;
            curs_set(FALSE);
            noecho();
            return;
        } else {
            try++;
            switch (try) {
                case 1:
                    attron(COLOR_PAIR(2));
                    mvprintw(row / 2 + 2, (col - 40) / 2, "Ritard?");
                    attroff(COLOR_PAIR(2));
                    break;
                case 2:
                    attron(COLOR_PAIR(2));
                    mvprintw(row / 2 + 3, (col - 40) / 2, "Yo be carefull this is your last chance!");
                    attroff(COLOR_PAIR(2));
                    break;
                case 3:
                    attron(COLOR_PAIR(2));
                    mvprintw(row / 2 + 4, (col - 40) / 2, "Bro comeback with someone who can write a fucking password");
                    attroff(COLOR_PAIR(2));
                    curs_set(FALSE);
                    noecho();
                    getch();
                    return;
            }
            refresh();
        }
    }
}


//////////////////////////////////////////////////////// Monsters /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Monsters /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Monsters /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Monsters /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Monsters /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Monsters /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Monsters /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Monsters /////////////////////////////////////////////////////////////////////////


void Monsters_Movement(Game *g, chtype **screen) {
    for(int k=0; k<6; k++) {
        for(int m=0; m<g->rooms[k].monsters_count; m++) {
            int delta_x;
            int delta_y;
            if(g->rooms[k].monsters[m].alive && g->rooms[k].monsters[m].on) {
                if(g->rooms[k].monsters[m].haunt < 0) {
                    if(g->rooms[k].monsters[m].type == 1) {
                        g->rooms[k].monsters[m].on = 0;
                        g->rooms[k].monsters[m].haunt = 0;
                    }
                    else if(g->rooms[k].monsters[m].type == 2) {
                            g->rooms[k].monsters[m].on = 0;
                            g->rooms[k].monsters[m].haunt = 0;
                    }
                    else {
                        if(g->rooms[k].monsters[m].position.x < g->rooms[k].pillar_seed.x) {
                            delta_x = 1;
                        }
                        else if(g->rooms[k].monsters[m].position.x == g->rooms[k].pillar_seed.x) {
                            delta_x = 0;
                        }
                        else {
                            delta_x = -1;
                        }
                        if(g->rooms[k].monsters[m].position.y < g->rooms[k].pillar_seed.y) {
                            delta_y = 1;
                        }
                        else if(g->rooms[k].monsters[m].position.y == g->rooms[k].pillar_seed.y) {
                            delta_y = 0;
                        }
                        else {
                            delta_y = -1;
                        }
                        char ch = screen[g->rooms[k].monsters[m].position.x + delta_x][g->rooms[k].monsters[m].position.y + delta_y] & A_CHARTEXT;
                        if(ch == '.' && (g->rooms[k].monsters[m].position.x + delta_x != g->rooms[k].pillar_seed.x || g->rooms[k].monsters[m].position.y + delta_y != g->rooms[k].pillar_seed.y)) {
                            g->rooms[k].monsters[m].position.x += delta_x; g->rooms[k].monsters[m].position.y += delta_y;
                        }

                        if(abs(g->rooms[k].monsters[m].position.x - g->player_pos.x) > g->rooms[k].monsters[m].radius+1 || abs(g->rooms[k].monsters[m].position.y - g->player_pos.y) > g->rooms[k].monsters[m].radius+1) {
                            if(g->rooms[k].monsters[m].type == 3) {g->rooms[k].monsters[m].haunt = 5;}
                            else if(g->rooms[k].monsters[m].type == 5) {g->rooms[k].monsters[m].haunt = 8;}
                        }
                    }
                    
                    return;
                }
                
                if((abs(g->rooms[k].monsters[m].position.x - g->player_pos.x) <= 1) && (abs(g->rooms[k].monsters[m].position.y - g->player_pos.y) <= 1)) {
                    if(g->players_extra_health > 0) {
                        g->players_extra_health -= g->rooms[k].monsters[m].damage;
                    }
                    else {
                        g->players_health -= g->rooms[k].monsters[m].damage;
                    }
                    if(g->rooms[k].monsters[m].type == 1) {strcpy(message, "Daemon scored an excellent hit on you!");}
                    else if(g->rooms[k].monsters[m].type == 2) {strcpy(message, "Fire Breathing Monster scored an excellent hit on you!");}
                    else if(g->rooms[k].monsters[m].type == 3) {strcpy(message, "Giant scored an excellent hit on you!");}
                    else if(g->rooms[k].monsters[m].type == 4) {strcpy(message, "Snake scored an excellent hit on you!");}
                    else if(g->rooms[k].monsters[m].type == 5) {strcpy(message, "Undead scored an excellent hit on you!");}
                }
                
                if(g->rooms[k].monsters[m].type == 1) {
                    g->rooms[k].monsters[m].haunt -= 1;
                }
                else if ( g->rooms[k].monsters[m].type == 2 ){
                    g->rooms[k].monsters[m].haunt -= 1;
                }
                else {
                    if(g->player_pos.x > g->rooms[k].monsters[m].position.x) {
                        delta_x = 1;
                    }
                    else if(g->player_pos.x == g->rooms[k].monsters[m].position.x) {
                        delta_x = 0;
                    }
                    else {
                        delta_x = -1;
                    }
                    if(g->player_pos.y > g->rooms[k].monsters[m].position.y) {
                        delta_y = 1;
                    }
                    else if(g->player_pos.y == g->rooms[k].monsters[m].position.y) {
                        delta_y = 0;
                    }
                    else {
                        delta_y = -1;
                    }
                    char ch = screen[g->rooms[k].monsters[m].position.x + delta_x][g->rooms[k].monsters[m].position.y + delta_y] & A_CHARTEXT;
                    if((ch == '.' && (g->rooms[k].monsters[m].position.x + delta_x != g->player_pos.x || g->rooms[k].monsters[m].position.y + delta_y != g->player_pos.y)) ||  (g->rooms[k].monsters[m].type == 4 && ((ch == '.' || ch == '+' || ch == '#') && (g->rooms[k].monsters[m].position.x + delta_x != g->player_pos.x || g->rooms[k].monsters[m].position.y + delta_y != g->player_pos.y)))) {
                        g->rooms[k].monsters[m].position.x += delta_x; g->rooms[k].monsters[m].position.y += delta_y;
                        g->rooms[k].monsters[m].haunt -= 1;
                    }
                }
            } 
        }
    }
}


void handle_monsters(Game *g, chtype **screen) {
    for(int k=0; k<6; k++) {
        for(int m=0; m<g->rooms[k].monsters_count; m++) {
            if(g->rooms[k].monsters[m].alive == 0) {
                g->rooms[k].monsters[m].on  = 0;
            }
            if(k != RoomChecking(g->rooms, g->player_pos.x, g->player_pos.y)) {
                g->rooms[k].monsters[m].on  = 0;
            }
            else {
                if(abs(g->player_pos.x - g->rooms[k].monsters[m].position.x) <= g->rooms[k].monsters[m].radius && abs(g->player_pos.y - g->rooms[k].monsters[m].position.y) <= g->rooms[k].monsters[m].radius) {
                    g->rooms[k].monsters[m].on  = 1;
                }
            }
        }
    }
}


int CheckMonsters(Game *g, int i, int j) {
    for(int k=0; k<6; k++) {
        for(int m=0; m<g->rooms[k].monsters_count; m++) {
            if(i == g->rooms[k].monsters[m].position.x && j == g->rooms[k].monsters[m].position.y) {
                return g->rooms[k].monsters[m].type;
            }
        }
    }
    return 0;
}

//////////////////////////////////////////////////////// Weapons /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Weapons /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Weapons /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Weapons /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Weapons /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Weapons /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Weapons /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// Weapons /////////////////////////////////////////////////////////////////////////

void Active_Weapons(Game *g, char ch, chtype **screen) {
    if (ch == 'w') {
        g->players_weapon = -1;
    }
    else if (ch == 'z') {
        if(g->players_mace > 0 && (g->players_weapon == -1 || g->players_weapon == 0)) {
            g->players_weapon = 0;
            strcpy(message, "weapon changed to mace");
        }
        else if(g->players_mace == 0) {
            strcpy(message, "You have no mace");
        }
        else {
            strcpy(message, "Unequip your current weapon first using 'W'");
        }
    }
    else if (ch == 'x') {
        if(g->players_sword > 0 && (g->players_weapon == -1 || g->players_weapon == 1)) {
            g->players_weapon = 1;
            strcpy(message, "weapon changed to sword");
        }
        else if(g->players_sword == 0) {
            strcpy(message, "You have no sword");
        }
        else {
            strcpy(message, "Unequip your current weapon first using 'W'");
        }
    }
    else if (ch == 'c') {
        if(g->players_dagger > 0 && (g->players_weapon == -1 || g->players_weapon == 2)) {
            g->players_weapon = 2;
            strcpy(message, "Unequip your current weapon first using 'W'");
        }
        else if(g->players_dagger == 0) {
            strcpy(message, "You have no dagger");
        }
        else {
            strcpy(message, "Unequip your current weapon first using 'W'");
        }
    }
    else if (ch == 'v') {
        if(g->players_magic_wand > 0 && (g->players_weapon == -1 || g->players_weapon == 3)) {
            g->players_weapon = 3;
            strcpy(message, "weapon changed to magic wand");
        }
        else if(g->players_magic_wand == 0) {
            strcpy(message, "You have no magic wand");
        }
        else {
            strcpy(message, "Unequip your current weapon first using 'W'");
        }
    }
    else if (ch == 'b') {
        if(g->players_arrow > 0 && (g->players_weapon == -1 || g->players_weapon == 4)) {
            g->players_weapon = 4;
            strcpy(message, "weapon changed to arrow");
        }
        else if(g->players_arrow == 0) {
            strcpy(message, "You have no arrow");
        }
        else {
            strcpy(message, "Unequip your current weapon first using 'W'");
        }
    }
    if (ch == ' ') {
        if(g->players_weapon == 0 || g->players_weapon == 1) {
            for(int i=g->player_pos.x-1; i<=g->player_pos.x+1; i++) {
                for(int j=g->player_pos.y-1; j<=g->player_pos.y+1; j++) {
                    int k = RoomChecking(g->rooms, i, j);
                    for(int m=0; m<g->rooms[k].monsters_count; m++) {
                        if(g->rooms[k].monsters[m].position.x == i && g->rooms[k].monsters[m].position.y == j && g->rooms[k].monsters[m].alive) {
                            if(g->players_weapon == 0) {g->rooms[k].monsters[m].health -= g->players_damage_rate*5;}
                            else {g->rooms[k].monsters[m].health -= g->players_damage_rate*10;}
                            int type = CheckMonsters(g, i, j);
                            if(type == 1) {strcpy(message, "You scored an excellent hit on Daemon"); g->players_score += 5;}
                            else if(type == 2) {strcpy(message, "You scored an excellent hit on Fire Breathing Monster"); g->players_score += 5;}
                            else if(type == 3) {strcpy(message, "You scored an excellent hit on Giant"); g->players_score += 5;}
                            else if(type == 4) {strcpy(message, "You scored an excellent hit on Snake"); g->players_score += 10;}
                            else if(type == 5) {strcpy(message, "You scored an excellent hit on Undead"); g->players_score += 10;}
                            if(g->rooms[k].monsters[m].health <= 0) {
                                g->rooms[k].monsters[m].alive = 0;
                                if(g->rooms[k].monsters[m].type == 1) {strcpy(message, "You killed the Daemon!"); g->players_score += 10;}
                                else if(g->rooms[k].monsters[m].type == 2) {strcpy(message, "You killed the Fire Breathing Monster!"); g->players_score += 10;}
                                else if(g->rooms[k].monsters[m].type == 3) {strcpy(message, "You killed the Giant!"); g->players_score += 10;}
                                else if(g->rooms[k].monsters[m].type == 4) {strcpy(message, "You killed the Snake!"); g->players_score += 20;}
                                else if(g->rooms[k].monsters[m].type == 5) {strcpy(message, "You killed the Undead!"); g->players_score += 20;}
                            }
                        }
                    }
                }
            }
        }
        else {
            ch = getch();
            g->players_weapon_direction = ch;
            ThrowWeapon(g, ch, screen);
        }
    }
    if (ch == 'a') {
        ThrowWeapon(g, g->players_weapon_direction, screen);
    }
    else {
        return;
    }
}

void ThrowWeapon(Game *g, char ch, chtype **screen) {
    int range;
    if(g->players_weapon == 2) {range = 5;}
    else if(g->players_weapon == 3) {range = 10;}
    else if(g->players_weapon == 4) {range = 5;}
    if(ch == '6') {
        for(int i=g->player_pos.x; i<=g->player_pos.x+range; i++) {
            int j=g->player_pos.y;
            char character = screen[i][j] & A_CHARTEXT;
            if(character == '|' || character == 'O' || character == '+') {if(g->players_weapon == 2) {g->players_dagger -= 1; screen[i-1][j] = '~';} else if(g->players_weapon == 3) {g->players_magic_wand -= 1; screen[i-1][j] = ('/' | COLOR_PAIR(5));} else if(g->players_weapon == 4) {g->players_arrow -= 1; screen[i-1][j] = ('}' | COLOR_PAIR(7));} break;}
            int k = RoomChecking(g->rooms, i, j);
            int shot = 0;
            for(int m=0; m<g->rooms[k].monsters_count; m++) {
                if(g->rooms[k].monsters[m].position.x == i && g->rooms[k].monsters[m].position.y == j && g->rooms[k].monsters[m].alive) {
                    if(g->players_weapon == 2) {g->players_dagger -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*12;}
                    else if(g->players_weapon == 3) {g->players_magic_wand -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*15;}
                    else if(g->players_weapon == 4) {g->players_arrow -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*5;}
                    int type = CheckMonsters(g, i, j);
                    if(type == 1) {strcpy(message, "You scored an excellent hit on Daemon"); g->players_score += 5;}
                    else if(type == 2) {strcpy(message, "You scored an excellent hit on Fire Breathing Monster"); g->players_score += 5;}
                    else if(type == 3) {strcpy(message, "You scored an excellent hit on Giant"); g->players_score += 5;}
                    else if(type == 4) {strcpy(message, "You scored an excellent hit on Snake"); g->players_score += 10;}
                    else if(type == 5) {strcpy(message, "You scored an excellent hit on Undead"); g->players_score += 10;}
                    if(g->rooms[k].monsters[m].health <= 0) {
                        g->rooms[k].monsters[m].alive = 0;
                        if(g->rooms[k].monsters[m].type == 1) {strcpy(message, "You killed the Daemon!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 2) {strcpy(message, "You killed the Fire Breathing Monster!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 3) {strcpy(message, "You killed the Giant!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 4) {strcpy(message, "You killed the Snake!"); g->players_score += 20;}
                        else if(g->rooms[k].monsters[m].type == 5) {strcpy(message, "You killed the Undead!"); g->players_score += 20;}
                    }
                    shot = 1;
                }
            }
            if(shot) {break;}
            if(i == g->player_pos.x+range) {if(g->players_weapon == 2) {g->players_dagger -= 1; screen[i][j] = '~';} else if(g->players_weapon == 3) {g->players_magic_wand -= 1; screen[i][j] = ('/' | COLOR_PAIR(5));} else if(g->players_weapon == 4) {g->players_arrow -= 1; screen[i][j] = ('}' | COLOR_PAIR(7));}}
        }
    }
    else if(ch == '4') {
        for(int i=g->player_pos.x; i>=g->player_pos.x-range; i--) {
            int j=g->player_pos.y;
            char character = screen[i][j] & A_CHARTEXT;
            if(character == '|' || character == 'O' || character == '+') {if(g->players_weapon == 2) {g->players_dagger -= 1; screen[i+1][j] = '~';} else if(g->players_weapon == 3) {g->players_magic_wand -= 1; screen[i+1][j] = ('/' | COLOR_PAIR(5));} else if(g->players_weapon == 4) {g->players_arrow -= 1; screen[i+1][j] = ('}' | COLOR_PAIR(7));} break;}
            int k = RoomChecking(g->rooms, i, j);
            int shot = 0;
            for(int m=0; m<g->rooms[k].monsters_count; m++) {
                if(g->rooms[k].monsters[m].position.x == i && g->rooms[k].monsters[m].position.y == j && g->rooms[k].monsters[m].alive) {
                    if(g->players_weapon == 2) {g->players_dagger -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*12;}
                    else if(g->players_weapon == 3) {g->players_magic_wand -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*15;}
                    else if(g->players_weapon == 4) {g->players_arrow -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*5;}
                    int type = CheckMonsters(g, i, j);
                    if(type == 1) {strcpy(message, "You scored an excellent hit on Daemon"); g->players_score += 5;}
                    else if(type == 2) {strcpy(message, "You scored an excellent hit on Fire Breathing Monster"); g->players_score += 5;}
                    else if(type == 3) {strcpy(message, "You scored an excellent hit on Giant"); g->players_score += 5;}
                    else if(type == 4) {strcpy(message, "You scored an excellent hit on Snake"); g->players_score += 10;}
                    else if(type == 5) {strcpy(message, "You scored an excellent hit on Undead"); g->players_score += 10;}
                    if(g->rooms[k].monsters[m].health <= 0) {
                        g->rooms[k].monsters[m].alive = 0;
                        if(g->rooms[k].monsters[m].type == 1) {strcpy(message, "You killed the Daemon!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 2) {strcpy(message, "You killed the Fire Breathing Monster!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 3) {strcpy(message, "You killed the Giant!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 4) {strcpy(message, "You killed the Snake!"); g->players_score += 20;}
                        else if(g->rooms[k].monsters[m].type == 5) {strcpy(message, "You killed the Undead!"); g->players_score += 20;}
                    }
                    shot = 1;
                }
            }
            if(shot) {break;}
            if(i == g->player_pos.x-range) {if(g->players_weapon == 2) {g->players_dagger -= 1; screen[i][j] = '~';} else if(g->players_weapon == 3) {g->players_magic_wand -= 1; screen[i][j] = ('/' | COLOR_PAIR(5));} else if(g->players_weapon == 4) {g->players_arrow -= 1; screen[i][j] = ('}' | COLOR_PAIR(7));}}
        }
    }
    else if(ch == '2') {
        for(int j=g->player_pos.y; j<=g->player_pos.y+range; j++) {
            int i=g->player_pos.x;
            char character = screen[i][j] & A_CHARTEXT;
            if(character == '-' || character == 'O' || character == '+') {if(g->players_weapon == 2) {g->players_dagger -= 1; screen[i][j-1] = '~';} else if(g->players_weapon == 3) {g->players_magic_wand -= 1; screen[i][j-1] = ('/' | COLOR_PAIR(5));} else if(g->players_weapon == 4) {g->players_arrow -= 1; screen[i][j-1] = ('}' | COLOR_PAIR(7));} break;}
            int k = RoomChecking(g->rooms, i, j);
            int shot = 0;
            for(int m=0; m<g->rooms[k].monsters_count; m++) {
                if(g->rooms[k].monsters[m].position.x == i && g->rooms[k].monsters[m].position.y == j && g->rooms[k].monsters[m].alive) {
                    if(g->players_weapon == 2) {g->players_dagger -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*12;}
                    else if(g->players_weapon == 3) {g->players_magic_wand -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*15;}
                    else if(g->players_weapon == 4) {g->players_arrow -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*5;}
                    int type = CheckMonsters(g, i, j);
                    if(type == 1) {strcpy(message, "You scored an excellent hit on Daemon"); g->players_score += 5;}
                    else if(type == 2) {strcpy(message, "You scored an excellent hit on Fire Breathing Monster"); g->players_score += 5;}
                    else if(type == 3) {strcpy(message, "You scored an excellent hit on Giant"); g->players_score += 5;}
                    else if(type == 4) {strcpy(message, "You scored an excellent hit on Snake"); g->players_score += 10;}
                    else if(type == 5) {strcpy(message, "You scored an excellent hit on Undead"); g->players_score += 10;}
                    if(g->rooms[k].monsters[m].health <= 0) {
                        g->rooms[k].monsters[m].alive = 0;
                        if(g->rooms[k].monsters[m].type == 1) {strcpy(message, "You killed the Daemon!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 2) {strcpy(message, "You killed the Fire Breathing Monster!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 3) {strcpy(message, "You killed the Giant!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 4) {strcpy(message, "You killed the Snake!"); g->players_score += 20;}
                        else if(g->rooms[k].monsters[m].type == 5) {strcpy(message, "You killed the Undead!"); g->players_score += 20;}
                    }
                    shot = 1;
                }
            }
            if(shot) {break;}
            if(j == g->player_pos.y+range) {if(g->players_weapon == 2) {g->players_dagger -= 1; screen[i][j] = '~';} else if(g->players_weapon == 3) {g->players_magic_wand -= 1; screen[i][j] = ('/' | COLOR_PAIR(5));} else if(g->players_weapon == 4) {g->players_arrow -= 1; screen[i][j] = ('}' | COLOR_PAIR(7));}}
        }
    }
    else if(ch == '8') {
        for(int j=g->player_pos.y; j>=g->player_pos.y-range; j--) {
            int i=g->player_pos.x;
            char character = screen[i][j] & A_CHARTEXT;
            if(character == '-' || character == 'O' || character == '+') {if(g->players_weapon == 2) {g->players_dagger -= 1; screen[i][j+1] = '~';} else if(g->players_weapon == 3) {g->players_magic_wand -= 1; screen[i][j+1] = ('/' | COLOR_PAIR(5));} else if(g->players_weapon == 4) {g->players_arrow -= 1; screen[i][j+1] = ('}' | COLOR_PAIR(7));} break;}
            int k = RoomChecking(g->rooms, i, j);
            int shot = 0;
            for(int m=0; m<g->rooms[k].monsters_count; m++) {
                if(g->rooms[k].monsters[m].position.x == i && g->rooms[k].monsters[m].position.y == j && g->rooms[k].monsters[m].alive) {
                    if(g->players_weapon == 2) {g->players_dagger -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*12;}
                    else if(g->players_weapon == 3) {g->players_magic_wand -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*15;}
                    else if(g->players_weapon == 4) {g->players_arrow -= 1; g->rooms[k].monsters[m].health -= g->players_damage_rate*5;}
                    int type = CheckMonsters(g, i, j);
                    if(type == 1) {strcpy(message, "You scored an excellent hit on Daemon"); g->players_score += 5;}
                    else if(type == 2) {strcpy(message, "You scored an excellent hit on Fire Breathing Monster"); g->players_score += 5;}
                    else if(type == 3) {strcpy(message, "You scored an excellent hit on Giant"); g->players_score += 5;}
                    else if(type == 4) {strcpy(message, "You scored an excellent hit on Snake"); g->players_score += 10;}
                    else if(type == 5) {strcpy(message, "You scored an excellent hit on Undead"); g->players_score += 10;}
                    if(g->rooms[k].monsters[m].health <= 0) {
                        g->rooms[k].monsters[m].alive = 0;
                        if(g->rooms[k].monsters[m].type == 1) {strcpy(message, "You killed the Daemon!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 2) {strcpy(message, "You killed the Fire Breathing Monster!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 3) {strcpy(message, "You killed the Giant!"); g->players_score += 10;}
                        else if(g->rooms[k].monsters[m].type == 4) {strcpy(message, "You killed the Snake!"); g->players_score += 20;}
                        else if(g->rooms[k].monsters[m].type == 5) {strcpy(message, "You killed the Undead!"); g->players_score += 20;}
                    }
                    shot = 1;
                }
            }
            if(shot) {break;}
            if(j == g->player_pos.y-range) {if(g->players_weapon == 2) {g->players_dagger -= 1; screen[i][j] = '~';} else if(g->players_weapon == 3) {g->players_magic_wand -= 1; screen[i][j] = ('/' | COLOR_PAIR(5));} else if(g->players_weapon == 4) {g->players_arrow -= 1; screen[i][j] = ('}' | COLOR_PAIR(7));}}
        }
    }
    else {
        return;
    }
}

int CheckWeapons(Pos *weapons, int i, int j) {
    for(int w=0; w<3; w++) {
        if(weapons[w].x == i && weapons[w].y == j) {
            return 1;
        }
    }
    return 0;
}


//////////////////////////////////////////////////////// End Game /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// End Game /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// End Game /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// End Game /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// End Game /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// End Game /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// End Game /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// End Game /////////////////////////////////////////////////////////////////////////



void terminate_game(int code, User *p, Game *g) {
    time_t endtime = time(NULL);
    double playtime = difftime(endtime , g->start_time);

    p->score += g->players_score;
    p->gold += g->players_gold;
    p->playtime += playtime/60;
    FILE *users = fopen("users.txt", "r");
    FILE *temp_file = fopen("temp.txt", "w");
    char line[50];
    while(fgets(line, 50, users)) {
        line[strcspn(line,"\n")] = 0;
        if(strcmp(line,p->username) == 0) {
            fprintf(temp_file, "%s\n", line);
            fgets(line, 50, users); line[strcspn(line,"\n")] = 0; fprintf(temp_file, "%s\n", line);
            fgets(line, 50, users); line[strcspn(line,"\n")] = 0; fprintf(temp_file, "%s\n", line);
            fgets(line, 50, users); fprintf(temp_file, "%d\n", p->score);
            fgets(line, 50, users); fprintf(temp_file, "%d\n", p->gold);
            fgets(line, 50, users); fprintf(temp_file, "%d\n", p->count_games);
            fgets(line, 50, users); fprintf(temp_file, "%.2lf\n", p->playtime);
        }
        else {
            fprintf(temp_file, "%s\n", line);
        }
    }
    fclose(users);
    fclose(temp_file);
    remove("users.txt");
    rename("temp.txt","users.txt");
    if(code) {
        YOUWON(p);
    }
    else {
        YOULOST(p,g);
    } 
}

void YOUWON(User *p) {
    clear();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);

    int x_center = COLS / 2;
    int y_center = LINES / 2;

    clear();

        attron(COLOR_PAIR(5)); 
        mvprintw(y_center - 4, x_center - 35, "██    ██  ██████  ██    ██    ██    ████    ██  ██████  ███    ██");
        mvprintw(y_center - 3, x_center - 35, " ██  ██  ██    ██ ██    ██    ██    ████    ██ ██    ██ ████   ██");
        mvprintw(y_center - 2, x_center - 35, "  ████   ██    ██ ██    ██    ██    ████    ██ ██    ██ ██ ██  ██");
        mvprintw(y_center - 1, x_center - 35, "   ██    ██    ██ ██    ██    ██    ████    ██ ██    ██ ██  ██ ██");
        mvprintw(y_center,     x_center - 35, "   ██     ██████   ██████     ███████  ███████  ██████  ██   ████");
        attroff(COLOR_PAIR(5));
        refresh();

    const char *msg = "VICTORY";
    int msg_len = strlen(msg);
    for (int i = 0; i < msg_len; i++) {
        //attron(COLOR_PAIR((i % 3) + 3));
        mvprintw(y_center + 3, x_center - (msg_len / 2) + i -4, "%c🏆🔥🎉", msg[i]);
        //attroff(COLOR_PAIR((i % 3) + 3));
        refresh();
        usleep(100000);
    }

    attron(COLOR_PAIR(5));
    mvprintw(y_center + 7, x_center - 5, "[Q] Quit");
    attroff(COLOR_PAIR(5));

    refresh();

    while (1) {
        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            endwin();
            exit(0);
        }
    }

    endwin();
}

void YOULOST(User *p, Game *g) {
    clear();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);

    int x_center = COLS / 2;
    int y_center = LINES / 2;

    clear();

    for (int i = 0; i < 5; i++) {
        attron(COLOR_PAIR((i % 3) + 1));
        mvprintw(y_center - 4, x_center - 35, "  ██████   █████   ███    ███ ███████      ███████ ██    ██ ███████ ██████  ");
        mvprintw(y_center - 3, x_center - 35, " ██       ██   ██  ████  ████ ██           ██   ██ ██    ██ ██      ██   ██ ");
        mvprintw(y_center - 2, x_center - 35, " ██   ███ ███████  ██ ████ ██ █████        ██   ██ ██    ██ █████   ██████  ");
        mvprintw(y_center - 1, x_center - 35, " ██    ██ ██   ██  ██  ██  ██ ██           ██   ██ ██    ██ ██      ██   ██ ");
        mvprintw(y_center,     x_center - 35, "  ██████  ██   ██  ██      ██ ███████      ███████  ██████  ███████ ██   ██ ");
        attroff(COLOR_PAIR((i % 3) + 1));
        refresh();
        usleep(200000);
    }

    const char *msg = "You fought bravely, but perished...";
    int msg_len = strlen(msg);
    for (int i = 0; i < msg_len; i++) {
        attron(COLOR_PAIR((i % 3) + 3));
        mvprintw(y_center + 3, x_center - (msg_len / 2) + i, "%c", msg[i]);
        attroff(COLOR_PAIR((i % 3) + 3));
        refresh();
        usleep(100000);
    }

    attron(COLOR_PAIR(5));
    mvprintw(y_center + 7, x_center - 12, "[R] Retry    [Q] Quit");
    attroff(COLOR_PAIR(5));

    refresh();

    while (1) {
        int ch = getch();
        if (ch == 'q' || ch == 'Q') {
            endwin();
            exit(0);
        } else if (ch == 'r' || ch == 'R') {
            pre_game_menu(p,g);
        }
    }

    endwin();
}




void Pause_Screen(User *p, Game *g, chtype **screen, int **visited) {

    refresh();

    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK); 

    char *choices[] = { "Resume", "Save Game", "Exit Game" };
    int num_choices = sizeof(choices) / sizeof(char *);
    int choice = 0;

    while (1) {
        clear();

        for (int i = 0; i < num_choices; i++) {

            int row = (LINES - 4) / 2 + i * 2; 
            int col = (COLS - 20) / 2; 
            
            if (i == choice) {
                if (i == 1) {
                    attron(COLOR_PAIR(2) | A_REVERSE);
                } else if (i == 2) {
                    attron(COLOR_PAIR(3) | A_REVERSE);
                } else {
                    attron(COLOR_PAIR(1) | A_REVERSE);
                }
                mvprintw(row, col , choices[i]);
                attroff(A_REVERSE);
                if (i == 1) {
                    attroff(COLOR_PAIR(2));
                } else if (i == 2) {
                    attroff(COLOR_PAIR(3));
                } else {
                    attroff(COLOR_PAIR(1));
                }
            } else {
                mvprintw(row,col, choices[i]);
            }
        }

        refresh();
        int ch = getch();

        if (ch == KEY_UP && choice > 0) {
            choice--;
        } else if (ch == KEY_DOWN && choice < num_choices - 1) {
            choice++;
        } else if (ch == 10) {
            if (choice == 0) {

                return;
            } else if (choice == 1) {

                mvprintw((LINES - 4) / 2 + 8 ,(COLS - 20) / 2 -10 , "Game Saved! Press Any Key to Exit.");
                refresh();
                getch();

                    time_t endtime = time(NULL);
                    double playtime = difftime(endtime , g->start_time);
                    p->playtime += playtime/60;
                    FILE *users = fopen("users.txt", "r");
                    FILE *temp_file = fopen("temp.txt", "w");
                    char line[50];
                    while(fgets(line, 50, users)) {
                        line[strcspn(line,"\n")] = 0;
                        if(strcmp(line,p->username) == 0) {
                            fprintf(temp_file, "%s\n", line);
                            fgets(line, 50, users); line[strcspn(line,"\n")] = 0; fprintf(temp_file, "%s\n", line);
                            fgets(line, 50, users); line[strcspn(line,"\n")] = 0; fprintf(temp_file, "%s\n", line);
                            fgets(line, 50, users); fprintf(temp_file, "%d\n", p->score);
                            fgets(line, 50, users); fprintf(temp_file, "%d\n", p->gold);
                            fgets(line, 50, users); fprintf(temp_file, "%d\n", p->count_games);
                            fgets(line, 50, users); fprintf(temp_file, "%.2lf\n", p->playtime);
                        }
                        else {
                            fprintf(temp_file, "%s\n", line);
                        }
                    }
                    fclose(users);
                    fclose(temp_file);
                    remove("users.txt");
                    rename("temp.txt","users.txt");

                SAVEGAME(p, g, screen, visited);
                endwin();
                exit(0);

            } else if (choice == 2) {
                refresh();

                    time_t endtime = time(NULL);
                    double playtime = difftime(endtime , g->start_time);
                    p->playtime += playtime/60;
                    FILE *users = fopen("users.txt", "r");
                    FILE *temp_file = fopen("temp.txt", "w");
                    char line[50];
                    while(fgets(line, 50, users)) {
                        line[strcspn(line,"\n")] = 0;
                        if(strcmp(line,p->username) == 0) {
                            fprintf(temp_file, "%s\n", line);
                            fgets(line, 50, users); line[strcspn(line,"\n")] = 0; fprintf(temp_file, "%s\n", line);
                            fgets(line, 50, users); line[strcspn(line,"\n")] = 0; fprintf(temp_file, "%s\n", line);
                            fgets(line, 50, users); fprintf(temp_file, "%d\n", p->score);
                            fgets(line, 50, users); fprintf(temp_file, "%d\n", p->gold);
                            fgets(line, 50, users); fprintf(temp_file, "%d\n", p->count_games);
                            fgets(line, 50, users); fprintf(temp_file, "%.2lf\n", p->playtime);
                        }
                        else {
                            fprintf(temp_file, "%s\n", line);
                        }
                    }
                    fclose(users);
                    fclose(temp_file);
                    remove("users.txt");
                    
                endwin();
                exit(0);
            }
        }
    }
}


void not_saved_screen(User *p, Game *g) {
    clear();

    mvprintw(LINES/2,COLS/2-14,"There Is No Saved File!");
    mvprintw(LINES/2+2,COLS/2-14,"Press any key to RETURN");

    int ch = getch();
    pre_game_menu(p,g);                
}



//////////////////////////////////////////////////////// SAVE GAME /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// SAVE GAME /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// SAVE GAME /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// SAVE GAME /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// SAVE GAME /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// SAVE GAME /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// SAVE GAME /////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// SAVE GAME /////////////////////////////////////////////////////////////////////////


void SAVEGAME(User *p, Game *g, chtype **screen, int **visited) {
    FILE *saved_game = fopen(txt_format(p->username),"w");
    if(saved_game) {
        fprintf(saved_game, "%d\n%d\n%d\n%d\n", g->difficulty, g->player_color, g->players_health, g->players_extra_health);
        fprintf(saved_game, "%d\n%d\n", g->players_score, g->players_gold);
        fprintf(saved_game, "%d\n", g->floor_number);
        fprintf(saved_game, "%d\n%d\n", g->players_hunger, g->players_ordinary_food);
        fprintf(saved_game, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n", g->players_weapon, g->players_weapon_direction, g->players_mace, g->players_dagger, g->players_magic_wand, g->players_arrow, g->players_sword);
        fprintf(saved_game, "%d\n%d\n%d\n%d\n", g->players_health_potion, g->players_speed_potion, g->players_speed, g->players_damage_potion);
        fprintf(saved_game, "%d\n%d\n%d\n%d\n", g->players_speed_step, g->players_health_step, g->players_damage_step, g->players_damage_rate);
        fprintf(saved_game, "%d\n%d\n", g->player_pos.x, g->player_pos.y);
        fprintf(saved_game, "%d\n%d\n", g->players_steps, g->k_lock);

        for(int k=0; k<6; k++) {
            fprintf(saved_game, "%d\n", g->rooms[k].type);
            fprintf(saved_game, "%d\n", g->rooms[k].locked);
            fprintf(saved_game, "%d\n", g->rooms[k].password);
            fprintf(saved_game, "%d\n%d\n", g->rooms[k].room_pos.x, g->rooms[k].room_pos.y);
            fprintf(saved_game, "%d\n%d\n", g->rooms[k].room_size_v, g->rooms[k].room_size_h);
            fprintf(saved_game, "%d\n%d\n", g->rooms[k].gold, g->rooms[k].dark_gold);
            fprintf(saved_game, "%d\n%d\n", g->rooms[k].ordinary_food, g->rooms[k].potion);
        }
        
        for(int j=1; j<LINES; j++) {
            for(int i=0; i<COLS; i++) {
                fprintf(saved_game, "%c", screen[i][j]);
            }
            fprintf(saved_game, "\n");
        }
        for(int j=1; j<LINES; j++) {
            for(int i=0; i<COLS; i++) {
                fprintf(saved_game, "%d", visited[i][j]);
            }
            fprintf(saved_game, "\n");
        }
        fclose(saved_game);
    }
}

void SaveLauncher(User *p, Game *g) {
    clear();
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, 208, COLOR_BLACK); //Orange
    init_pair(8, 8, COLOR_BLACK); //Gray 

    FILE *saved_game = fopen(txt_format(p->username),"r");
    char num[10];
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->difficulty = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->player_color = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_health = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_extra_health = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_score = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_gold = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->floor_number = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_hunger = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_ordinary_food = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_weapon = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_weapon_direction = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_mace = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_dagger = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_magic_wand = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_arrow = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_sword = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_health_potion = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_speed_potion = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_speed = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_damage_potion = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_speed_step = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_health_step = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_damage_step = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_damage_rate = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->player_pos.x = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->player_pos.y = str_to_num(num);
    fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->players_steps = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->k_lock= str_to_num(num);
    for(int k=0; k<6; k++) {
        fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].type = str_to_num(num);
        fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].locked = str_to_num(num);
        fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].password = str_to_num(num);
        fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].room_pos.x = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].room_pos.y = str_to_num(num);
        fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].room_size_v = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].room_size_h = str_to_num(num);
        fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].gold = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].dark_gold= str_to_num(num);
        fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].ordinary_food = str_to_num(num); fgets(num, 10, saved_game); num[strcspn(num,"\n")] = 0; g->rooms[k].potion = str_to_num(num);
    }

    chtype **screen = (chtype **) malloc(COLS*sizeof(chtype*));
    for(int i=0; i<COLS; i++) {
        *(screen+i) = (chtype *) malloc(LINES*sizeof(chtype));
    }

    int **visited = (int **) malloc(COLS*sizeof(int *));
    for(int i=0; i<COLS; i++) {
        *(visited+i) = (int *) calloc(LINES,sizeof(int));
    }

    for(int j=1; j<LINES; j++) {
        for(int i=0; i<COLS; i++) {
            screen[i][j] = fgetc(saved_game);
        }
        fgetc(saved_game);
    }
    for(int j=1; j<LINES; j++) {
        for(int i=0; i<COLS; i++) {
            char ch;
            visited[i][j] = fgetc(saved_game)-48;
        }
        fgetc(saved_game);
    }

    fclose(saved_game);

    if(g->difficulty) {
        g->MAX_health = 5;
        g->hungriness_rate = 2;
    }
    else {
        g->MAX_health = 10;
        g->hungriness_rate = 1;
    }

    g->start_time = time(NULL);
    g->password_start_time = time(NULL);

    SaveFloor(p, g, screen, visited);
    
    clear();
}

void SaveFloor(User *p, Game *g, chtype **screen, int **visited) {
    srand(time(NULL));
    clear();

    for(int k=0; k<6; k++) {
        g->rooms[k].type = 0;
        g->rooms[k].locked = 0;
    }

    if(g->floor_number == 1) {
        strcpy(message, "Wellcome to the 1st floor!");
        map(g,6);
        for(int k=0; k<5; k++) {
            g->rooms[k].monsters_count = rand() % 2;
            for(int i=0; i<g->rooms[k].monsters_count; i++) {
                g->rooms[k].monsters[i].type = 1+rand()%4; g->rooms[k].monsters[i].alive = 1; g->rooms[k].monsters[i].on = 0; g->rooms[k].monsters[i].room = k;
                if(g->rooms[k].monsters[i].type == 1) {
                    g->rooms[k].monsters[i].radius = 1; g->rooms[k].monsters[i].health = 5; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 0;}
                else if(g->rooms[k].monsters[i].type == 2) {
                    g->rooms[k].monsters[i].radius = 3; g->rooms[k].monsters[i].health = 10; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 3;}
                else if(g->rooms[k].monsters[i].type == 3) {
                    g->rooms[k].monsters[i].radius = 4; g->rooms[k].monsters[i].health = 15; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 5;}
                else if(g->rooms[k].monsters[i].type == 4) {
                    g->rooms[k].monsters[i].radius = 5; g->rooms[k].monsters[i].health = 20; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 100;}
            }
        }
    }
    else if(g->floor_number == 2) {
        strcpy(message, "You're now in the 2nd floor!");
        map(g,RoomChecking(g->rooms,g->player_pos.x,g->player_pos.y));
        for(int k=0; k<6; k++) {
            g->rooms[k].monsters_count = 1 + rand() % 2;
            for(int i=0; i<g->rooms[k].monsters_count; i++) {
                g->rooms[k].monsters[i].type = 1+rand()%4; g->rooms[k].monsters[i].alive = 1; g->rooms[k].monsters[i].on = 0; g->rooms[k].monsters[i].room = k;
                if(g->rooms[k].monsters[i].type == 1) {g->rooms[k].monsters[i].radius = 1; g->rooms[k].monsters[i].health = 5; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 0;}
                else if(g->rooms[k].monsters[i].type == 2) {g->rooms[k].monsters[i].radius = 3; g->rooms[k].monsters[i].health = 10; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 3;}
                else if(g->rooms[k].monsters[i].type == 3) {g->rooms[k].monsters[i].radius = 4; g->rooms[k].monsters[i].health = 15; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 5;}
                else if(g->rooms[k].monsters[i].type == 4) {g->rooms[k].monsters[i].radius = 5; g->rooms[k].monsters[i].health = 20; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 100;}
            }
        }
    }
    else if(g->floor_number == 3) {
        strcpy(message, "You're now in the 3rd floor!");
        map(g,RoomChecking(g->rooms,g->player_pos.x,g->player_pos.y));
        for(int k=0; k<6; k++) {
            g->rooms[k].monsters_count = 1+rand()%2;
            for(int i=0; i<g->rooms[k].monsters_count; i++) {
                g->rooms[k].monsters[i].type = 1+rand()%4; g->rooms[k].monsters[i].alive = 1; g->rooms[k].monsters[i].on = 0; g->rooms[k].monsters[i].room = k;
                if(g->rooms[k].monsters[i].type == 1) {g->rooms[k].monsters[i].radius = 1; g->rooms[k].monsters[i].health = 5; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 0;}
                else if(g->rooms[k].monsters[i].type == 2) {g->rooms[k].monsters[i].radius = 3; g->rooms[k].monsters[i].health = 10; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 3;}
                else if(g->rooms[k].monsters[i].type == 3) {g->rooms[k].monsters[i].radius = 4; g->rooms[k].monsters[i].health = 15; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 5;}
                else if(g->rooms[k].monsters[i].type == 4) {g->rooms[k].monsters[i].radius = 5; g->rooms[k].monsters[i].health = 20; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 100;}
            }
        }
    }
    else if(g->floor_number == 4) {
        strcpy(message, "You're now in the last floor!");
        map(g,RoomChecking(g->rooms,g->player_pos.x,g->player_pos.y));
        int count_treasure_rooms = 0; double avg_size = 0;
        for(int k=0; k<6; k++) { avg_size += (g->rooms[k].room_size_h+g->rooms[k].room_size_v); }
        avg_size /= 6;

        for(int k=0; k<6; k++) {
            if(g->rooms[k].type == 1) {
                g->rooms[k].monsters_count = 10;
            }
            else {
                g->rooms[k].monsters_count = 2+rand()%2;
                for(int i=0; i<g->rooms[k].monsters_count; i++) {
                    g->rooms[k].monsters[i].type = 1+rand()%5; g->rooms[k].monsters[i].alive = 1; g->rooms[k].monsters[i].on = 0; g->rooms[k].monsters[i].room = k;
                    if(g->rooms[k].monsters[i].type == 1) {g->rooms[k].monsters[i].radius = 1; g->rooms[k].monsters[i].health = 5; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 0;}
                    else if(g->rooms[k].monsters[i].type == 2) {g->rooms[k].monsters[i].radius = 3; g->rooms[k].monsters[i].health = 10; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 3;}
                    else if(g->rooms[k].monsters[i].type == 3) {g->rooms[k].monsters[i].radius = 4; g->rooms[k].monsters[i].health = 15; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 5;}
                    else if(g->rooms[k].monsters[i].type == 4) {g->rooms[k].monsters[i].radius = 5; g->rooms[k].monsters[i].health = 20; g->rooms[k].monsters[i].damage = 1; g->rooms[k].monsters[i].haunt = 100;}
                    else if(g->rooms[k].monsters[i].type == 5) {g->rooms[k].monsters[i].radius = 10; g->rooms[k].monsters[i].health = 30; g->rooms[k].monsters[i].damage = 2; g->rooms[k].monsters[i].haunt = 8;}
                }
            }
        }
    }

    for(int k=0; k<6; k++) {
            g->rooms[k].traps_count = 1+rand()%3;
    }

    g->rooms[g->k_lock].password = password();  int k_secret_door_1 = rand()%6; int k_secret_door_2 = rand()%6; g->secret_doors_count = 0;
    for(int k=0; k<6; k++) {
        int trap = 0; int secret_door = 0;
        int monster = 0;
        for(int j=1; j<LINES; j++) {
            for(int i=0; i<COLS; i++) {
                if(abs(i-g->rooms[k].room_pos.x) < g->rooms[k].room_size_h && abs(j-g->rooms[k].room_pos.y) < g->rooms[k].room_size_v) {
                    if(g->rooms[k].type == 1) {
                        if(rand()%10 == 1 && (mvinch(j,i) & A_CHARTEXT) == '.' && trap<g->rooms[k].traps_count) {
                            g->rooms[k].traps[trap].x = i; g->rooms[k].traps[trap].y = j;
                            trap++;
                        }
                    }
                    else {
                        if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && trap<g->rooms[k].traps_count) {
                            g->rooms[k].traps[trap].x = i; g->rooms[k].traps[trap].y = j;
                            trap++;
                        }
                    }
                    
                    if(g->floor_number != 4) {
                        if(k == k_secret_door_1 || k == k_secret_door_2) {
                            if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && secret_door<1) {
                                g->secret_doors[g->secret_doors_count].x = i; g->secret_doors[g->secret_doors_count].y = j;
                                secret_door++;
                                g->secret_doors_count++;
                            }
                        }  
                    }
                    
                    if(rand()%((g->rooms[k].room_size_h*g->rooms[k].room_size_v)) == 0 && (mvinch(j,i) & A_CHARTEXT) == '.' && monster<g->rooms[k].monsters_count) {
                        g->rooms[k].monsters[monster].position.x = i; g->rooms[k].monsters[monster].position.y = j;
                        monster++;
                    }
                }
            }
        }
    }

    int display_whole = 0;
    ShowScreen(g,"hidden",visited,screen);
    while(1) {
        MessageBar(g, g->floor_number, g->players_score, g->players_gold);
        HEALTH_BAR(g);
        Food_Bar(g);
        time_t current_time = time(NULL);
        double elapsed_time = difftime(current_time, g->start_time);
        double password_time = difftime(current_time, g->password_start_time);

        if(elapsed_time > 30) {
            if(g->players_hunger == 10 && g->players_health < 9) {
                g->players_health += 1;
            }
            if(g->players_hunger > 0 ) {
                g->players_hunger -= g->hungriness_rate;
            }
            g->start_time = time(NULL);
            if(g->players_hunger <= 5) {
                g->players_health -= (10 - g->players_hunger)/5;
            }
        }
        if(g->players_steps - g->players_speed_step >= 15) {g->players_speed = 1; g->players_speed_step = -15;}
        if(g->players_steps - g->players_health_step >= 20) {g->players_extra_health = 0; g->players_health_step = -20;}
        if(g->players_steps - g->players_damage_step >= 5) {g->players_damage_rate = 1; g->players_damage_step = -5;}
        if(g->players_health + g->players_extra_health <= 0) {
            terminate_game(0, p, g);
        }
        if(display_whole%2 == 1) {
            ShowScreen(g,"view",visited,screen);
        }
        else {
            ShowScreen(g,"hidden",visited,screen);
        }
        attron(COLOR_PAIR(g->player_color));
        mvprintw(g->player_pos.y, g->player_pos.x, "@");
        attroff(COLOR_PAIR(g->player_color));

        int ch = getch();
        if(ch == 'q') {
            if ( !p->guest ){
            Pause_Screen(p,g,screen,visited);
            }
            else{
                pre_game_menu(p,g);
            }
        }
        else if(ch == 'm') {
            display_whole++;
            continue;
        }
        else if(ch == 'f') {
            FoodTab(g);
        }
        else if(ch == 'i') {
            WeaponTab(g);
        }
        else if(ch == 'p') {
            SpellTab(g);
        }
        else if ( ch == 'k'){
            show_key_guide();
        }

        handle_monsters(g, screen);
        Monsters_Movement(g, screen);

        Active_Weapons(g, ch, screen);

        HEALTH_BAR(g);

        for(int i = g->player_pos.x-1; i <= g->player_pos.x+1; i++) {
            for(int j = g->player_pos.y-1; j <= g->player_pos.y+1; j++) {
                if(ch == 's') {
                    if(CheckTrap(g->rooms,i,j)) {
                        screen[i][j] = '^';
                    }
                }
                if(CheckEnchantDoor(g->secret_doors_count,g->secret_doors,i,j)) {
                    screen[i][j] = '?';
                }
            }
        }
        switch(Movement(screen, visited, ch, g,p)) {
            case 1:
                g->floor_number += 1;
                FloorGenerator(p,g);
                break;
            case 2:
                if(rand()%10 == 9) {
                    g->rooms[g->k_lock].password = password();
                }
                Show_Password(g->rooms[g->k_lock].password);
                break;
            case 3:
                password_screen(g);
                break;
            case 4:
                g->rooms[0].room_pos.x = COLS/2; g->rooms[0].room_pos.y = LINES/2; g->rooms[0].room_size_v = 12; g->rooms[0].room_size_h = 35;
                for(int i=0; i<6; i++) {g->rooms[i].monsters_count = 0;}
                break;
            case 5:
                Pos saved_pos; saved_pos.x = g->player_pos.x; saved_pos.y = g->player_pos.y; int saved_secret_doors_count = g->secret_doors_count; Pos saved_first_door; saved_first_door.x = g->secret_doors[0].x; saved_first_door.y = g->secret_doors[0].y;
                int monsters_count[6]; for(int i=0; i<6; i++) { monsters_count[i] = g->rooms[i].monsters_count; g->rooms[i].monsters_count = 0;}
                g->enchant_start_time = time(NULL);
                EnchantRoom(p, g);
                g->player_pos.x = saved_pos.x; g->player_pos.y = saved_pos.y; g->secret_doors_count = saved_secret_doors_count; g->secret_doors[0].x = saved_first_door.x; g->secret_doors[0].y = saved_first_door.y;
                for(int i=0; i<6; i++) { g->rooms[i].monsters_count = monsters_count[i];}
                break;
            case 6:
                saved_pos.x = g->player_pos.x; saved_pos.y = g->player_pos.y; Pos room_0_pos; room_0_pos.x = g->rooms[0].room_pos.x; room_0_pos.y = g->rooms[0].room_pos.y;
                int size_v = g->rooms[0].room_size_v; int size_h = g->rooms[0].room_size_h;
                g->rooms[0].room_pos.x = COLS/2; g->rooms[0].room_pos.y = LINES/2; g->rooms[0].room_size_v = 12; g->rooms[0].room_size_h = 35;
                for(int i=0; i<6; i++) { monsters_count[i] = g->rooms[i].monsters_count; g->rooms[i].monsters_count = 0;}
                g->player_pos.x = saved_pos.x; g->player_pos.y = saved_pos.y;
                g->rooms[0].room_pos.x = room_0_pos.x; g->rooms[0].room_pos.y = room_0_pos.y; g->rooms[0].room_size_v = size_v; g->rooms[0].room_size_h = size_h;
                for(int i=0; i<6; i++) { g->rooms[i].monsters_count = monsters_count[i];}
                break;
            case 7:
                 saved_pos; saved_pos.x = g->player_pos.x; saved_pos.y = g->player_pos.y;
                 monsters_count[6]; for(int i=0; i<6; i++) { monsters_count[i] = g->rooms[i].monsters_count; g->rooms[i].monsters_count = 0;}
                g->treasure_start_time = time(NULL);
                TreasureRoom(p, g);
                g->player_pos.x = saved_pos.x; g->player_pos.y = saved_pos.y;
                for(int i=0; i<6; i++) { g->rooms[i].monsters_count = monsters_count[i];}
                break;                             
        }
    }
}