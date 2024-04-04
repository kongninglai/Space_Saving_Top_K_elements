CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

SRC = find_freq_items.c
OBJ = $(SRC:.c=.o)
EXECUTABLE = find_freq_items

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJ)
