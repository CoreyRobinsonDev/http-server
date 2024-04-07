
TARGET = bin/http_server
SRC = $(wildcard src/*.c) 
OBJ = $(patsubst src/%.c, bin/obj/%.o, $(SRC)) 

run: clean default
	./$(TARGET)

default: $(TARGET)

clean:
	@rm -rf bin/*
	@mkdir bin/obj

$(TARGET): $(OBJ)
	@gcc -o $@ $? 

bin/obj/%.o: src/%.c
	@gcc -c $< -o $@ -Isrc/include 
    
