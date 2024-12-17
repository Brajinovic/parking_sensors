SRCS =  *.c
CFLAGS = -lglut -lGL -lX11 -lXtst -lasound -Isrc/include
vpath %.c $(src/)
vpath %.o %(src/)

compile:
	gcc src/$(SRCS) -o bin/main.out $(CFLAGS)
	./bin/main.out

run:
	./bin/main.out

generate_dependencies:
	makedepend -- $(CFLAGS) -- $(SRCS)

key_press:
	gcc uart_handler.c -o uart_handler.out -lX11 -lXtst
	./uart_handler.out