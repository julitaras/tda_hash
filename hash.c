#include "hash.h"
#include "lista.h"
#define ERROR -1
#define EXITO 0

struct hash{
    lista_t** tabla;
    hash_destruir_dato_t destructor;
    int capacidad;
    int cantidad;
};


hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){

    hash_t* hash = malloc(sizeof(hash_t));

    if(!hash){
        return NULL;
    }

    hash->capacidad = capacidad;
    hash->destructor = destruir_elemento;
    hash->cantidad = 0;
    hash->tabla = NULL;

    return hash;
}

/*
long hasheo(char* clave, int tam){
    long suma = 0;

    for(size_t i=0;clave[i]; i++){
        suma+=clave[i];
    }

    return suma;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
    if(!hash){
        return ERROR;
    }


    // factor de carga: cantidad/capacidad
    //Si la cantidad /capacidad da mas del 75% agregar capacidad.
    //Hashear la clave para guardar el elemento

    return EXITO;
}
*/

size_t hash_cantidad(hash_t* hash){
    if(!hash){
        return 0;
    }

    return hash->cantidad;
}