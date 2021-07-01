CC=gcc
CFLOBJ=-Wall -lpthread -c -Iinclude/ -o
EXE=bin/framework


$(EXE): obj/main.o obj/processor.o obj/buffer.o obj/log.o
	$(CC) -o  $@ $^

obj/main.o: src/main.c
	$(CC) $(CFLOBJ) $@  $^

obj/processor.o: src/processor.c
	$(CC) -o $(CFLOBJ) $@ $^

obj/buffer.o: src/buffer.c
	$(CC) -o $(CFLOBJ) $@ $^

obj/log.o: src/log.c
	$(CC) -o $(CFLOBJ) $@ $^

sensor: obj/sensor.o
	$(CC) -o bin/$@ $^

obj/sensor.o: src/sensor.c
	$(CC) -o $(CFLOBJ) $@ $^


run:
	./$(EXE)

.PHONY: clean
clean:
	rm bin/* obj/*.o