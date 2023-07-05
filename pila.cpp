#include <stdlib.h> 
#include <assert.h>

#define CAP_PILA 50

// Declaración del tipo `pila_t. Es de tipo puntero a `rep_pila'.
// struct rep_pila es la representación de `pila_t'
struct rep_pila
{
   // el rango del arreglo es desde 0 hasta CAP_PILA - 1
   char datos[CAP_PILA];
   // cantidad indica la cantidad de elementos = posición donde se insertaria el nuevo
   int cantidad;
};

typedef struct rep_pila *pila_t;

// Devuelve un elemento de tipo `pila_t' vacío (sin elementos) que puede mantener hasta `CAP_PILA' elementos.
pila_t crear_pila()
{
   pila_t p = new rep_pila;
   (*p).cantidad = 0;
   
   return p;
}

// Libera la memoria asignada a `p'. 
void liberar_pila(pila_t &p)
{ 
  delete p; 
}

// Devuelve `true' si y solo si `p' no tiene elementos.                                                                       
bool es_vacia_pila(pila_t p)
{ 
  return (*p).cantidad == 0; 
}

// Devuelve `true' si y solo si `p' tiene `CAP_PILA' elementos.
bool es_llena_pila(pila_t p)
{ 
  return (*p).cantidad == CAP_PILA; 
}

// Devuelve el resultado de apilar `nuevo' en `p'.
// Precondición: ! es_llena_pila(p).
pila_t apilar(char nuevo, pila_t p)
{
   assert(!es_llena_pila(p));
   (*p).datos[(*p).cantidad] = nuevo;
   (*p).cantidad++;

   return p;
}

// Devuelve el tope de `p'.
// Precondición: ! es_vacia_pila(p).
char tope(pila_t p) 
{
   assert(!es_vacia_pila(p));
   return (*p).datos[(*p).cantidad - 1];
}

// Devuelve el resultado de desapilar el tope de `p'.
// Precondición: ! es_vacia_pila(p).
pila_t desapilar(pila_t p)
{
   assert(!es_vacia_pila(p));
   (*p).cantidad--;

   return p;
}