OBJECTS = obj/main.o obj/bmp.o
HEADERS = include/bmp.h
OUTPUT = hw-01_bmp
FLAGS = -Wall -Werror -Wextra

$(OUTPUT) : obj $(OBJECTS)
	gcc $(FLAGS) -I./include -o $@ $(OBJECTS)
$(OBJECTS): obj/%.o  : src/%.c $(HEADERS) 
	gcc $(FLAGS) -I./include -c  $< -o $@
obj : 
	mkdir -p obj
clean:
	rm -r obj
	rm $(OUTPUT)
