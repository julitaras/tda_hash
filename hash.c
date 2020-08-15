#include "hash.h"
#include "lista.h"
#define ERROR -1
#define EXITO 0
#define FACTOR_CARGA 0.7

struct hash{
    lista_t** tabla;
    hash_destruir_dato_t destructor;
    int capacidad;
    int cantidad;
};

typedef struct nodo_hash{
    void* elemento;
    char* clave;
}nodo_hash_t;


/*Crea la estructura de nodo_hash_t, reservando memoria para la misma*/
nodo_hash_t* nodo_hash_crear(){

    nodo_hash_t* nodo = calloc(1, sizeof(nodo_hash_t));
    
    return nodo;
}

hash_t* hash_crear(hash_destruir_dato_t destruir_elemento, size_t capacidad){

    hash_t* hash = malloc(sizeof(hash_t));

    if(!hash){
        return NULL;
    }

    if(capacidad < 3){
        capacidad = 3;
    }

    lista_t** tabla = calloc(capacidad, sizeof(lista_t*));
	if (!tabla) {
		free(hash);
		return NULL;
	}

    for (size_t i = 0; i < capacidad; i++){
		tabla[i] = lista_crear();
	}

    hash->tabla = tabla;
    hash->capacidad = capacidad;
    hash->destructor = destruir_elemento;
    hash->cantidad = 0;

    return hash;
}


size_t hasheo(char* clave, int tam){
    long suma = 0;

    for(size_t i=0;clave[i]; i++){
        suma+=clave[i];
    }

    return suma%tam;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
    if(!hash || !clave){
        return ERROR;
    }

    size_t pos = hasheo(clave, hash->capacidad);

    if(hash_contiene(hash, clave)){
        //reemplazar el valor ?
        //si reemplazamos encima perderiamos memoria es por eso que hay que usar una func de reeemplazo
    }


    nodo_hash_t* nodo = nodo_hash_crear();

    nodo->clave = clave;
    nodo->elemento = elemento;

    int exito = lista_insertar_en_posicion(*(hash->tabla), nodo, pos);
    if(exito == ERROR){
        free(nodo);
        return ERROR;
    }

    hash->cantidad++;

    if(hash->cantidad / hash->capacidad >= FACTOR_CARGA){
        //AUMENTAR TAMAÑO Y POR LO TANTO REHASHEAR
    }

    // factor de carga: cantidad/capacidad
    // Si la cantidad /capacidad da mas del 75% agregar capacidad.
    // Hashear la clave para guardar el elemento

    return EXITO;
}

bool hash_contiene(hash_t* hash, const char* clave){
    if(!hash){
        return false;
    }

    size_t pos = hasheo(clave, hash->capacidad);

    nodo_hash_t* elem = lista_elemento_en_posicion(hash->tabla, pos);
    if(!elem){
        return false;
    }

    return true;
}

void* hash_obtener(hash_t* hash, const char* clave){
    if(!hash){
        return NULL;
    }

    size_t pos = hasheo(clave, hash->capacidad);

    nodo_hash_t* elem = lista_elemento_en_posicion(hash->tabla, pos);

    return elem;
}

size_t hash_cantidad(hash_t* hash){
    if(!hash){
        return 0;
    }

    return hash->cantidad;
}

void hash_destruir(hash_t* hash){
    if(!hash){
        return;
    }

    //ver bien como destruir en la lista 

	free(hash);
}