#include "avl.cpp"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>

struct nodocola
{
  avl_t avl;
  nodocola *sig;
};

typedef struct nodocola *avl_cola_t;

struct rep_cola_avls
{
  nodocola *ppio;
  nodocola *final;
};

typedef struct rep_cola_avls *cola_avls_t;

// Devuelve una cola_avls_t vacía (sin elementos).
cola_avls_t crear_cola_avls()
{
  cola_avls_t colaavl = new rep_cola_avls;
  colaavl->ppio = colaavl->final=NULL;
  
  return colaavl;
}

// Devuelve `true' si y sólo si `c' es vacía (no tiene elementos).
bool es_vacia_cola_avls(cola_avls_t c)
{
  return (c->ppio==NULL && c->final==NULL);
}

// Devuelve el elemento que está en el frente de `c'.
// Precondición: ! es_vacia_cola_binarios(c);
avl_t frente(cola_avls_t c)
{
  assert(!es_vacia_cola_avls(c));
  return c->ppio->avl;
}

// Libera la memoria asignada a `c', pero NO la de sus elementos.
void liberar_cola_avls(cola_avls_t &c)
{
  delete c;
  c = NULL;  
}

// Encola `avl' en `c'.
void encolar(avl_t b, cola_avls_t &c)
{
  avl_cola_t nuevonodo = new nodocola; 
  nuevonodo->avl=b;
  nuevonodo->sig=NULL;
  if (es_vacia_cola_avls(c))
    c->ppio = c->final = nuevonodo;
  else
  {
    c->final->sig = nuevonodo;
    c->final = nuevonodo;
  }
}

// Remueve de `c' el elemento que está en el frente.
void desencolar(cola_avls_t &c)
{
  if (!es_vacia_cola_avls(c))
  {
    if (c->ppio == c->final)
    {
      delete c->ppio;
      c->ppio = c->final = NULL;
    }
    else
    {
		   avl_cola_t aux = c->ppio->sig;
		   delete c->ppio;
		   c->ppio = aux;
    }
  }
}