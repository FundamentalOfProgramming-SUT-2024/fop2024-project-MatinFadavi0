#ifndef HEADER_H
#define HEADER_H

#include <time.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 100
#define MAX_LENGTH 50

typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char email[MAX_LENGTH];
    int score;
    int gold;
    int count_games;
    int experience;
} User;

typedef struct {
    int x;
    int y;
} Pos;

typedef struct {
    User Player;
    int difficulty;
    int player_color;
} Game;

void draw_menu();
void sign_in();
void sign_up();
void forgot_password();
void load_users();
void save_users();

void pre_game_menu();
void start_new_game();
void continue_previous_game();
void display_leaderboard();

void settings_menu();
void change_difficulty();
void change_character_color();
void select_music();
#endif