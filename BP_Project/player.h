#ifndef PLAYER_H
#define PLAYER_H

#include "libs.h"

typedef struct {
    char username[MAX_LENGTH];
    char password[MAX_LENGTH];
    char email[MAX_LENGTH];
    int score;
    int gold;
    int count_games;
    double playtime;
    time_t start_time;
    int guest;
} User;

#endif