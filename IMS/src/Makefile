all:  how angle  dragless

run: how angle dragless
	./how && ./angle && ./dragless

how: howitzer.c 
	gcc howitzer.c -o how -lm

angle: best_angle.c
	gcc best_angle.c -o angle -lm

dragless: dragless.c
	gcc dragless.c -o dragless -lm

clean:
	rm -f how angle dragless