CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -MMD -MP

SRC = src/main.c src/ticket/ticket.c src/utils/utils.c
OBJ = $(SRC:.c=.o)
DEP = $(OBJ:.o=.d)

TARGET = ticket_app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJ) $(DEP)

-include $(DEP)