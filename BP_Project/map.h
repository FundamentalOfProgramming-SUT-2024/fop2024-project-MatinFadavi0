#ifndef MAP_H
#define MAP_H

#include "player.h"
#include "libs.h"


typedef struct {
    int x;
    int y;
} Pos;

typedef struct {
    int alive;
    int type;
    int room;
    Pos position;
    int radius;
    int on;    
    int health;
    int damage;
    int haunt;
} Monster;

typedef struct {
    int type;
    int locked;
    int password;
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
    int potion;
    int monsters_count;
    Monster monsters[8];
} Room;

typedef struct {
    User player;
    int difficulty;
    int player_color;
    Pos player_pos;
    Room rooms[6];
    int MAX_health;
    int hungriness_rate;
    int players_health;
    int players_extra_health;
    int players_score;
    int players_gold;
    int floor_number;
    int players_hunger;
    int players_ordinary_food;
    int players_mace;
    int players_dagger;
    int players_magic_wand;
    int players_arrow;
    int players_sword;
    int players_health_potion;
    int players_speed_potion;
    int players_speed;
    int players_damage_potion;
    time_t start_time;
    int k_lock;
    time_t password_start_time;
    int secret_doors_count;
    Pos secret_doors[3];
    time_t enchant_start_time;
    time_t treasure_start_time;
    int players_weapon;
    int players_weapon_direction;
    int players_steps;
    int players_speed_step;
    int players_health_step;
    int players_damage_step;
    Pos weapons[3];
    int players_damage_rate;
} Game;



void GameLauncher(User *p, Game *g);


void MessageBar(Game *g, int floor, int score, int gold);
void HEALTH_BAR(Game *g);
void Food_Bar(Game *g);
void Show_Password(int password);



void FoodTab(Game *g);
void WeaponTab(Game *g);
void SpellTab(Game *g);
void show_key_guide();




void FloorGenerator(User *p, Game *g);
void map(Game *g, int initial_room);
int Movement(chtype **screen, int **visited, int ch, Game *g,User *p);
void Corridor(char direction, Pos door1, Pos door2);
void ShowScreen(Game *g, char mode[], int **visited, chtype **screen);
int RoomChecking(Room *rooms, int i, int j);
int CheckTrap(Room *rooms, int i, int j);
int CheckEnchantDoor(int count, Pos *secret_doors, int i, int j);
void password_screen(Game *g);
void EnchantRoom(User *p, Game *g);
void TreasureRoom(User *p, Game *g);




void Monsters_Movement(Game *g, chtype **screen);
void handle_monsters(Game *g, chtype **screen);
int CheckMonsters(Game *g, int i, int j);



void Active_Weapons(Game *g, char ch, chtype **screen);
void ThrowWeapon(Game *g, char ch, chtype **screen);
int CheckWeapons(Pos *weapons, int i, int j);



void terminate_game(int code, User *p, Game *g);
void YOUWON(User *p);
void YOULOST(User *p, Game *g);
void Pause_Screen();
void not_saved_screen(User *p, Game *g);




void SAVEGAME(User *p, Game *g, chtype **screen, int **visited);
void SaveLauncher(User *p, Game *g);
void SaveFloor(User *p, Game *g, chtype **screen, int **visited);


#endif
