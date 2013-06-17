all: t1enc.o ep4.o getline.o word.o Item.o t2enc.o t1lp.o t2lp.o
	make ep4encenc ep4enclp ep4lpenc ep4lplp

ep4encenc: t1enc.o ep4.o getline.o word.o Item.o t2enc.o
	gcc t1enc.o ep4.o getline.o word.o Item.o t2enc.o -o ep4encenc -lm

ep4enclp: t1enc.o ep4.o getline.o word.o Item.o t2lp.o
	gcc t1enc.o ep4.o getline.o word.o Item.o t2lp.o -o ep4enclp -lm

ep4lpenc: t1lp.o ep4.o getline.o word.o Item.o t2enc.o
	gcc t1lp.o ep4.o getline.o word.o Item.o t2enc.o -o ep4lpenc -lm

ep4lplp: t1lp.o ep4.o getline.o word.o Item.o t2lp.o
	gcc t1lp.o ep4.o getline.o word.o Item.o t2lp.o -o ep4lplp -lm 

ep4.o: ep4.c
	gcc -Wall -ansi -pedantic -g -c ep4.c -lm

t1enc.o: t1enc.c
	gcc -Wall -ansi -pedantic -g -c t1enc.c -lm

t2enc.o: t2enc.c
	gcc -Wall -ansi -pedantic -g -c t2enc.c -lm

t1lp.o: t1lp.c
	gcc -Wall -ansi -pedantic -g -c t1lp.c -lm

t2lp.o: t2lp.c
	gcc -Wall -ansi -pedantic -g -c t2lp.c -lm

getline.o: getline.c
	gcc -Wall -ansi -pedantic -g -c getline.c -lm 

word.o: word.c
	gcc -Wall -ansi -pedantic -g -c word.c -lm

Item.o: Item.c
	gcc -Wall -ansi -pedantic -g -c Item.c -lm

clean:
	rm *.o ep4encenc ep4enclp ep4lplp ep4lpenc
