#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncursesw/ncurses.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

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
    int guest;
} User;

typedef struct {
    int x;
    int y;
} Pos;

typedef struct {
    char type[10];
    Pos room_pos;
    int room_size_v;
    int room_size_h;
    Pos doors[2];
    Pos pillar_seed;
    int traps_count;
    Pos traps[10];
    int gold;
    int dark_gold;
    int ordinary_food;
} Room;

typedef struct {
    User Player;
    int difficulty;
    int player_color;
    Pos player_pos;
    Room rooms[6];
    int MAX_health;
    int players_health;
    int players_score;
    int players_gold;
    int floor_number;
    int players_hungriness;
    int players_ordinary_food;
    time_t start_time;
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

void GameLauncher(User *p, Game *g);
void map(Game *g, int initial_room);
int movement(char **screen, int **visited, int ch, Game *g);
void ShowHealth(Game *g);
void Corridor(char direction, Pos door1, Pos door2);
void ShowScreen(char mode[], int **visited, char **screen);
void FloorGenerator(User *p, Game *g);
int check_room(Room *rooms, int i, int j);
void ShowMessage(char message[], int floor, int score, int gold);
int Trap_Check(Room *rooms, int i, int j);
void EndGame(User *p, Game *g);
void ExitScreen(User *p);
void Save_Game(User *p, Game *g, char **screen);
void save_screen(User *p , Game *g);
void not_saved_screen();
void food_screen(Game *g);
#endif
