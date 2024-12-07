SRCS =  main.c uart_handler.c draw.c idle.c
CFLAGS = -lglut -lGL -lX11 -lXtst
compile:
	gcc $(SRCS) -o main.out $(CFLAGS)
	./main.out

generate_dependencies:
	makedepend -- $(CFLAGS) -- $(SRCS)

key_press:
	gcc uart_handler.c -o uart_handler.out -lX11 -lXtst
	./uart_handler.out