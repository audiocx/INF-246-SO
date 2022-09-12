orden: ordenArchivos.o
	gcc -o orden ordenArchivos.o -lm -Wall

orden.o: ordenArchivos.c
	gcc -c ordenArchivos.c
	
visual: visualizacionDatos.o
	gcc -o visual visualizacionDatos.o -lm -Wall
	
visual.o:
	gcc -c visualizacionDatos.c

clean:
	rm -f orden *.o
	rm -f visual *.o
