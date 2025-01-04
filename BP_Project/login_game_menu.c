#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "login_game_menu.h"

#define MAX_USERS 100
#define MAX_LENGTH 50

typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char email[MAX_LENGTH];
} User;

User users[MAX_USERS];
int user_count = 0;

void load_users() {
    FILE *file = fopen("users.txt", "r");
    if (file != NULL) {
        while (fscanf(file, "%s %s %s", users[user_count].username, users[user_count].password, users[user_count].email) != EOF) {
            user_count++;
        }
        fclose(file);
    }
}

void save_users() {
    FILE *file = fopen("users.txt", "w");
    if (file != NULL) {
        for (int i = 0; i < user_count; i++) {
            fprintf(file, "%s %s %s\n", users[i].username, users[i].password, users[i].email);
        }
        fclose(file);
    }
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

void forgot_password() {
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

        // Check if email exists
        for (int i = 0; i < user_count; i++) {
            if (strcmp(users[i].email, email) == 0) {
                mvprintw(row / 2 + 4, (col - 30) / 2, "Your password is: %s", users[i].password);
                found = 1;
                break;
            }
        }
        if (!found) {
            mvprintw(row / 2 + 4, (col - 30) / 2, "Email not found!");
        }

        getch();
        return;
    }
}

void sign_in() {
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
                    forgot_password();
                    return;
                } else if (highlight == 3) {  // Submit
                    int found = 0;
                    for (int i = 0; i < user_count; i++) {
                        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
                            mvprintw(row / 2 + 4, (col - 25) / 2, "Login successful!");
                            pre_game_menu();
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        mvprintw(row / 2 + 4, (col - 30) / 2, "Invalid username or password!");
                    }
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

void sign_up() {
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
                        for (int i = 0; i < user_count; i++) {
                            if (strcmp(users[i].username, username) == 0) {
                                exists = 1;
                                break;
                            }
                        }
                        if (exists) {
                            mvprintw(row / 2 + 4, (col - 30) / 2, "Username already exists!");
                            getch();
                        } else {
                            strcpy(users[user_count].username, username);
                            strcpy(users[user_count].password, password);
                            strcpy(users[user_count].email, email);
                            user_count++;
                            save_users();
                            mvprintw(row / 2 + 4, (col - 30) / 2, "User registered successfully!");
                            pre_game_menu();
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

void sign_in_as_guest() {
    int row, col;
    clear();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 2, (col - 25) / 2, "Logged in as Guest!");
    mvprintw(row / 2, (col - 30) / 2, "Press any key to continue...");
    getch();
    pre_game_menu();
}

void draw_menu() {
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
                if (highlight == 0) sign_in();
                else if (highlight == 1) sign_up();
                else if (highlight == 2) sign_in_as_guest();
                else return;
                curs_set(0);
                break;
            default:
                break;
        }
    }
}

void pre_game_menu() {
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
                    start_new_game();
                } else if (highlight == 1) {  // Continue Previous Game
                    continue_previous_game();
                } else if (highlight == 2) {  // Leaderboard
                    display_leaderboard();
                } else if (highlight == 3) {  // Settings
                    settings_menu();
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


void start_new_game() {
    int row, col;
    clear();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 2, (col - 25) / 2, "Starting a new game...");
    mvprintw(row / 2, (col - 30) / 2, "Press any key to continue...");
    getch();
    //SAMPLE
}
void continue_previous_game(){
    int row, col;
    clear();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 2, (col - 25) / 2, "Loading previous game...");
    mvprintw(row / 2, (col - 30) / 2, "Press any key to continue...");
    getch();
    //SAMPLE
    
}
void display_leaderboard(){
    int row, col;
    clear();
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2 - 8, (col - 25) / 2, "Leaderboard");
    mvprintw(row - 2, 2, "Press any key to go back");


    // SAMPLE
    mvprintw(row / 2, col / 2 - 10, "Rank | Username   | Score");
    mvprintw(row / 2 + 2, col / 2 - 10, "1    | Player1    | 1000");
    mvprintw(row / 2 + 3, col / 2 - 10, "2    | Player2    | 800");
    mvprintw(row / 2 + 4, col / 2 - 10, "3    | Player3    | 750");
    getch();

    // THIS IS FOR NOW..... LATER I AM GONNA READ SCOREBOARD FROM FILE
}
void change_difficulty(){
    char *difficulties[] = {"Easy", "Medium", "Hard"};
    int highlight = 0, choice, row, col;
    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 20) / 2, "Select Difficulty");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        for (int i = 0; i < 3; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 15) / 2, difficulties[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        choice = getch();
        switch (choice) {
            case KEY_UP:
                highlight = (highlight + 2) % 3;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 3;
                break;
            case '\n':
                mvprintw(row / 2 + 4, (col - 30) / 2, "Difficulty set to %s!", difficulties[highlight]);
                // Save the Choice in File Later!
                getch();
                return;
            case 27:  // ESC key
                return;
            default:
                break;
        }
    }
}

void change_character_color() {
    char *colors[] = {"Red", "Green", "Blue", "Yellow", "White"};
    int highlight = 0, choice, row, col;
    while (1) {
        clear();
        getmaxyx(stdscr, row, col);
        mvprintw(row / 2 - 8, (col - 25) / 2, "Select Character Color");
        mvprintw(row - 2, 2, "Press 'ESC' to go back");

        for (int i = 0; i < 5; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 20) / 2, colors[i]);
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
                mvprintw(row / 2 + 4, (col - 30) / 2, "Character color set to %s!", colors[highlight]);
                // Save The Colors In File later!
                getch();
                return;
            case 27:  // ESC key
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



void settings_menu() {
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
                    change_difficulty();
                else if (highlight == 1)
                    change_character_color();
                else if (highlight == 2)
                    select_music();
                else
                    return;
                break;
            case 27:  // ESC key
                return;
            default:
                break;
        }
    }
}


