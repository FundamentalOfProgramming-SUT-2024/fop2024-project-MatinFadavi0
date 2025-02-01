#ifndef FUNCS_H
#define FUNCS_H

#include "map.h"

int str_to_num(char *str);
void sort(int *arr, int scores[], int size);
int power(int num, int n);
const char *txt_format(char username[]);
int password();
void play_music(const char *filename);
void stop_music();

#endif