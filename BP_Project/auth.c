#include "libs.h"
#include "player.h"
#include "funcs.h"
#include "map.h"
#include "game_menu.h"

void save_users(User *p) {
    FILE *file = fopen("users.txt", "a");

    if (file == NULL) {
        printf("Unable to open the file!\n");
        return;
    }
    p->score = 0;
    p->gold = 0;
    p->count_games = 0;
    p->playtime = 0;
    fprintf(file, "%s\n%s\n%s\n%d\n%d\n%d\n%.2lf\n", p->username, p->password, p->email, p->score, p->gold, p->count_games,p->playtime);

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
    char score[50]; char gold[50]; char count_games[50]; char playtime[50];
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
                                p->guest = 0;
                                strcpy(p->password,password);
                                fgets(p->email, 50, file);
                                fgets(score, 50, file); score[strcspn(score, "\n")] = '\0';
                                fgets(gold, 50, file); gold[strcspn(gold, "\n")] = '\0';
                                fgets(count_games, 50, file); count_games[strcspn(count_games, "\n")] = '\0';
                                fgets(playtime, 50, file); playtime[strcspn(playtime, "\n")] = '\0';
                                p->score = str_to_num(score); p->gold = str_to_num(gold); p->count_games = str_to_num(count_games); p->playtime = strtod(playtime , NULL);
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

int is_username_taken(const char *filename, const char *username) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, username) == 0) {
            fclose(file);
            return 1;
        }
        for (int i = 0; i < 5; i++) {
            if (!fgets(line, sizeof(line), file)) break;
        }
    }

    fclose(file);
    return 0;
}

void generate_random_password(char *password, int length) {
    const char lowercase[] = "abcdefghijklmnopqrstuvwxyz";
    const char uppercase[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const char digits[] = "0123456789";
    const char all_chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    srand(time(NULL));
    

    password[0] = lowercase[rand() % (sizeof(lowercase) - 1)];
    password[1] = uppercase[rand() % (sizeof(uppercase) - 1)];
    password[2] = digits[rand() % (sizeof(digits) - 1)];
    

    for(int i = 3; i < length; i++) {
        password[i] = all_chars[rand() % (sizeof(all_chars) - 1)];
    }
    password[length] = '\0';
    

    for(int i = 0; i < length; i++) {
        int j = rand() % length;
        char temp = password[i];
        password[i] = password[j];
        password[j] = temp;
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


        char *fields[] = {"Username", "Password", "Generate Password", "Email", "Submit"};
        for (int i = 0; i < 5; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row / 2 - 4 + i * 2, (col - 20) / 2, fields[i]);
            if (i == highlight)
                attroff(A_REVERSE);
        }

        if (highlight < 4) {
            move(row / 2 - 4 + highlight * 2, col / 2);
            if (highlight == 0)
                printw("%s", username);
            else if (highlight == 1)
                printw("%s", password);
            else if (highlight == 3)
                printw("%s", email);
        }

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                highlight = (highlight + 4) % 5;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % 5;
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
                } else if (highlight == 2) {  // Generate Password
                    generate_random_password(password, 10);
                    mvprintw(row / 2 - 4 + 1 * 2, col / 2, "            ");
                    mvprintw(row / 2 - 4 + 1 * 2, col / 2, "%s", password);
                } else if (highlight == 3) {  // Email
                    echo();
                    mvprintw(row / 2 - 4 + highlight * 2, col / 2, "                         ");
                    move(row / 2 - 4 + highlight * 2, col / 2);
                    getstr(email);
                    noecho();
                } else if (highlight == 4) {  // Submit
                    if (strlen(username) == 0 || strlen(password) == 0 || strlen(email) == 0) {
                        mvprintw(row / 2 + 6, (col - 30) / 2, "All fields are required!");
                        getch();
                    } else if (!validate_email(email)) {
                        mvprintw(row / 2 + 6, (col - 30) / 2, "Invalid email format!");
                        getch();
                    } else if (!valid_password(password)) {
                        mvprintw(row / 2 + 6, (col - 30) / 2, "Password must be at least 7 characters, contain one uppercase, one lowercase and one digit!");
                        getch();
                    } else {
                        int exists = is_username_taken("users.txt",username);
                        if (exists) {
                            mvprintw(row / 2 + 6, (col - 30) / 2, "Username already exists!");
                            getch();
                        } else {
                            strcpy(p->username, username);
                            strcpy(p->password, password);
                            strcpy(p->email, email);
                            save_users(p);
                            mvprintw(row / 2 + 6, (col - 30) / 2, "User registered successfully!");
                            p->guest = 0;
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
    p->guest = 1;
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
        // Display choices at the bottom
        for (int i = 0; i < 4; i++) {
            if (i == highlight)
                attron(A_REVERSE);
            mvprintw(row - 23 + 2*i, (col - 20) / 2, choices[i]);
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
                else{ endwin(); exit(0);}
                curs_set(0);
                break;
            default:
                break;
        }
    }
}