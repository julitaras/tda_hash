#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ERROR -1
#define EXITO 0
#define FACTOR_CARGA 0.7

struct hash{
    lista_t** tabla;
    hash_destruir_dato_t destructor;
    size_t capacidad;
    size_t cantidad;
};

typedef struct nodo_hash{
    void* elemento;
    char* clave;
}nodo_hash_t;

/*
* Destructor de la tabla hash
*/
void hash_destruir_tabla(lista_t** tabla_hash, size_t tamanio){
	for(size_t i=0; i<tamanio; i++){
		lista_destruir(tabla_hash[i]);
    }

	free(tabla_hash);	
}

/*
* Funcion que dice si un numero es primo
*/
bool es_primo(int n) {
    if (n <= 1){
        return false;
    }
    
    if (n <= 3){
        return true;
    }
    
    if (n % 2 == 0 || n % 3 == 0){
        return false;
    }

    for (int i = 5; i * i <= n; i = i + 6){
        if (n % i == 0 || n % (i + 2) == 0){
           return false; 
        }
    }
    return true;
}

/*
* Funcion que recibe un numero por parametro 
* e indica el proximo primo a ese numero
*/
size_t proximo_primo(int n) {
    if ((n % 2) == 0){
        n++;
    }

    while (!es_primo(n)){
        n += 2;
    }
    return (size_t)n;
}

/*
* Funcion que se encarga de agregar la clave al heap
*/
char* hash_duplicar_clave(const char *palabra) {
    
    char* copia = malloc(strlen(palabra) + 1);
    if (!copia){
        return NULL;
    }
    
    strcpy(copia, palabra);
    
    return copia;
}

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

    lista_t** tabla = calloc(proximo_primo((int)capacidad), sizeof(lista_t*));
	if (!tabla) {
		free(hash);
		return NULL;
	}

    for (size_t i = 0; i < capacidad; i++){
		tabla[i] = lista_crear();

        if(!tabla[i]){
            hash_destruir_tabla(tabla, capacidad);
        }
	}

    hash->tabla = tabla;
    hash->capacidad = capacidad;
    hash->destructor = destruir_elemento;
    hash->cantidad = 0;

    return hash;
}

/*Funcion de hasheo*/
size_t hasheo(const char* clave, size_t tam){
    char* suma = NULL;
    size_t codigo = 0;

    for(size_t i=0;clave[i]; i++){
       // codigo = clave[i];
        suma+=clave[i];
    }
    codigo = (size_t)suma;

    return codigo%tam;
}

/*
* Funcion que se encarga de redimensionar el hash y por lo tanto, 
* rehashear los elementos que estaban guardados en la tabla  
* para asi poder guardarlos en la nueva tabla del hash
*/
int hash_redimensionar(hash_t* hash){

    lista_iterador_t* iterador;
	
    size_t nuevo_tam = proximo_primo((int)hash->capacidad);
	
    lista_t** nueva_tabla = calloc(nuevo_tam, sizeof(lista_t*));
	if (!nueva_tabla){
        return ERROR;
    }
	
	for (size_t i = 0; i < nuevo_tam; i++) {
		nueva_tabla[i] = lista_crear();

        if(!nueva_tabla[i]){
            hash_destruir_tabla(nueva_tabla, nuevo_tam);
        }
	}
	
	for(size_t i=0; i < hash->capacidad; i++){	
        
        iterador = lista_iterador_crear(hash->tabla[i]);
        
        while(lista_iterador_tiene_siguiente(iterador)){
			nodo_hash_t* nodo = lista_iterador_siguiente(iterador);
			size_t pos_nueva_tabla = hasheo(nodo->clave, nuevo_tam);
			
            if(lista_insertar(nueva_tabla[pos_nueva_tabla], nodo) == ERROR){
                hash_destruir_tabla(nueva_tabla, nuevo_tam);
                lista_iterador_destruir(iterador);

                return ERROR;
			}
		}
		lista_iterador_destruir(iterador);
	}
	
    hash_destruir_tabla(hash->tabla, hash->capacidad);
		
	hash->capacidad = nuevo_tam;
	hash->tabla = nueva_tabla;

	return EXITO;
}

/*
* Devuelve un nodo del hash con la clave dada o NULL si dicho
* nodo no existe (o en caso de error).
*/
nodo_hash_t* hash_obtener_nodo(hash_t* hash, const char* clave){
    if(!hash || !clave){
        return NULL;
    }
    size_t pos = hasheo(clave, hash->capacidad);

    lista_iterador_t* iterador = lista_iterador_crear(hash->tabla[pos]);
    
    if(!iterador){
       return NULL;
    }

    while(lista_iterador_tiene_siguiente(iterador)){
        nodo_hash_t* nodo = lista_iterador_siguiente(iterador);

        if(!nodo){
            lista_iterador_destruir(iterador);
            
            return NULL;
        }

        if(strcmp(nodo->clave, clave) == 0){
            lista_iterador_destruir(iterador);

            return nodo;
        }
    }

    lista_iterador_destruir(iterador);

    return NULL;
}

/*
* Se encarga de reemplzar el valor en el nodo del hash
*/
int hash_reemplazar_valor(hash_t *hash, const char *clave, void *elemento) {
    if(!hash){
        return ERROR;
    }

    nodo_hash_t* nodo = hash_obtener_nodo(hash, clave);
    if(!nodo){
        return ERROR;
    }

    if(hash->destructor){
        hash->destructor(nodo->elemento);
    }

    nodo->elemento = elemento;

    return EXITO;
}

int hash_insertar(hash_t* hash, const char* clave, void* elemento){
    if(!hash || !clave){
        return ERROR;
    }

    if(hash->cantidad / hash->capacidad >= FACTOR_CARGA){
        return hash_redimensionar(hash);
    }

    size_t pos = hasheo(clave, hash->capacidad);

    if(hash_contiene(hash, clave)){
        return hash_reemplazar_valor(hash, clave, elemento);
    }

    nodo_hash_t* nodo = nodo_hash_crear();

    nodo->elemento = elemento;
    nodo->clave = hash_duplicar_clave(clave);

    int exito = lista_insertar(hash->tabla[pos], nodo);
    if(exito == ERROR){
        free(nodo);
        return ERROR;
    }

    hash->cantidad++;

    return EXITO;
}

bool hash_contiene(hash_t* hash, const char* clave){
   if(!hash || !clave){
        return false;
    }
 
    nodo_hash_t* nodo = hash_obtener_nodo(hash, clave);
    if(!nodo){
        return false;
    }

    return true;
}

void* hash_obtener(hash_t* hash, const char* clave){
    if(!hash || !clave){
        return NULL;
    }

    nodo_hash_t* nodo = hash_obtener_nodo(hash, clave);
    if(!nodo){
        return NULL;
    }

    return nodo->elemento;
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

	for(size_t i = 0; i < hash->capacidad; i++){

		while(hash->tabla[i] != NULL && !lista_vacia(hash->tabla[i])){
			nodo_hash_t* nodo = lista_ultimo(hash->tabla[i]);
			
            if(hash->destructor){
				hash->destructor(nodo->elemento);
            }

            free(nodo->clave);
			free(nodo);
			
            lista_borrar(hash->tabla[i]);
		}

		lista_destruir(hash->tabla[i]);
	}

	free(hash->tabla);
	free(hash);
}

/*
* Funcion que se encarga de obtener la 
* posicion del elemento dentro de la lista
*/
size_t hash_obtener_pos_lista(hash_t* hash, const char* clave){
    size_t pos_lista = 0;
    
    if(!hash || !clave){
        return pos_lista;
    }
    size_t pos = hasheo(clave, hash->capacidad);
    
    lista_iterador_t* iterador = lista_iterador_crear(hash->tabla[pos]);
    
    if(!iterador){
       return pos_lista;
    }

    while(lista_iterador_tiene_siguiente(iterador)){
        nodo_hash_t* nodo = lista_iterador_siguiente(iterador);

        if(!nodo){
            lista_iterador_destruir(iterador);

            return pos_lista;
        }
        
        if(strcmp(nodo->clave, clave) == 0){
            lista_iterador_destruir(iterador);
            
            return pos_lista;
        }
        pos_lista++;
    }

    lista_iterador_destruir(iterador);

    return pos_lista;
}

int hash_quitar(hash_t* hash, const char* clave){
    if(!hash || !clave){
        return ERROR;
    }

    size_t pos = hasheo(clave, hash->capacidad);

    nodo_hash_t* nodo_eliminar = hash_obtener_nodo(hash, clave);

    if(!nodo_eliminar){
        return ERROR;
    }

    int exito = lista_borrar_de_posicion(hash->tabla[pos], hash_obtener_pos_lista(hash, clave));
    if(exito == ERROR){
        return ERROR;
    }

    if(hash->destructor){
        hash->destructor(nodo_eliminar->elemento);
    }
    
    free(nodo_eliminar->clave);
    free(nodo_eliminar);

    hash->cantidad--;

    return EXITO;
}