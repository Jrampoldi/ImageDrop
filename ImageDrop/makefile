CC = g++
INC = -I Inc/
SRC = $(shell ls Src/*.cpp)
CFLAGS = -Wall -Wextra -g 
ENABLEPY = -shared -fPIC
NAME = pyGui/ImageDrop.so

ImageDrop: $(SRC)
	$(CC) $(ENABLEPY) $(INC) -o $(NAME) $(SRC)

.PHONY: clean

clean:
	rm ImageDrop.so *.png *.jpg
