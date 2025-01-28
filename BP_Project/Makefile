CC = gcc
CFLAGS = -Wall -Wextra
LDFLAGS = -lSDL2 -lSDL2_mixer -lncursesw
TARGET = my_project

SRCS = main.c game_menu.c auth.c map.c funcs.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.c game_menu.h auth.h player.h libs.h map.h
	$(CC) $(CFLAGS) -c $< -o $@

game_menu.o: game_menu.c game_menu.h player.h libs.h funcs.h
	$(CC) $(CFLAGS) -c $< -o $@

auth.o: auth.c auth.h player.h libs.h funcs.h
	$(CC) $(CFLAGS) -c $< -o $@

map.o: map.c map.h player.h funcs.h
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned up!"

.PHONY: all clean
