#include "info.cpp"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>

typedef unsigned int nat;

struct nodo
{
   info_t dato;
   nodo *anterior;
   nodo *siguiente;
};

typedef struct nodo *localizador_t;

// Lista doblemente enlazada de elementos de tipo `info_t', con cabezal con punteros al inicio y al final.
struct rep_cadena 
{
   nodo *inicio;
   nodo *final;
};

typedef struct rep_cadena *cadena_t;

// Devuelve la cadena_t vacía (sin elementos).
// El tiempo de ejecución es O(1).
cadena_t crear_cadena()
{
   cadena_t res = new rep_cadena;
   res->inicio = res->final = NULL;

   return res;
}

// Se inserta `i' como último elemento de `cad'. Devuelve `cad'.
// Si es_vacia_cadena (cad) `i' se inserta como único elemento de `cad'.
// El tiempo de ejecución es O(1).
cadena_t insertar_al_final(info_t i, cadena_t cad)
{
  nodo *nuevo = new nodo;
  nuevo->dato= i;
  nuevo->siguiente = NULL;
  nuevo->anterior = cad->final;
  if (cad->final == NULL)
  {
     assert(cad->inicio == NULL);
	   cad->inicio = nuevo;
  }
  else
  {
     assert(cad->inicio != NULL);
	   cad->final->siguiente = nuevo;
  }
  cad->final = nuevo;
  
  return cad;
}

// Se inserta `i' como un nuevo elemento inmediatamente antes de `loc'. Devuelve `cad'.
// El tiempo de ejecución es O(1).
// Precondición: localizador_en_cadena(loc, cad).
cadena_t insertar_antes(info_t i, localizador_t loc, cadena_t cad)
{
    nodo *nuevoNodo = new nodo;
    nuevoNodo->dato = i;
    nuevoNodo->siguiente = NULL;
    nuevoNodo->anterior = NULL;

    if (es_vacia_cadena(cad))
    {
       cad->inicio = cad->final = nuevoNodo;
    }
    else
    {
       if (inicio_cadena(cad) == loc)
       {
          loc->anterior = nuevoNodo;
          nuevoNodo->siguiente = loc;
          cad->inicio = nuevoNodo;
       } 
       else
       {
          loc->anterior->siguiente = nuevoNodo;
          nuevoNodo->anterior = loc->anterior;
          loc->anterior = nuevoNodo;
          nuevoNodo->siguiente = loc;
       }
    }

    return cad;
}

// Se inserta la cadena_t `sgm' inmediatamente después de `loc' en `cad', manteniendo los elementos originales y 
// el orden relativo entre ellos. Devuelve `cad'. Si es_vacia_cadena(cad), `loc' es ignorado y el segmento queda insertado.
// El tiempo de ejecución es O(1).
// Precondición: es_vacia_cadena(cad) o localizador_en_cadena(loc, cad).
cadena_t insertar_segmento_despues(cadena_t &sgm, localizador_t loc, cadena_t cad)
{
    assert(es_vacia_cadena(cad) || localizador_en_cadena(loc,cad));
    if (es_vacia_cadena(cad))
    {
      cad->inicio = sgm->inicio;
      cad->final = sgm->final;
    }
    else if (!es_vacia_cadena(sgm))
    {
       sgm->inicio->anterior = loc;
       sgm->final->siguiente = loc->siguiente;
       if (es_final_cadena(loc,cad))
         cad->final = sgm->final;
       else loc->siguiente->anterior = sgm->final;
      
       loc->siguiente = sgm->inicio;
    }
    sgm->inicio = sgm->final = NULL;

    return cad;
}

// Devuelve una cadena_t con los elementos de `cad' que se encuentran entre `desde' y `hasta', incluidos.
// Si es_vacia_cadena(cad) devuelve la cadena_t vacia.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena resultado.
// Precondición: es_vacia_cadena(cad) o precede_en_cadena(desde, hasta, cad).
cadena_t copiar_segmento(localizador_t desde, localizador_t hasta, cadena_t cad)
{
  assert(es_vacia_cadena(cad) || precede_en_cadena(desde,hasta,cad));
  cadena_t res=crear_cadena();
  if (!es_vacia_cadena(cad))
  {
    localizador_t loc = desde;
	  while (loc != siguiente(hasta,cad))
    {
       info_t info = copia_info((loc->dato));
       insertar_al_final(info,res);
       loc = siguiente(loc,cad);
    }
  }

  return res;
}

// Remueve de `cad' los elementos que se encuentran  entre `desde' y `hasta', incluidos y libera la memoria que tenían asignada y 
// la de sus nodos. Devuelve `cad'.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena resultado.
// Precondición: es_vacia_cadena(cad) o precede_en_cadena(desde, hasta, cad).
cadena_t cortar_segmento(localizador_t desde, localizador_t hasta, cadena_t cad)
{
    if (es_vacia_cadena(cad))
       return cad;

    if (es_inicio_cadena(desde, cad) && es_final_cadena(hasta, cad))
    {
       cad->inicio = NULL;
       cad->final = NULL;
    } 
    else if (es_inicio_cadena(desde, cad))
    {
       cad->inicio = hasta->siguiente;
	     if (cad->inicio !=NULL)
          cad->inicio->anterior = NULL;
    }
    else if (es_final_cadena(hasta, cad))
    {
       cad->final = desde->anterior;
       if (cad->final !=NULL)
          cad->final->siguiente = NULL;
    }
    else
    {
       desde->anterior->siguiente = hasta->siguiente;
       hasta->siguiente->anterior = desde->anterior;
    }
    desde->anterior = NULL;
    hasta->siguiente = NULL;

    cadena_t cadenaAux = crear_cadena();
    cadenaAux->inicio = desde;
    cadenaAux->final = hasta;

    cadenaAux = liberar_cadena(cadenaAux);

    return cad;
}

// Se quita el elemento al que se accede desde `loc' y se libera la memoria asignada al mismo y al nodo apuntado por el localizador.
// Devuelve `cad'. El valor de `loc' queda indeterminado.
// El tiempo de ejecución es O(1).
// Precondición: localizador_en_cadena(loc, cad).
cadena_t remover_de_cadena(localizador_t &loc, cadena_t cad)
{
    if(es_vacia_cadena(cad))
      return cad;
    
    if(es_inicio_cadena(loc, cad) && es_final_cadena(loc, cad))
    {
        cad->inicio = NULL;
        cad->final = NULL;
    }
    else if (es_inicio_cadena(loc, cad))
    {
        cad->inicio = loc->siguiente;
        loc->siguiente->anterior = NULL;
        loc->siguiente = NULL;
    }
    else if (es_final_cadena(loc, cad))
    {
        cad->final = loc->anterior;
        loc->anterior->siguiente = NULL;
        loc->anterior = NULL;
    }
    else
    {
        loc->anterior->siguiente = loc->siguiente;
        loc->siguiente->anterior = loc->anterior;
    }
    
    liberar_info(loc->dato);
    delete loc;
    loc = NULL;
    
    return cad;
}

// Libera la memoria asignada a `cad' y la de todos sus elementos. Devuelve `cad'.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena.
cadena_t liberar_cadena(cadena_t cad)
{
   localizador_t iterador = inicio_cadena(cad);
    while (es_localizador(iterador))
    {
        nodo *a_borrar = iterador;
        cad->inicio = iterador->siguiente;
        iterador = cad->inicio;
        liberar_info(a_borrar->dato); 
        delete (a_borrar);
    }
    delete cad;

    return cad; 
}

// Devuelve `true' si y sólo si `loc' es un localizador_t válido.
// En cadenas enlazadas un localizador_t no es válido si es `NULL'.
// El tiempo de ejecución es O(1).
bool es_localizador(localizador_t loc)
{
   return loc != NULL;
}

// Devuelve `true' si y sólo si `cad' es vacía (no tiene elementos).
// El tiempo de ejecución es O(1).
bool es_vacia_cadena(cadena_t cad)
{
   return (cad->inicio == NULL) && (cad->final == NULL);
}

// Devuelve `true' si y sólo si con `loc' se accede al último elemento de `cad'.
// Si es_vacia_cadena (cad) devuelve `false'.
// El tiempo de ejecución es O(1).
bool es_final_cadena(localizador_t loc, cadena_t cad)
{
  if (es_vacia_cadena(cad))
    return false;
  
  return cad->final == loc;
}

// Devuelve `true' si y sólo si con `loc' se accede al primer elemento de `cad'.
// Si es_vacia_cadena (cad) devuelve `false'.
// El tiempo de ejecución es O(1).
bool es_inicio_cadena(localizador_t loc, cadena_t cad)
{
   if (es_vacia_cadena(cad))
    return false;

   return cad->inicio == loc; 
}

// Devuelve `true' si y sólo si con `loc' se accede a un elemento de `cad' (o sea, si apunta a un nodo de `cad').
// Si es_vacia_cadena (cad) devuelve `false'.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena.
bool localizador_en_cadena(localizador_t loc, cadena_t cad)
{
  localizador_t cursor = inicio_cadena(cad);
  while (es_localizador(cursor) && (cursor !=loc))
      cursor = siguiente(cursor,cad);
  
  return es_localizador(cursor);
}

// Devuelve `true' si y sólo si `loc1' es igual o precede a `loc2` en `cad`.
// Si es_vacia_cadena (cad) devuelve `false'.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la scadena.
bool precede_en_cadena(localizador_t loc1, localizador_t loc2, cadena_t cad)
{
  if (es_vacia_cadena(cad))
     return false;
  if (loc1 == loc2)
     return true;

  while(es_localizador(loc1) && loc1 != loc2)
  {
     loc1 = loc1->siguiente;
  }    
    
  return loc1 == loc2;
}

// Devuelve el localizador_t con el que se accede al inicio de `cad`.
// Si es_vacia_cadena(cad) devuelve un localizador_t no válido.
// El tiempo de ejecución es O(1).
localizador_t inicio_cadena(cadena_t cad)
{
   if (es_vacia_cadena(cad))
      return NULL;
    
   return cad->inicio;
}

// Devuelve el localizador_t con el que se accede al final de `cad'.
// Si es_vacia_cadena(cad) devuelve un localizador_t no válido.
// El tiempo de ejecución es O(1).
localizador_t final_cadena(cadena_t cad)
{
  if (es_vacia_cadena(cad))
    return NULL;

  return cad->final;
}

// Devuelve el localizador_t con el que se accede al k-esimo elemento de `cad'.
// Si `k' es 0 o mayor a la cantidad de elementos de `cad' devuelve un localizdor no válido.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena.
localizador_t kesimo(nat k, cadena_t cad)
{
    if (k==0)
      return NULL;
  
    localizador_t iterador = cad->inicio;
    nat contador = 1;
    while(es_localizador(iterador) && contador != k)
    {
        contador++;
        iterador = iterador->siguiente;
    }

    if (contador == k)
       return iterador;
    
    return NULL;
}

// Devuelve el localizador_t con el que se accede al elemento de `cad'
// inmediatamente siguiente a `loc'.
// Si es_final_cadena(loc, cad) devuelve un localizador_t no válido.
// El tiempo de ejecución es O(1).
// Precondición: localizador_en_cadena(loc, cad).
localizador_t siguiente(localizador_t loc, cadena_t cad)
{
  if (es_final_cadena(loc, cad))
     return NULL;
  
  return loc->siguiente;
}

// Devuelve el localizador_t con el que se accede al elemento de `cad'
// inmediatamente anterior a `loc'.
// Si es_inicio_cadena(loc, cad) devuelve un localizador_t no válido.
// El tiempo de ejecución es O(1).
// Precondición: localizador_en_cadena(loc, cad).
localizador_t anterior(localizador_t loc, cadena_t cad)
{
   if (es_inicio_cadena(loc, cad))
     return NULL;
  
   return loc->anterior;
}

// Devuelve el localizador_t con el que se accede al elemento cuyo dato numérico es el menor en el segmento 
// que va desde `loc' hasta el final_cadena(cad).
// Si hay más de un elemento cuyo valor es el menor el resultado accede al que precede a los otros.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena.
// Precondición: localizador_en_cadena(loc, cad).
localizador_t menor_en_cadena(localizador_t loc,cadena_t cad)
{
    assert(localizador_en_cadena(loc,cad));
    localizador_t res = loc;
    while (es_localizador(siguiente(loc, cad)))
    {
      loc = siguiente(loc,cad);
      if (numero_info(info_cadena(loc,cad)) <  numero_info(info_cadena(res,cad)))
        res = loc;
    }

    return res;
}

// Devuelve el primer localizador_t con el que se accede a un elemento cuyo dato numérico es igual a `clave', 
// buscando desde `loc' (inclusive) hacia el final
// Si no se encuentra o `cad' es vacía devuelve un localizador_t no válido.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena.
// Precondición: es_vacia_cadena(cad) o localizador_en_cadena(loc, cad).
localizador_t siguiente_clave(int clave, localizador_t loc, cadena_t cad)
{
  assert(es_vacia_cadena(cad) || localizador_en_cadena(loc, cad));
  
  if (es_vacia_cadena(cad))
     return NULL;
  
  localizador_t res = loc;
  while (es_localizador(res) && numero_info(info_cadena(res, cad)) != clave)
       res = siguiente(res, cad);  
  
  return res;
}

// Devuelve el elemento de `cad' al que se accede con `loc'.
// El tiempo de ejecución es O(1).
// Precondición: localizador_en_cadena(loc, cad).
info_t info_cadena(localizador_t loc, cadena_t cad)
{
   return loc->dato;
}

// Sustituye con `i' el elemento de `cad' al que se accede con `loc'. Devuelve `cad'.
// No destruye el elemento al que antes se accedía con `loc'.
// El tiempo de ejecución es O(1).
// Precondición: localizador_en_cadena(loc, cad).
cadena_t cambiar_en_cadena(info_t i,localizador_t loc, cadena_t cad)
{
    assert(localizador_en_cadena(loc,cad));
    loc->dato = i;

    return cad;
}

// Intercambia los elementos a los que se accede con `loc1' y `loc2'.
// `loc1' y `loc2' mantienen su relación de precedencia. Devuelve `cad'.
// El tiempo de ejecución es O(1).
// Precondición: localizador_en_cadena (loc1, cad) y localizador_en_cadena (loc2, cad).
cadena_t intercambiar(localizador_t loc1, localizador_t loc2, cadena_t cad)
{
    info_t datoloc1 = info_cadena(loc1,cad);
    loc1->dato = info_cadena(loc2,cad);
    loc2->dato = datoloc1;
    
    return cad;
}

// Imprime los elementos de `cad` de la siguiente forma: (dn de pos1,fr de pos1)(dn de pos2,fr de pos2) ...
// donde `dn` es el dato numérico y `fr` es la frase.
// Antes de terminar, se debe imprimir un fin de linea. Si es_vacia_cadena(cad) sólo se imprime el fin de línea.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena.
void imprimir_cadena(cadena_t cad)
{
    localizador_t iterador = inicio_cadena(cad);
    while(es_localizador(iterador))
    {
        printf("(%d", numero_info(iterador->dato));
        printf(",");
        printf("%s)", frase_info(iterador->dato));
        
        iterador = siguiente(iterador, cad);
    }

    printf("\n");
}
