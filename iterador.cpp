#include "info.cpp"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct nodo
{
   info_t dato;
   nodo *sig;
};

struct rep_iterador
{
   nodo *inicio, *fin, *actual;
   bool bloqueado;
};

typedef struct rep_iterador *iterador_t;

// Crea un iterador_t vacío (sin elementos) cuya posición actual no estádefinida.  No está bloqueado.
// El tiempo de ejecución es O(1).
iterador_t crear_iterador()
{
    iterador_t res = new rep_iterador;
    res->actual = res->inicio = res->fin = NULL;
    res->bloqueado = false;
    
    return res;
}

// Libera la memoria asignada a `iter' y a sus elementos.
// El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `iter'.
void liberar_iterador(iterador_t &iter)
{
    iter->actual = iter->inicio;

    while (iter->actual != NULL)
    {
      nodo *a_borrar = iter->actual;
      iter->actual = iter->actual->sig;
      liberar_info(a_borrar->dato);
      delete a_borrar;
    }

    delete iter;
} 

// Bloquea `iter'. No se podrán agregar más elementos.
// El tiempo de ejecución es O(1).
void bloquear_iterador(iterador_t &iter)
{ 
   iter->bloqueado = true; 
}

// Agrega una copia de `info' al final de `iter' si y solo si `iter' no ha sido bloqueado.
// El tiempo de ejecución es O(1).
void agregar_a_iterador(info_t info, iterador_t &iter)
{
   if (!iter->bloqueado)
   {
     nodo *nuevo = new nodo;
     nuevo->dato = copia_info(info);
     nuevo->sig = NULL;
    
     if (iter->fin != NULL)
       iter->fin->sig = nuevo;
     else
       iter->inicio = nuevo;

     iter->fin = nuevo;
   }
}

// Mueve la posición actual al primero.
// El tiempo de ejecución es O(1).
void reiniciar_iterador(iterador_t &iter)
{
   if (iter->inicio != NULL)
     iter->actual = iter->inicio;
   else
     assert(iter->actual == NULL);
}

// Avanza la posición actual hacia el siguiente.
// Si ! hay_siguiente_en_iter(iter) la posición actual queda indefinida;
// Si la posición actual no está definida no hace nada.
// El tiempo de ejecución es O(1).
void avanzar_iterador(iterador_t &iter)
{
   if (iter->actual != NULL)
      iter->actual = iter->actual->sig;
}

// Devuelve el elemento de la posición actual.
// El tiempo de ejecución es O(1).
// Precondición: esta_definida_actual(iter).
info_t actual_en_iterador(iterador_t &iter)
{
   assert(esta_definida_actual(iter));
   return (iter->actual != NULL) ? (iter->actual->dato) : NULL;
}

// Devuelve `true' si y sólo si en `iter' hay un elemento siguiente al de la posición actual.
// El tiempo de ejecución es O(1).
// Precondición: esta_definida_actual(iter).
bool hay_siguiente_en_iterador(iterador_t iter)
{
   assert(esta_definida_actual(iter));
   return (iter->actual->sig != NULL);
}

// Devuelve `true' si y sólo si la posición actual de `iter' está definida.
// El tiempo de ejecución es O(1).
bool esta_definida_actual(iterador_t iter)
{ 
   return iter->actual != NULL; 
}
