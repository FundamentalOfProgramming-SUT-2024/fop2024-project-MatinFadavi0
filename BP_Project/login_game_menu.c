#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "header.h"


// void load_users(User *p) {
//     FILE *file = fopen("users.txt", "r");
//     if (file != NULL) {
//         while (fscanf(file, "%s\n%s\n%s\n", p->username, p->password, p->email) != EOF) {
//         }
//         fclose(file);
//     }
// }

void save_users(User *p) {
    FILE *file = fopen("users.txt", "a");

    if (file == NULL) {
        printf("Unable to open the file!\n");
        return;
    }
    p->score = 0;
    p->gold = 0;
    p->count_games = 0;
    fprintf(file, "%s\n%s\n%s\n%d\n%d\n%d\n", p->username, p->password, p->email, p->score, p->gold, p->count_games);

    fclose(file);
}

int validate_email(const char *email) {
    const char *at = strchr(email, '@');
    if (!at || at == email || at[1] == '\0') return 0;
    const char *dot = strchr(at, '.');
    return dot && dot > at + 1 && dot[1] != '\0';
}

int valid_password(const char *password) {
    if (strlen(password) < 7) {
        return 0;  // Password is too short
    }
    int has_upper = 0, has_lower = 0, has_digit = 0;
    for (int i = 0; password[i] != '\0'; i++) {
        if (isupper(password[i])) has_upper = 1;
        else if (islower(password[i])) has_lower = 1;
        else if (isdigit(password[i])) has_digit = 1;
    }
    return has_upper && has_lower && has_digit;
}


void forgot_password(User *p) {
    char email[MAX_LENGTH] = "";
    int row, col;
    int found = 0;

    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 15) / 2, "Forgot Password");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        mvprintw(row / 2 - 4, (col - 20) / 2, "Email");
        move(row / 2 - 4, col / 2);
        echo();
        getstr(email);
        noecho();


        FILE *file = fopen("users.txt", "r");
        if (file == NULL) {
            mvprintw(row / 2 + 4, (col - 30) / 2, "Unable to open users file!");
            getch();
            return;
        }

        while (fgets(p->username, MAX_LENGTH, file) != NULL) {

            p->username[strcspn(p->username, "\n")] = '\0';


            if (fgets(p->password, MAX_LENGTH, file) != NULL) {
                p->password[strcspn(p->password, "\n")] = '\0';

                if (fgets(p->email, MAX_LENGTH, file) != NULL) {
                    p->email[strcspn(p->email, "\n")] = '\0';

                    if (strcmp(p->email, email) == 0) {
                        mvprintw(row / 2 + 4, (col - 30) / 2, "Your password is: %s", p->password);
                        found = 1;
                        break;
                    }
                }
            }
        }

        if (!found) {
            mvprintw(row / 2 + 4, (col - 30) / 2, "Email not found!");
        }

        fclose(file);
        getch();
        return;
    }
}


void sign_in(User *p, Game *g) {
    char username[MAX_LENGTH] = "";
    char password[MAX_LENGTH] = "";
    int highlight = 0;
    int row, col;

    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 15) / 2, "Sign In");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        char *fields[] = {"Username", "Password", "Forgot Password", "Submit"};
        for (int i = 0; i < 4; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 20) / 2, fields[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        if (highlight < 2) {
            move(row / 2 - 4 + highlight * 2, col / 2);
            if (highlight == 0)
                printw("%s", username);
            else if (highlight == 1)
                printw("%s", password);
        }

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                highlight = (highlight + 3) % 4;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 4;
                break;
            case 10:  // Enter key
                if (highlight == 0) {  // Username
                    echo();
                    mvprintw(row / 2 - 4 + highlight * 2, col / 2, "                         ");
                    move(row / 2 - 4 + highlight * 2, col / 2);
                    getstr(username);
                    noecho();
                } else if (highlight == 1) {  // Password
                    echo();
                    mvprintw(row / 2 - 4 + highlight * 2, col / 2, "                         ");
                    move(row / 2 - 4 + highlight * 2, col / 2);
                    getstr(password);
                    noecho();
                } else if (highlight == 2) {  // Forgot Password
                    forgot_password(p);
                    return;
                } else if (highlight == 3) {  // Submit
                    int found = 0;
                    FILE *file = fopen("users.txt", "r");
                    if (file == NULL) {
                        mvprintw(row / 2 + 4, (col - 30) / 2, "Unable to open users file!");
                        getch();
                        return;
                    }

                    while (fgets(p->username, MAX_LENGTH, file) != NULL) {

                        p->username[strcspn(p->username, "\n")] = '\0';
                        
                        if (fgets(p->password, MAX_LENGTH, file) != NULL) {

                            p->password[strcspn(p->password, "\n")] = '\0';

                            if (strcmp(p->username, username) == 0 && strcmp(p->password, password) == 0) {
                                mvprintw(row / 2 + 4, (col - 25) / 2, "Login successful!");
                                getchar();
                                pre_game_menu(p, g);
                                found = 1;
                                break;
                            }
                        }
                    }
                    if (!found) {
                        mvprintw(row / 2 + 4, (col - 30) / 2, "Invalid username or password!");
                    }
                    fclose(file);
                    getch();
                    return;
                }
                break;
            case 27:  // ESC key
                return;
            default:
                break;
        }
    }
}

void sign_up(User *p, Game *g) {
    char username[MAX_LENGTH] = "";
    char password[MAX_LENGTH] = "";
    char email[MAX_LENGTH] = "";
    int highlight = 0;
    int row, col;

    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 15) / 2, "Sign Up");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        char *fields[] = {"Username", "Password", "Email", "Submit"};
        for (int i = 0; i < 4; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 20) / 2, fields[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        if (highlight < 3) {
            move(row / 2 - 4 + highlight * 2, col / 2);
            if (highlight == 0)
                printw("%s", username);
            else if (highlight == 1)
                printw("%s", password);
            else if (highlight == 2)
                printw("%s", email);
        }

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                highlight = (highlight + 3) % 4;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 4;
                break;
            case 10:  // Enter key
                if (highlight == 0) {  // Username
                    echo();
                    mvprintw(row / 2 - 4 + highlight * 2, col / 2, "                         ");
                    move(row / 2 - 4 + highlight * 2, col / 2);
                    getstr(username);
                    noecho();
                } else if (highlight == 1) {  // Password
                    echo();
                    mvprintw(row / 2 - 4 + highlight * 2, col / 2, "                         ");
                    move(row / 2 - 4 + highlight * 2, col / 2);
                    getstr(password);
                    noecho();
                } else if (highlight == 2) {  // Email
                    echo();
                    mvprintw(row / 2 - 4 + highlight * 2, col / 2, "                         ");
                    move(row / 2 - 4 + highlight * 2, col / 2);
                    getstr(email);
                    noecho();
                } else if (highlight == 3) {  // Submit
                    if (strlen(username) == 0 || strlen(password) == 0 || strlen(email) == 0) {
                        mvprintw(row / 2 + 4, (col - 30) / 2, "All fields are required!");
                        getch();
                    } else if (!validate_email(email)) {
                        mvprintw(row / 2 + 4, (col - 30) / 2, "Invalid email format!");
                        getch();
                    } else if (!valid_password(password)) {
                        mvprintw(row / 2 + 4, (col - 30) / 2, "Password must be at least 7 characters, contain one uppercase letter, one lowercase letter, and one digit!");
                        getch();
                    } else {
                        int exists = 0;

                            if (strcmp(p->username, username) == 0) {
                                exists = 1;
                            }
                        
                        if (exists) {
                            mvprintw(row / 2 + 4, (col - 30) / 2, "Username already exists!");
                            getch();
                        } else {
                            strcpy(p->username, username);
                            strcpy(p->password, password);
                            strcpy(p->email, email);
                            save_users(p);
                            mvprintw(row / 2 + 4, (col - 30) / 2, "User registered successfully!");
                            pre_game_menu(p,g);
                            return;
                            
                        }
                    }
                }
                break;
            case 27:  // ESC key
                return;
            default:
                break;
        }
    }
}

void sign_in_as_guest(User *p, Game *g) {
    int row, col;
    clear();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 2, (col - 25) / 2, "Logged in as Guest!");
    mvprintw(row / 2, (col - 30) / 2, "Press any key to continue...");
    getch();
    pre_game_menu(p,g);
}

void draw_menu(User *p, Game *g) {
    char *choices[] = {"Sign In", "Sign Up", "Sign In as Guest", "Exit"};
    int choice, highlight = 0;
    int row, col;
    curs_set(0);
    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(LINES / 2 - 12, COLS / 2 - 23, " ____                                      ");
        mvprintw(LINES / 2 - 11, COLS / 2 - 23, "/\\  _`\\                                    ");
        mvprintw(LINES / 2 - 10, COLS / 2 - 23, "\\ \\ \\L\\ \\    ___      __   __  __     __   ");
        mvprintw(LINES / 2 - 9, COLS / 2 - 23, " \\ \\ ,  /   / __`\\  /'_ `\\/\\ \\/\\ \\  /'__`\\ ");
        mvprintw(LINES / 2 - 8, COLS / 2 - 23, "  \\ \\ \\\\ \\ /\\ \\L\\ \\/\\ \\L\\ \\ \\ \\_\\ \\/\\  __/ ");
        mvprintw(LINES / 2 - 7, COLS / 2 - 23, "   \\ \\_\\ \\_\\ \\____/\\ \\____ \\ \\____/\\ \\____\\");
        mvprintw(LINES / 2 - 6, COLS / 2 - 23, "    \\/_/\\/ /\\/___/  \\/___L\\ \\/___/  \\/____/");
        mvprintw(LINES / 2 - 5, COLS / 2 - 23, "                      /\\____/              ");
        mvprintw(LINES / 2 - 4, COLS / 2 - 23, "                      \\_/__/               ");
        
        // Display choices at the bottom
        for (int i = 0; i < 4; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row - 4 + i, (col - 20) / 2, choices[i]);
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
                curs_set(1);
                if (highlight == 0) sign_in(p,g);
                else if (highlight == 1) sign_up(p,g);
                else if (highlight == 2) sign_in_as_guest(p,g);
                else return;
                curs_set(0);
                break;
            default:
                break;
        }
    }
}

void pre_game_menu(User *p, Game *g) {
    char *choices[] = {"New Game", "Continue Previous Game", "Leaderboard", "Settings", "Back"};
    int highlight = 0, choice, row, col;

    curs_set(0);

    while (1) {
        clear();
        getmaxyx(stdscr, row, col);

        mvprintw(LINES / 2 - 12, COLS / 2 - 23, " ____                                      ");
        mvprintw(LINES / 2 - 11, COLS / 2 - 23, "/\\  _`\\                                    ");
        mvprintw(LINES / 2 - 10, COLS / 2 - 23, "\\ \\ \\L\\ \\    ___      __   __  __     __   ");
        mvprintw(LINES / 2 - 9, COLS / 2 - 23, " \\ \\ ,  /   / __`\\  /'_ `\\/\\ \\/\\ \\  /'__`\\ ");
        mvprintw(LINES / 2 - 8, COLS / 2 - 23, "  \\ \\ \\\\ \\ /\\ \\L\\ \\/\\ \\L\\ \\ \\ \\_\\ \\/\\  __/ ");
        mvprintw(LINES / 2 - 7, COLS / 2 - 23, "   \\ \\_\\ \\_\\ \\____/\\ \\____ \\ \\____/\\ \\____\\");
        mvprintw(LINES / 2 - 6, COLS / 2 - 23, "    \\/_/\\/ /\\/___/  \\/___L\\ \\/___/  \\/____/");
        mvprintw(LINES / 2 - 5, COLS / 2 - 23, "                      /\\____/              ");
        mvprintw(LINES / 2 - 4, COLS / 2 - 23, "                      \\_/__/               ");

        mvprintw(row - 2, 2, "Press 'ESC' to go back");


        for (int i = 0; i < 5; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 + i * 2, (col - 20) / 2, choices[i]);
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
                    continue_previous_game(p,g);
                } else if (highlight == 2) {  // Leaderboard
                    display_leaderboard(p,g);
                } else if (highlight == 3) {  // Settings
                    settings_menu(p,g);
                } else if (highlight == 4) {  // Back
                    return;
                }
                break;
            case 27:  // ESC key
                return;
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
    //game_launcher(p,g);
}
void continue_previous_game(User *p , Game *g){
    int row, col;
    clear();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 2, (col - 25) / 2, "Loading previous game...");
    mvprintw(row / 2, (col - 30) / 2, "Press any key to continue...");
    getch();
    //SAMPLE
    
}

                                                                        void sort(int *arr, int scores[], int size) {
                                                                            for(int i=0; i<size-1; i++) {
                                                                                for(int j=i+1; j<size; j++) {
                                                                                    if(scores[i] < scores[j]) {
                                                                                        int temp = *(arr+i);
                                                                                        *(arr+i) = *(arr+j);
                                                                                        *(arr+j) = temp;
                                                                                    }
                                                                                }
                                                                            }
                                                                        }

                                                                        int str_to_num (char *str) {
                                                                        
                                                                            int num = 0;
                                                                            int p = strlen(str)-1;
                                                                            for(p; p>=0; p--) {
                                                                                num = num*10 + (str[strlen(str)-p-1]-'0');
                                                                            }
                                                                            
                                                                            return num;
                                                                        }                                                                        
void display_leaderboard(User *p , Game *g){
    clear();
    init_pair(1, COLOR_BLACK, COLOR_CYAN);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

    attron(COLOR_PAIR(1));
    mvprintw(1, 1, "SCORE BOARD");
    attroff(COLOR_PAIR(1));
    mvprintw(1, 20, "Press any key to RETURN");
    mvprintw(3, 1, "RANK"); mvprintw(3, 10, "username"); mvprintw(3, 24, "score"); mvprintw(3, 35, "gold"); mvprintw(3, 44, "gamecounts"); mvprintw(3, 58, "exp");

    char usernames[10][50]; int scores[10]; int golds[10]; int game_counts[10]; int exps[10] = {0};
    int arr[] = {0,1,2,3,4,5,6,7,8,9};
    int *ptr_arr = arr;

    FILE *fptr;
    fptr = fopen("users.txt","r");
    char line[50]; int i = 0; int count = 0;
    while(fgets(line, 50, fptr) != NULL) {
        line[strcspn(line,"\n")] = '\0';
        if(i%6 == 0) {
            if(i != 0) {
                count++;
            }
            strcpy(usernames[count],line);
        }
        else if(i%6 == 3) {
            scores[count] = str_to_num(line);
        }
        else if(i%6 == 4) {
            golds[count] = str_to_num(line);
        }
        else if(i%6 == 5) {
            game_counts[count] = str_to_num(line);
        }
        i++;
    }
    fclose(fptr);

    sort(ptr_arr, scores, (count%10)+1);
    for(int i=0; i<=count; i++) {
        if(0<=i && i<=2) {
            attron(COLOR_PAIR(3));
            mvprintw(5+i, 66, "BEST PLAYER");
            attroff(COLOR_PAIR(3));
            attron(COLOR_PAIR(2));
        }
        else{
            attroff(COLOR_PAIR(2));
        }
        if(strcmp(p->username,usernames[arr[i]]) == 0) {
            mvprintw(5+i, 1, "-> %d", i+1); mvprintw(5+i, 10, "%s", usernames[arr[i]]); mvprintw(5+i, 24, "%d", scores[arr[i]]); mvprintw(5+i, 35, "%d", golds[arr[i]]); mvprintw(5+i, 44, "%d", game_counts[arr[i]]); mvprintw(5+i, 58, "%d", exps[arr[i]]);
        }
        else {
            mvprintw(5+i, 1, "%d", i+1); mvprintw(5+i, 10, "%s", usernames[arr[i]]); mvprintw(5+i, 24, "%d", scores[arr[i]]); mvprintw(5+i, 35, "%d", golds[arr[i]]); mvprintw(5+i, 44, "%d", game_counts[arr[i]]); mvprintw(5+i, 58, "%d", exps[arr[i]]);
        }
        
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


