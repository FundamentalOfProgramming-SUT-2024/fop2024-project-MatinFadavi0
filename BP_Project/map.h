#ifndef MAP_H
#define MAP_H

#include "player.h"
#include "libs.h"


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