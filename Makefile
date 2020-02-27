
main:	lab2-main.o
	gcc lab2-main.o -o output

lab2-main.o: lab2-main.c
	gcc -c lab2-main.c

run:
	./output

clean:
	rm *.o output