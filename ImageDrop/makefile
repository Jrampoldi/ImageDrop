CC = g++
INC = -I Inc/
SRC = $(shell ls Src/*.cpp)
CFLAGS = -Wall -Wextra 

ImageDrop: $(SRC)
	$(CC) $(INC) -o $@ $(SRC)

.PHONY: clean

clean:
	rm ImageDrop
