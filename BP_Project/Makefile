CC = gcc
CFLAGS = -lncurses
TARGET = my_project

SRCS = main.c login_menu.c
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)
