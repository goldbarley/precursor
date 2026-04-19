all:
	gcc -x c main.c src/prc_window.c -Iinclude -o main -lncurses

off:
	gcc -x c -DNOECHO main.c src/prc_window.c -Iinclude -o main -lncurses

b:
	gcc -x c main.c src/prc_window.c -Iinclude -o main -lncurses && ./main

bf:
	gcc -x c -DNOECHO main.c src/prc_window.c -Iinclude -o main -lncurses && ./main

clean:
	rm --force main
