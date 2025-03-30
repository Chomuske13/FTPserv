CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -lreadline
TARGET = ftp_server
SRC_DIR = src
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/client_handler.c $(SRC_DIR)/file_ops.c \
          $(SRC_DIR)/archive.c $(SRC_DIR)/commands.c $(SRC_DIR)/interface.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean