CC = gcc
CFLAGS = -Wall -Wextra
TARGET = main
SRCS = main.c platform.c vl53l8cx_api.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
