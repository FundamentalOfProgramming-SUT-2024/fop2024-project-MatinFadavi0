#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 100
#define MAP_HEIGHT 40
#define MAX_ROOMS 6
#define PLAYER '@'
#define FLOOR '.'
#define WALL_VERTICAL '|'
#define WALL_HORIZONTAL '='
#define DOOR '+'
#define CORRIDOR '#'

typedef struct {
    int x, y, width, height;
    int door_x, door_y;
}Room;

void initialize_map(char map[MAP_HEIGHT][MAP_WIDTH]);
void draw_map(char map[MAP_HEIGHT][MAP_WIDTH]);
void generate_rooms(char map[MAP_HEIGHT][MAP_WIDTH], Room rooms[], int *room_count);
void connect_rooms(char map[MAP_HEIGHT][MAP_WIDTH], Room r1, Room r2);
void move_player(char map[MAP_HEIGHT][MAP_WIDTH], int *player_x, int *player_y, int dx, int dy);

#endif
