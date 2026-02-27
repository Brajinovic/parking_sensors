SRCS =  *.c
CFLAGS = -lncurses -Isrc/include
vpath %.c $(src/)
vpath %.o %(src/)

compile:
	gcc src/$(SRCS) -o bin/main.out $(CFLAGS)
	./bin/main.out

run:
	./bin/main.out

clean:
	rm ./bin/*