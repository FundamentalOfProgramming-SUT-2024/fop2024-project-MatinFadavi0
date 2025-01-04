#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "map.h"

void initialize_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
    for (int i = 0; i < MAP_HEIGHT; i++)
        for (int j = 0; j < MAP_WIDTH; j++)
            map[i][j] = ' ';
}

void draw_map(char map[MAP_HEIGHT][MAP_WIDTH]) {
    clear();
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            mvaddch(i, j, map[i][j]);
        }
    }
    refresh();
}

void generate_rooms(char map[MAP_HEIGHT][MAP_WIDTH], Room rooms[], int *room_count) {
    srand(time(NULL));
    while (*room_count < MAX_ROOMS){
        Room new_room;
        new_room.width = 10 + rand() % 12;
        new_room.height = 6 + rand() % 8;
        
        new_room.x = rand() % (MAP_WIDTH - new_room.width - 2) + 1;
        new_room.y = rand() % (MAP_HEIGHT - new_room.height - 2) + 1;

        int overlapping = 0;
        for (int i = 0; i < *room_count; i++){
            Room r = rooms[i];
            if (new_room.x < r.x + r.width && new_room.x + new_room.width > r.x &&
                new_room.y < r.y + r.height && new_room.y + new_room.height > r.y) {
                overlapping = 1;
                break;
            }
        }

        if (!overlapping) {
            rooms[*room_count] = new_room;

            for (int i = 0; i < new_room.height; i++){
                for (int j = 0; j < new_room.width; j++){
                    int y = new_room.y + i;
                    int x = new_room.x + j;
                    if (i == 0 || i == new_room.height - 1){
                        map[y][x] = WALL_HORIZONTAL;
                    } 
                    else if (j == 0 || j == new_room.width - 1){
                        map[y][x] = WALL_VERTICAL;
                    } 
                    else{
                        map[y][x] = FLOOR;
                    }
                }
            }

            int door_side = rand() % 4;
            if (door_side == 0){
                new_room.door_x = new_room.x + 1 + rand() % (new_room.width - 2);
                new_room.door_y = new_room.y;
            } else if (door_side == 1){
                new_room.door_x = new_room.x + 1 + rand() % (new_room.width - 2);
                new_room.door_y = new_room.y + new_room.height - 1;
            } else if (door_side == 2) {
                new_room.door_x = new_room.x;
                new_room.door_y = new_room.y + 1 + rand() % (new_room.height - 2);
            } else {
                new_room.door_x = new_room.x + new_room.width - 1;
                new_room.door_y = new_room.y + 1 + rand() % (new_room.height - 2);
            }

            if (new_room.door_x >= 0 && new_room.door_x < MAP_WIDTH &&
                new_room.door_y >= 0 && new_room.door_y < MAP_HEIGHT) {
                map[new_room.door_y][new_room.door_x] = DOOR;
            }
            (*room_count)++;
        }
    }
}

void connect_rooms(char map[MAP_HEIGHT][MAP_WIDTH], Room r1, Room r2){
    int x1 = r1.door_x;
    int y1 = r1.door_y;
    int x2 = r2.door_x;
    int y2 = r2.door_y;

    while (x1 != x2) {
        if (x1 >= 0 && x1 < MAP_WIDTH && y1 >= 0 && y1 < MAP_HEIGHT){
            map[y1][x1] = CORRIDOR;
        }
        x1 += (x1 < x2) ? 1 : -1;
    }

    while (y1 != y2) {
        if (x1 >= 0 && x1 < MAP_WIDTH && y1 >= 0 && y1 < MAP_HEIGHT){
            map[y1][x1] = CORRIDOR;
        }
        
        y1 += (y1 < y2) ? 1 : -1;
    }
}

void move_player(char map[MAP_HEIGHT][MAP_WIDTH], int *player_x, int *player_y, int dx, int dy){
    int new_x = *player_x + dx;
    int new_y = *player_y + dy;

    if (new_x >= 0 && new_x < MAP_WIDTH && 
        new_y >= 0 && new_y < MAP_HEIGHT && 
        (map[new_y][new_x] == FLOOR || 
         map[new_y][new_x] == CORRIDOR || 
         map[new_y][new_x] == DOOR)) {
        if (map[*player_y][*player_x] != DOOR) {
            map[*player_y][*player_x] = FLOOR;
        }
        *player_x = new_x;
        *player_y = new_y;
        map[new_y][new_x] = PLAYER;
    }
}
