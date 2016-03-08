build: menu.o game_logic.o main.o
	gcc menu.o game_logic.o main.o -lncurses -lmenu -Wall -o 2048EXE
	rm menu.o game_logic.o main.o
	export TERM='xterm-256color'
menu.o: main.c
	gcc -c menu.c -lmenu -lncurses -o menu.o
game_logic.o: game_logic.c
	gcc -c game_logic.c -lmenu -lncurses -o game_logic.o
main.o: main.c
	gcc -c main.c -lmenu -lncurses -o main.o
clean:
	rm *.o
