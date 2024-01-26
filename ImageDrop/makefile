CC = g++
INC = -I Inc/
SRC = $(shell ls Src/*.cpp)
CFLAGS = -Wall -Wextra 
ENABLEPY = -shared -fPIC
NAME = ImageDrop

ImageDrop: $(SRC)
	$(CC)  $(INC) -o $(NAME) $(SRC)

.PHONY: clean

clean:
	rm ImageDrop *.png *.jpg
