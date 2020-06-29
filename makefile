CC = gcc 

sisop_ej2: sisop_ej2.o
	$(CC) -o sisop_ej2  sisop_ej2.o -pthread

sisop_ej2.o: sisop_ej2.c
	$(CC) -c sisop_ej2.c 

clean: 
	rm sisop_ej2.o