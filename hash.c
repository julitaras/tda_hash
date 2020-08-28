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

/*
* Destructor de la tabla hash
*/
void destruir_tabla(lista_t** tabla_hash, size_t tamanio){
	for(size_t i=0; i<tamanio; i++){
		lista_destruir(tabla_hash[i]);
    }

	free(tabla_hash);	
}

/*
* Funcion que dice si un numero es primo
*/
bool es_primo(size_t n) {
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
size_t proximo_primo(size_t n) {
    if ((n % 2) == 0){
        n++;
    }

    while (!es_primo(n)){
        n += 2;
    }
    return n;
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

    lista_t** tabla = calloc(proximo_primo(capacidad), sizeof(lista_t*));
	if (!tabla) {
		free(hash);
		return NULL;
	}

    for (size_t i = 0; i < capacidad; i++){
		tabla[i] = lista_crear();

        if(!tabla[i]){
            destruir_tabla(tabla, capacidad);
        }
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

/*
* Funcion que se encarga de redimensionar el hash y por lo tanto, 
* rehashear los elementos que estaban guardados en la tabla  
* para asi poder guardarlos en la nueva tabla del hash
*/
int hash_redimensionar(hash_t* hash){

    lista_iterador_t* iterador;
	
    size_t nuevo_tam = proximo_primo(2*hash->capacidad);
	
    lista_t** nueva_tabla = calloc(nuevo_tam, sizeof(lista_t*));
	if (!nueva_tabla){
        return ERROR;
    }
	
	for (size_t i = 0; i < nuevo_tam; i++) {
		nueva_tabla[i] = lista_crear();

        if(!nueva_tabla[i]){
            destruir_tabla(nueva_tabla, nuevo_tam);
        }
	}
	
	for(size_t i=0; i < hash->capacidad; i++){	
        
        iterador = lista_iterador_crear(hash->tabla[i]);
        
        while(lista_iterador_tiene_siguiente(iterador)){
			nodo_hash_t* nodo = lista_iterador_siguiente(iterador);
			size_t pos_nueva_tabla = hasheo(nodo->clave, nuevo_tam);
			
            if(lista_insertar(nueva_tabla[pos_nueva_tabla], nodo) == ERROR){
                destruir_tabla(nueva_tabla, nuevo_tam);
                lista_iterador_destruir(iterador);

                return ERROR;
			}
		}
		lista_iterador_destruir(iterador);
	}
	
    destruir_tabla(hash->tabla, hash->capacidad);
		
	hash->capacidad = nuevo_tam;
	hash->tabla = nueva_tabla;

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

    nodo->clave = clave;
    nodo->elemento = elemento;

    int exito = lista_insertar_en_posicion(*(hash->tabla), nodo, pos);
    if(exito == ERROR){
        free(nodo);
        return ERROR;
    }

    hash->cantidad++;

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
* Se encarga de reemplzar el valor en el nodo del hash
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