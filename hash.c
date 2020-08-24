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

/*Funcion de hasheo*/
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

    if(hash->cantidad / hash->capacidad >= FACTOR_CARGA){
        //AUMENTAR TAMAÑO Y POR LO TANTO REHASHEAR
    }

    size_t pos = hasheo(clave, hash->capacidad);

    if(hash_contiene(hash, clave)){
        return hash_reemplazar_valor(hash, clave, elemento);
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

    lista_iterador_t* iterador = lista_iterador_crear(hash->tabla[pos]);
    if(!iterador){
       return NULL;
    }

    while(lista_iterador_tiene_siguiente(iterador)){
        nodo_hash_t* nodo = lista_iterador_siguiente(iterador);
        if(strcmp(nodo->clave, clave) == 0){
            lista_iterador_destruir(iterador);

            return true;
        }
    }

    lista_iterador_destruir(iterador);
    
    return false;
}

void* hash_obtener(hash_t* hash, const char* clave){
    if(!hash){
        return NULL;
    }

    size_t pos = hasheo(clave, hash->capacidad);

   lista_iterador_t* iterador = lista_iterador_crear(hash->tabla[pos]);
   
   if(!iterador){
       return NULL;
    }

    while(lista_iterador_tiene_siguiente(iterador)){
        nodo_hash_t* nodo = lista_iterador_siguiente(iterador);
        
        if(strcmp(nodo->clave, clave) == 0){
            lista_iterador_destruir(iterador);
            
            return nodo;
        }
    }

    lista_iterador_destruir(iterador);

    return NULL;
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

/*
* SE encarga de reemplzar el valor en el nodo del hash
*/
int hash_reemplazar_valor(hash_t *hash, const char *clave, void *elemento) {
    if(!hash){
        return ERROR;
    }

    nodo_hash_t* nodo = hash_obtener(hash, clave);
    if(!nodo){
        return ERROR;
    }

    if(hash->destructor){
        hash->destructor(nodo->elemento);
    }

    nodo->elemento = elemento;

    return EXITO;
}

  //  FIJARSE BIEN TEMA DE LOS PRIMOS 

bool hash_redimensionar(hash_t* hash) {
	// ELEGIR NUEVO TAMAÑO
	size_t nuevo_tamanio = 1;
	lista_t** nueva_tabla = calloc(nuevo_tamanio, sizeof(lista_t*));
	if (!nueva_tabla) return false;
	
	// Creo una nueva lista por cada posición de la nueva tabla
	for (size_t i = 0; i < nuevo_tamanio; i++) {
		nueva_tabla[i] = lista_crear();
	}
	// Saco los nodos del hash anterior, los vuelvo a hashear y los inserto
	// en el nuevo hash
	for (size_t i = 0; i < hash->capacidad; i++){
		while (!lista_esta_vacia(hash->tabla[i])){
			nodo_hash_t* nodo = lista_borrar_primero(hash->tabla[i]);
			size_t pos_vect = fhash(nodo->clave, nuevo_tamanio);
			lista_insertar_primero(nueva_tabla[pos_vect], nodo);
		}
		// Destruyo las listas del hash anterior
		lista_destruir(hash->tabla[i]);
	}
	
	free(hash->tabla);
	hash->tabla = nueva_tabla;
	hash->capacidad = nuevo_tamanio;
	return true;
}
