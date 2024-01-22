CC = g++
INC = $(shell ls Inc/*.h)
SRC = $(shell ls Src/*.cpp)
CFLAGS = -Wall -Wextra 

ImageDrop: $(SRC)
	$(CC) $(INC) -o $@ $(SRC) $(CFLAGS)

.PHONY: clean

clean:
	rm ImageDrop
