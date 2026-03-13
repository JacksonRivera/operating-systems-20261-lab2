CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
TARGET = scheduler_sim
SRC = src/main.c src/scheduler/scheduler.c src/utils/utils.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) results.csv
