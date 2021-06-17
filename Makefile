CC=gcc
CFLOBJ=-Wall -c -Iinclude/ -o
EXE=bin/framework
$(EXE): obj/main.o 
	$(CC) -o  $@ $^

obj/main.o: src/main.c
	$(CC) $(CFLOBJ) $@  $^


run:
	./$(EXE)

.PHONY: clean
clean:
	rm bin/* obj/*.o