#Makefile simpletron_2

CC = gcc
CFLAGS = -Wall
ANSI_FLAG = -ansi
FLAG_ESP = -DIDIOMA_ESPANIOL
FLAG_ENG = -DLANG_ENGLISH

espaniol:
	make CFLAGS+="$(FLAG_ESP)" all

english:
	make CFLAGS+="$(FLAG_ENG)" all

ansi:
	make CFLAGS+="$(ANSI_FLAG)" all
	
all: simpletron clean

simpletron: main.o procesar_arg.o lectura.o simpletron.o dump.o error.o vector.o herramientas.o tda_lista_ite.o
	$(CC) $(CFLAGS) -o simpletron main.o procesar_arg.o tda_lista_ite.o lectura.o simpletron.o dump.o error.o vector.o herramientas.o
main.o: main.c error.h tda_lista.h lectura.h simpletron.h dump.h
	$(CC) $(CFLAGS) -o main.o -c main.c
procesar_arg.o: procesar_arg.c procesar_arg.h error.h
	$(CC) $(CFLAGS) -o procesar_arg.o -c procesar_arg.c
lectura.o: lectura.c lectura.h vector.h simpletron.h procesar_arg.h herramientas.h error.h tda_lista.h
	$(CC) $(CFLAGS) -o lectura.o -c lectura.c
simpletron.o: simpletron.c simpletron.h vector.h tda_lista.h
	$(CC) $(CFLAGS) -o simpletron.o -c simpletron.c
dump.o: dump.c dump.h simpletron.h vector.h tda_lista.h idioma.h
	$(CC) $(CFLAGS) -o dump.o -c dump.c
error.o: error.c error.h idioma.h
	$(CC) $(CFLAGS) -o error.o -c error.c
vector.o: vector.c vector.h simpletron.h error.h tda_lista.h types.h
	$(CC) $(CFLAGS) -o vector.o -c vector.c
herramientas.o: herramientas.c herramientas.h idioma.h error.h simpletron.h
	$(CC) $(CFLAGS) -o herramientas.o -c herramientas.c
tda_lista_ite.o: tda_lista_ite.c tda_lista.h
	$(CC) $(CFLAGS) -o tda_lista_ite.o -c tda_lista_ite.c
	
clean: rm *.o
