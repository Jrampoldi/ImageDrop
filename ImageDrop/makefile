CC = g++
INC = -I Inc/
SRC = $(shell ls Src/*.cpp)
CFLAGS = -Wall -Wextra 
ENABLEPY = -shared -fPIC
NAME = ImageDrop.so

ImageDrop: $(SRC)
	$(CC) $(ENABLEPY) $(INC) -o $(NAME) $(SRC)

.PHONY: clean

clean:
	rm ImageDrop.so *.png *.jpg
