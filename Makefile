compile:
	gcc main.c -o main.out -lglut -lGL -lX11 -lXtst
	./main.out

generate_dependencies:
	gcc -MM -lGlut -lGL -lX11 -lXtst main.c > dependency_list.d

key_press:
	gcc uart_handler.c -o uart_handler.out -lX11 -lXtst
	./uart_handler.out