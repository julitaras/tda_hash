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

Se entrega el desarrollo del tda y sus respectivas pruebas, evaluando las primitivas.

### Una explicación de los diferentes tipos de hashes que conoce y cómo se resuelven las colisiones.

- HASH ABIERTO
    
- HASH CERRADO

 
### ¿Qué entiende por rehash y por qué es importante?

- Rehash, valga la rebundancia seria hashear nuestra tabla del hash nuevamente ya que se requiere mas espacio por ejemplo. Basicamente porque la capacidad deja de ser igual y como nuestra funcion de hashing depende de la misma es necesario hacer este rehash.

- Sea el caso en que no se rehashe no encontrariamos a los elementos a traves de la clave que enviemos, ya que al hashear esa clave puede que devuelva que el elemento se encuentra en una posicion que no esta o que directamente no esta.