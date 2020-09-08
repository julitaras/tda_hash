#include "hash.h"
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

    hash_destruir(hash);
}

/*Prueba para funcion hash_cantidad*/
void prueba_hash_cantidad(){
    printf("\nINICIO DE PRUEBAS HASH CANTIDAD\n");

    /* Declaro las variables a utilizar*/
    hash_t* hash = hash_crear(NULL, 5);
    int a = 1, b = 2, c = 3;

    /*Inicio de pruebas*/
    prueba("El hash se creo con exito", hash != NULL);
    prueba("El hash tiene 0 elementos", hash_cantidad(hash) == 0);
    prueba("Se inserta un elemento al hash", hash_insertar(hash, "clave1", &a) == 0);
    prueba("El hash tiene 1 elemento", hash_cantidad(hash) == 1);
    prueba("Se inserta un elemento al hash con la misma clave que el anterior", hash_insertar(hash, "clave1", &b) == 0);
    prueba("El hash tiene 1 elemento", hash_cantidad(hash) == 1);
    prueba("Se inserta un otro elemento al hash", hash_insertar(hash, "clave2", &c) == 0);
    prueba("El hash tiene 2 elementos", hash_cantidad(hash) == 2);
    prueba("Se quita un elemento al hash", hash_quitar(hash, "clave1") == 0);
    prueba("El hash tiene 1 elemento", hash_cantidad(hash) == 1);
    prueba("Se quita un elemento al hash", hash_quitar(hash, "clave2") == 0);
    prueba("El hash tiene 0 elemento", hash_cantidad(hash) == 0);

    hash_destruir(hash);
}

/*Prueba para funcion hash_contiene*/
void prueba_hash_contiene(){
    printf("\nINICIO DE PRUEBAS HASH CONTIENE\n");

    /* Declaro las variables a utilizar*/
    hash_t* hash = hash_crear(NULL, 5);
    int a = 1, b = 2, c = 3, d = 4;

    /*Inicio de pruebas*/
    prueba("El hash se creo con exito", hash != NULL);
    prueba("Se inserta un elemento al hash", hash_insertar(hash, "clave1", &a) == 0);
    prueba("Obtengo el elemento insertado", hash_contiene(hash, "clave1"));
    prueba("Se inserta otro elemento al hash", hash_insertar(hash, "clave2", &b) == 0);
    prueba("Obtengo el elemento insertado", hash_contiene(hash, "clave2"));
    prueba("Se inserta otro elemento al hash", hash_insertar(hash, "clave3", &c) == 0);
    prueba("Obtengo el elemento insertado", hash_contiene(hash, "clave3"));
    prueba("Se inserta otro elemento que tiene la misma clave que el anterior al hash", hash_insertar(hash, "clave3", &d) == 0);
    prueba("Obtengo el elemento insertado", hash_contiene(hash, "clave3"));
    prueba("Intento obtener un elemento que no esta en el hash", !hash_contiene(hash, "clave4"));
    prueba("Se quita un elemento al hash", hash_quitar(hash, "clave2") == 0);
    prueba("Intento obtener un elemento que ya no esta mas en el hash", !hash_contiene(hash, "clave2"));

    hash_destruir(hash);
}


/*Prueba para funcion hash_obtener*/
void prueba_hash_obtener(){
    printf("\nINICIO DE PRUEBAS HASH OBTENER\n");

    /* Declaro las variables a utilizar*/
    hash_t* hash = hash_crear(NULL, 5);
    int a = 1, b = 2, c = 3 , d = 4;

    /*Inicio de pruebas*/
    prueba("El hash se creo con exito", hash != NULL);
    prueba("Se inserta un elemento al hash", hash_insertar(hash, "clave1", &a) == 0);
    prueba("Obtengo el elemento insertado", hash_obtener(hash, "clave1") == &a);
    prueba("Se inserta otro elemento al hash", hash_insertar(hash, "clave2", &b) == 0);
    prueba("Obtengo el elemento insertado", hash_obtener(hash, "clave2") == &b);
    prueba("Se inserta otro elemento al hash", hash_insertar(hash, "clave3", &c) == 0);
    prueba("Obtengo el elemento insertado", hash_obtener(hash, "clave3") == &c);
    prueba("Se inserta otro elemento que tiene la misma clave que el anterior al hash", hash_insertar(hash, "clave3", &d) == 0);
    prueba("Obtengo el elemento insertado", hash_obtener(hash, "clave3") == &d);

    hash_destruir(hash);
}

/*Prueba para funcion hash_quitar*/
void prueba_hash_quitar(){
    printf("\nINICIO DE PRUEBAS HASH QUITAR\n");

    /* Declaro las variables a utilizar*/
    hash_t* hash = hash_crear(NULL, 5);
    int a = 1, b = 2, c = 3 , d = 4;

    /*Inicio de pruebas*/
    prueba("El hash se creo con exito", hash != NULL);
    prueba("Se inserta un elemento al hash", hash_insertar(hash, "clave1", &a) == 0);
    prueba("Se inserta otro elemento al hash", hash_insertar(hash, "clave2", &b) == 0);
    prueba("Se inserta otro elemento al hash", hash_insertar(hash, "clave3", &c) == 0);
    prueba("Se inserta otro elemento al hash", hash_insertar(hash, "clave4", &d) == 0);
    prueba("Se quita un elemento al hash", hash_quitar(hash, "clave2") == 0);
    prueba("Se quita un elemento al hash", hash_quitar(hash, "clave1") == 0);
    prueba("Se quita un elemento al hash", hash_quitar(hash, "clave3") == 0);
    prueba("Se quita un elemento al hash", hash_quitar(hash, "clave4") == 0);

    hash_destruir(hash);
}

int main(){
    prueba_hash_crear();
    prueba_hash_cantidad();
    prueba_hash_quitar();
    prueba_hash_contiene();
    prueba_hash_obtener();
    return 0;
}