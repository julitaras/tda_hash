<h1 align="center">TDA hash</h3>

---

<p align="center"> Desarrollo de TDA hash abierto
    <br> 
</p>

## 📝 Contenido

- [Enunciado](#enunciado)
- [Compilacion y Ejecucion](#compilacion_ejecucion)
- [FAQS](#faqs)


## Enunciado <a name = "enunciado"></a>

Se pide implementar un TDA Hash. Para ello se brindan las firmas de las funciones públicas a implementar y se deja a criterio del alumno tanto la creación de las funciones privadas del TDA para el correcto funcionamiento del Hash cumpliendo con las buenas prácticas de programación. El Hash a implementar debe ser abierto y cumplir con los requisitos detallados en el .h .

Adicionalmente se pide la creación de un TDA iterador externo para el recorrido de las claves almacenadas en el Hash.

## 🏁 Compilacion y Ejecucion <a name = "compilacion_ejecucion"></a>

Compilamos:

````c
gcc lista.c hash.c hash_pruebas.c -o hash -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
````

Ejecutamos:

````c
 ./hash
````

##  FAQS <a name = "faqs"></a>

### ¿Qué se esta entregando?

Se entrega el desarrollo del tda y sus respectivas pruebas, evaluando todas las primitivas, puede observar --> [pruebas](hash_pruebas.c)

### Una explicación de los diferentes tipos de hashes que conoce y cómo se resuelven las colisiones.

 
### ¿Qué entiende por rehash y por qué es importante?





>Que es un hash?

>Nos sirve para almacenar datos, estructura que nos sirve para almacenar datos y nos permite guardar el elemento en una posicion determinada.

>Podemos acceder a la posicion que tenemos que almacenarlo casi directamente.

