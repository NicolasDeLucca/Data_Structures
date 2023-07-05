#include "info.cpp"

#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

typedef unsigned int nat;

struct rep_heap
{
  info_t *infos;
  nat tope;
  nat capacidad_maxima;
  nat rango_num;
  nat *vector;
};

typedef rep_heap *heap_t;

static void filtrado_ascendente_rec(nat *vector, info_t *arreglo, nat pos)
{
  if ((pos>1) && (numero_info(arreglo[pos/2]) > numero_info(arreglo[pos])))
  {
     info_t swap = arreglo[pos];
     arreglo[pos] = arreglo[pos/2];
     arreglo[pos/2] = swap;
     filtrado_ascendente_rec(vector, arreglo,pos/2);
  }
  else if (pos==1)
  {
    if (arreglo[2]!=NULL)
    {
      if (numero_info(arreglo[1]) > numero_info(arreglo[2]))
      {
        info_t swap = arreglo[1];
        arreglo[1] = arreglo[2];
        arreglo[2] = swap;
      }
    }
  }
  vector[numero_info(arreglo[pos])] = pos;
}
 
static void filtrado_descendente(nat *vector, info_t *arreglo, nat tope, nat pos)
{
   info_t swapHeap = arreglo[pos];
   while (2*pos <= tope)
   { 
      nat hijo = 2*pos;
      if((hijo+1 <= tope) && numero_info(arreglo[hijo]) > numero_info(arreglo[hijo+1]))
         hijo = hijo + 1;
      if (numero_info(swapHeap) > numero_info(arreglo[hijo]))
      {
         vector[numero_info(arreglo[hijo])] = pos;
         arreglo[pos] = arreglo[hijo];
         pos = hijo;   
      }
      else break;  
   }

   vector[numero_info(swapHeap)] = pos;
   arreglo[pos] = swapHeap;
}

// Devuelve un `heap' que puede contener hasta `tamanio' elementos.
// Los datos numéricos pueden estar en el rango 0 .. max_valor.
heap_t crear_heap(nat tamanio, nat max_valor)
{
  heap_t heapRes = new rep_heap;
  heapRes->infos= new info_t[tamanio+1];
  heapRes->vector = new nat[max_valor+1];

  // Inicializo heap
  for(nat i = 1; i <= tamanio; i++)
  {
    heapRes->infos[i] = NULL;
  }

  // Inicializo vector
  for(nat i = 0; i <= max_valor; i++)
  {
    heapRes->vector[i] = 0;
  }
  
  heapRes->tope = 0;
  heapRes->capacidad_maxima = tamanio;
  heapRes->rango_num = max_valor;
  return heapRes;
}

// si inserta el elemento `i'.
// El tiempo de ejecución es O(log tamanio).
// Precondición: !es_lleno_heap(h) y !hay_valor(v,h) y v <= max_valor(h), donde v = numero_info(i).
void insertar_en_heap(info_t i, heap_t &h)
{
  assert(!es_lleno_heap(h));
  assert(!hay_valor(numero_info(i),h));
  assert(numero_info(i) <= max_valor(h));
  
  h->tope ++;
  h->infos[h->tope] = i;  
  filtrado_ascendente_rec(h->vector,h->infos,h->tope);
}

// Reduce a v/2 el valor del elemento cuyo valor actual es `v'.
// El tiempo de ejecución es O(log tamanio).
// Precondición: hay_valor(v, h) y !hay_valor(v/2, h).
void reducir(nat v, heap_t &h)
{
  assert(hay_valor(v,h));
  assert(!hay_valor(v/2,h));
  int posicionEnHeap = h->vector[(int)v];
  h->vector[(int)v] = 0;
  
  nat numero = numero_info(h->infos[posicionEnHeap])/2;
  int largoFrase = strlen(frase_info(h->infos[posicionEnHeap]));
  char *frase = new char[largoFrase + 1];
  strcpy(frase, frase_info(h->infos[posicionEnHeap]));
  liberar_info(h->infos[posicionEnHeap]);
  h->infos[posicionEnHeap] = crear_info(numero, frase);
  
  filtrado_ascendente_rec(h->vector, h->infos, posicionEnHeap);
}

// Elimina de 'h' el elemento de menor valor y libera la memoria que tiene asignada.
// El tiempo de ejecución es O(log tamanio).
// Precondición: ! es_vacia_heap(h).
void eliminar_menor(heap_t &h)
{
   assert(!es_vacio_heap(h));
   h->vector[numero_info(h->infos[1])] = 0;
   info_t Info = h->infos[1];
   h->infos[1]=h->infos[h->tope];
   h->vector[numero_info(h->infos[h->tope])] = 0;
   h->tope --;

   if (!es_vacio_heap(h)) 
      filtrado_descendente(h->vector,h->infos,h->tope,1);

   liberar_info(Info);
   h->infos[h->tope+1] = NULL;  
}

// Libera la memoria asignada a `h' y a sus elementos. 
void liberar_heap(heap_t &h)
{
  if (!es_vacio_heap(h))
  {
    for(nat i = 1; i <= h->tope; i++)
    {
       liberar_info(h->infos[i]);
    }
  }
  
  delete[] h->vector;
  delete[] h->infos; 
  delete h;
  h = NULL;
}

// Devuelve `true' si y sólo si en `h' no hay elementos.
// El tiempo de ejecución es O(1).
bool es_vacio_heap(heap_t h)
{
   return h->tope == 0;
}

// Devuelve `true' si y sólo si en `h' hay `tamanio' elementos.
// El tiempo de ejecución es O(1).
bool es_lleno_heap(heap_t h)
{
   return h->tope == h->capacidad_maxima;  
}

// Devuelve `true' si y sólo si en 'h' hay un elemento con valor `v'.
// El tiempo de ejecución es O(1).
bool hay_valor(nat v, heap_t h)
{
   return h->vector[v]!=0;
}

// Devuelve el elemento de 'h' con menor valor.
// El tiempo de ejecución es O(1).
// Precondición: ! es_vacio_h(h).
info_t menor(heap_t h)
{
   assert(!es_vacio_heap(h));
   return h->infos[1]; 
}

// Devuelve el máximo valor del rango para los datos numericos, establecido en crear_h.
nat max_valor(heap_t h)
{
   return h->rango_num;
}