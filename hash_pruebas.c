#include "hash.c"
#include <stdio.h>
#include <stdbool.h>

#define COLOR_ROJO	   "\x1b[1m\x1b[31m"
#define COLOR_VERDE   "\x1b[1m\x1b[32m"
#define COLOR_NORMAL   "\x1b[0m"
#define COLOR_VIOLETA "\x1b[35;1m"

/*
* Funcion prueba, recibe el mensaje a dar 
* y si el resultado esta bien o mal. 
* De estar bien se pone en verde, de estar mal se pone en rojo
*/
void prueba(const char* mensaje, bool ok) {
	if (ok) {
		printf(COLOR_NORMAL "%s... ", mensaje);
		printf(COLOR_VERDE "OK\n");
	} else {
		printf(COLOR_NORMAL "%s: ", mensaje);
		printf(COLOR_ROJO "ERROR\n");
	}
	printf(COLOR_NORMAL);
}

/*Prueba para funcion hash_crear*/
void prueba_hash_crear(){
    printf("\nINICIO DE PRUEBAS HASH CREAR\n");

    /* Declaro las variables a utilizar*/
    hash_t* hash = hash_crear(NULL, 5);

    /*Inicio de pruebas*/
    prueba("El hash se creo con exito", hash != NULL);
}

/*Prueba para funcion hash_cantidad*/
void prueba_hash_cantidad(){
    printf("\nINICIO DE PRUEBAS HASH CANTIDAD\n");

    /* Declaro las variables a utilizar*/
    hash_t* hash = hash_crear(NULL, 5);

    /*Inicio de pruebas*/
    prueba("El hash se creo con exito", hash != NULL);
    prueba("El hash tiene 0 elementos", hash_cantidad(hash) == 0);
}


int main(){
    prueba_hash_crear();
    prueba_hash_cantidad();
    return 0;
}