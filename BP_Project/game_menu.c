#include "libs.h"
#include "player.h"
#include "funcs.h"
#include "map.h"
#include "auth.h"
#include "game_menu.h"

void pre_game_menu(User *p, Game *g) {
    
    char *choices[] = {"New Game", "Resume Game", "Leaderboard", "Settings", "Back"};
    int highlight = 0, choice, row, col;

    curs_set(0);

    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        init_pair(1, COLOR_MAGENTA,-1);
        attron(COLOR_PAIR(1));
        
        int start_x = COLS / 2 - 35;
        int start_y = LINES / 2 - 12;

        mvprintw(start_y, start_x, "      ___           ___           ___           ___           ___     ");
        mvprintw(start_y + 1, start_x, "     /  /\\         /  /\\         /  /\\         /__/\\         /  /\\    ");
        mvprintw(start_y + 2, start_x, "    /  /::\\       /  /::\\       /  /:/_        \\  \\:\\       /  /:/_   ");
        mvprintw(start_y + 3, start_x, "   /  /:/\\:\\     /  /:/\\:\\     /  /:/ /\\        \\  \\:\\     /  /:/ /\\  ");
        mvprintw(start_y + 4, start_x, "  /  /:/~/:/    /  /:/  \\:\\   /  /:/_/::\\   ___  \\  \\:\\   /  /:/ /:/_ ");
        mvprintw(start_y + 5, start_x, " /__/:/ /:/___ /__/:/ \\__\\:\\ /__/:/__\\/\\:\\ /__/\\  \\__\\:\\ /__/:/ /:/ /\\");
        mvprintw(start_y + 6, start_x, " \\  \\:\\/:::::/ \\  \\:\\ /  /:/ \\  \\:\\ /~~/:/ \\  \\:\\ /  /:/ \\  \\:\\/:/ /:/");
        mvprintw(start_y + 7, start_x, "  \\  \\::/~~~~   \\  \\:\\  /:/   \\  \\:\\  /:/   \\  \\:\\  /:/   \\  \\::/ /:/ ");
        mvprintw(start_y + 8, start_x, "   \\  \\:\\        \\  \\:\\/:/     \\  \\:\\/:/     \\  \\:\\/:/     \\  \\:\\/:/  ");
        mvprintw(start_y + 9, start_x, "    \\  \\:\\        \\  \\::/       \\  \\::/       \\  \\::/       \\  \\::/   ");
        mvprintw(start_y + 10, start_x, "     \\__/          \\__/          \\__/          \\__/          \\__/    ");

        refresh();

        attroff(COLOR_PAIR(1));
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        for (int i = 0; i < 5; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 + i * 2 + 4, (col - 20) / 2, choices[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight + 4) % 5;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 5;
                break;
            case '\n':
                if (highlight == 0) {  // New Game
                    start_new_game(p,g);
                } else if (highlight == 1) {  // Continue Previous Game
                            FILE *last_saved_game = fopen(txt_format(p->username),"r");
                            if(last_saved_game) {
                                continue_previous_game(p,g);
                                }
                            else {
                                 not_saved_screen(p,g);
                                 }
                } else if (highlight == 2) {  // Leaderboard
                    display_leaderboard(p,g);
                } else if (highlight == 3) {  // Settings
                    settings_menu(p,g);
                } else if (highlight == 4) {  // Back
                    draw_menu(p,g);
                }
                break;
            case 27:  // ESC key
                draw_menu(p,g);
            default:
                break;
        }
    }
}


void start_new_game(User *p , Game *g) {
    int row, col;
    clear();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 2, (col - 25) / 2, "Starting a new game...");
    mvprintw(row / 2, (col - 30) / 2, "Press any key to continue...");
    getch();
    GameLauncher(p,g);
}

void continue_previous_game(User *p , Game *g){
    int row, col;
    clear();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 2, (col - 25) / 2, "Loading previous game...");
    mvprintw(row / 2, (col - 30) / 2, "Press any key to continue...");
    getch();
    SaveLauncher(p,g);
    
}
                                                                    
void display_leaderboard(User *p , Game *g){
    clear();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(4, 208, COLOR_BLACK);
    int row, col;
    getmaxyx(stdscr, row, col);
    mvprintw(1, 1, "LEADERBOARD");
    mvprintw(row - 2, 2, "Press any key to go back");
    mvprintw(3, 1, "RANK"); mvprintw(3, 10, "username"); mvprintw(3, 24, "score"); mvprintw(3, 35, "gold"); mvprintw(3, 44, "gamecounts"); mvprintw(3, 58, "playtime");

    attron(COLOR_PAIR(1));
    int start_col = col - 40;
    mvprintw(3, start_col,  "⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⡄⢠⣤⣤⣤⣤⣤⣤⡄⢠⣤⡄⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(4, start_col,  "⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⢸⣿⣿⣿⣿⣿⣿⡇⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(5, start_col,  "⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⢸⣿⣿⣿⣿⣿⣿⡇⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(6, start_col,  "⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⢸⣿⣿⣿⣿⣿⣿⡇⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(7, start_col,  "⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⢸⣿⣿⣿⣿⣿⣿⡇⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(8, start_col,  "⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⢸⣿⣿⣿⣿⣿⣿⡇⢸⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(9, start_col,  "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠃⠘⠛⠛⠛⠛⠛⠛⠃⠘⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(10, start_col, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠛⠛⠛⠛⠛⠛⠛⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(11, start_col, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣶⣶⠂⣰⣶⣶⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(12, start_col, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⡏⢠⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(13, start_col, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⡈⠻⣿⣿⣶⣾⣿⣿⣿⣿⠿⠋⡀⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(14, start_col, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣶⣄⠙⣿⣿⣿⣿⣟⢁⣴⣾⡇⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(15, start_col, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⡏⢠⣿⠟⠛⢿⣿⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(16, start_col, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠀⠀⣠⣶⣷⣤⡈⠛⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    mvprintw(17, start_col, "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠛⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀");
    attroff(COLOR_PAIR(1));

    char usernames[100][50]; 
    int scores[100]; 
    int golds[100]; 
    int game_counts[100]; 
    char exps[100][10] = {0};
    int arr[] = {0,1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19,20};
    int *ptr_arr = arr;

    FILE *fptr;
    fptr = fopen("users.txt","r");
    char line[50]; 
    int i = 0; 
    int count = 0;
    while(fgets(line, 50, fptr) != NULL) {
        line[strcspn(line,"\n")] = '\0';
        if(i % 7 == 0) {
            if(i != 0) {
                count++;
            }
            strcpy(usernames[count], line);
        }
        else if(i % 7 == 3) {
            scores[count] = str_to_num(line);
        }
        else if(i % 7 == 4) {
            golds[count] = str_to_num(line);
        }
        else if(i % 7 == 5) {
            game_counts[count] = str_to_num(line);
        }
        else if (i % 7 == 6) {
            for ( int i = 0 ; i < 10 ; i++){
                exps[count][i] = line[i];
            }
        }
        i++;
    }
    fclose(fptr);

    sort(ptr_arr, scores, (count % 10) + 1);

    for (int i = 0; i <= count; i++) {

    if (i == 0) {
        attron(COLOR_PAIR(3));
        mvprintw(5 + (i * 4), 69, "G.O.A.T 🐐");
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(4));
    }
    else if (i == 1) {
        attron(COLOR_PAIR(3));
        mvprintw(5 + (i * 4), 69, "LEGEND 💀");
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(4));
    }
    else if (i == 2) {
        attron(COLOR_PAIR(3));
        mvprintw(5 + (i * 4), 69, "BEST 💪");
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(4));
    }
    else {
        attroff(COLOR_PAIR(4));
    }

    if (strcmp(p->username, usernames[arr[i]]) == 0) {
        mvprintw(5 + (i * 4), 1, "🤙 %d", i + 1); 
        mvprintw(5 + (i * 4), 10, "%s", usernames[arr[i]]); 
        mvprintw(5 + (i * 4), 24, "%d", scores[arr[i]]); 
        mvprintw(5 + (i * 4), 35, "%d", golds[arr[i]]); 
        mvprintw(5 + (i * 4), 44, "%d", game_counts[arr[i]]); 
        mvprintw(5 + (i * 4), 58, "%s M", exps[arr[i]]);
    }
    else {

        mvprintw(5 + (i * 4), 1, "%d", i + 1); 
        mvprintw(5 + (i * 4), 10, "%s", usernames[arr[i]]); 
        mvprintw(5 + (i * 4), 24, "%d", scores[arr[i]]); 
        mvprintw(5 + (i * 4), 35, "%d", golds[arr[i]]); 
        mvprintw(5 + (i * 4), 44, "%d", game_counts[arr[i]]); 
        mvprintw(5 + (i * 4), 58, "%s M", exps[arr[i]]);
    }

    mvprintw(5 + (i * 4) + 1, 1, "");
    mvprintw(5 + (i * 4) + 2, 1, "");
    mvprintw(5 + (i * 4) + 3, 1, "");
    }
    char ch = getch();
    clear();
}

void change_difficulty(User *p, Game *g) {
    char *difficulties[] = {"Easy", "Hard"};
    int highlight = 0, choice, row, col;

    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 20) / 2, "Select Difficulty");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        for (int i = 0; i < 2; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 15) / 2, difficulties[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight + 1) % 2;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 2;
                break;
            case '\n':
                g->difficulty = highlight;
                mvprintw(row / 2 + 4, (col - 30) / 2, "Difficulty set to %s!", difficulties[highlight]);
                getch();
                return;
            case 27:
                return;
            default:
                break;
        }
    }
}


void change_character_color(User *p, Game *g) {
    char *colors[] = {"White", "Blue", "Red", "Green"};
    int highlight = 0, choice, row, col;

    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 25) / 2, "Select Character Color");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        for (int i = 0; i < 4; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 20) / 2, colors[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight + 3) % 4;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 4;
                break;
            case '\n':
                g->player_color = highlight;
                mvprintw(row / 2 + 4, (col - 30) / 2, "Character color set to %s!", colors[highlight]);
                getch();
                return;
            case 27:
                return;
            default:
                break;
        }
    }
}


void select_music() {
    char *tracks[] = {"Track 1", "Track 2", "Track 3", "No Music"};
    int highlight = 0, choice, row, col;
    Mix_Music *music = NULL;

    if (SDL_Init(SDL_INIT_AUDIO) < 0){
        mvprintw(0, 0, "SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return;
    }

    if (Mix_Init(MIX_INIT_MP3) == 0) {
        mvprintw(0, 0, "Mix_Init failed: %s", Mix_GetError());
        return;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        mvprintw(0, 0, "SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        return;
    }

    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 20) / 2, "Select Music");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        for (int i = 0; i < 4; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 15) / 2, tracks[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight + 3) % 4;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 4;
                break;
            case '\n':
                mvprintw(row / 2 + 4, (col - 30) / 2, "Music set to %s!", tracks[highlight]);

                if (Mix_PlayingMusic()){
                    Mix_HaltMusic();
                    Mix_FreeMusic(music);
                }

                if (highlight == 0) {
                    music = Mix_LoadMUS("/home/matin/Desktop/fop2024-project-MatinFadavi0/BP_Project/music/track1.mp3");
                } else if (highlight == 1){
                    music = Mix_LoadMUS("/home/matin/Desktop/fop2024-project-MatinFadavi0/BP_Project/music/track2.mp3");
                } else if (highlight == 2) {
                    music = Mix_LoadMUS("/home/matin/Desktop/fop2024-project-MatinFadavi0/BP_Project/music/track3.mp3");
                } else {
                    music = NULL;
                }

                if (music != NULL)
                {
                    Mix_PlayMusic(music, -1);
                }

                getch();
                return;
            case 27:
                if (Mix_PlayingMusic()){

                    Mix_HaltMusic();
                    Mix_FreeMusic(music);
                }
                return;
            default:
                break;
        }
    }

    Mix_CloseAudio();
    Mix_Quit();
    SDL_Quit();
}



void settings_menu(User *p, Game *g) {
    char *settings[] = {"Change Difficulty", "Change Character Color", "Select Music", "Back"};
    int highlight = 0, choice, row, col;
    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 15) / 2, "Settings");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        for (int i = 0; i < 4; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 20) / 2, settings[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight + 3) % 4;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 4;
                break;
            case '\n':
                if (highlight == 0)
                    change_difficulty(p,g);
                else if (highlight == 1)
                    change_character_color(p,g);
                else if (highlight == 2)
                    select_music();
                else
                    return;
                break;
            case 27: 
                return;
            default:
                break;
        }
    }
}

void GameLauncher(User *p, Game *g) {
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

    if(g->difficulty) {
        g->MAX_health = 5;
        g->hungriness_rate = 2;
    }
    else {
        g->MAX_health = 10;
        g->hungriness_rate = 1;
    }
    g->players_health = g->MAX_health;
    g->players_extra_health = 0;
    g->players_score = 0;
    g->players_gold = 0;
    g->floor_number = 1;
    g->players_hunger = 10;
    g->players_ordinary_food = 0;
    g->players_weapon = -1;
    g->players_weapon_direction = 5;
    g->players_mace = 1;
    g->players_dagger = 0;
    g->players_magic_wand = 0;
    g->players_arrow = 0;
    g->players_sword = 0;
    g->players_health_potion = 0;
    g->players_speed_potion = 0;
    g->players_speed = 1;
    g->players_damage_potion = 0;
    g->start_time = time(NULL);
    g->players_steps = 0;
    g->players_speed_step = -15;
    g->players_health_step = -20;
    g->players_damage_step = -5;
    g->players_damage_rate = 1;

    p->count_games++;

    FloorGenerator(p, g);
    

    clear();
}