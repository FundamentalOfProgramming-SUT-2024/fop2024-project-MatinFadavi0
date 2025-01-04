CC = gcc
CFLAGS = -lncurses -lSDL2 -lSDL2_mixer
TARGET = my_project

SRCS = main.c login_game_menu.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
