#include "cola_avls.cpp"
#include "info.cpp"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>

struct rep_avl
{
  info_t dato;
  nat altura;
  nat cantidad;
  rep_avl* izq, der;
};

typedef rep_avl *avl_t;

struct avl_ultimo
{
  avl_t avl;
  int ultimo;
};

typedef avl_ultimo *avl_ultimo_avl;

static nat max(nat n1,nat n2)
{
  if (n1 > n2)
     return n1;

  return n2;
}

static nat absoluto(int n)
{
  if (n < 0)
    n = n + (2*(-n));

  return n;
}

static void rotarD(avl_t &avl)
{
   nat cantavl = cantidad_en_avl(avl);
   nat cantavlsubarbolderinicial = cantidad_en_avl(avl->der);
   nat cantavlsubarbolderdeavlizqinicial = cantidad_en_avl(avl->izq->der);

   avl_t avlizq = avl->izq;
   avl_t avlsubarbolderdeavlizq=avlizq->der;
   avl->izq = avlsubarbolderdeavlizq;
   avlizq->der = avl;

   avl->altura = max(altura_de_avl(avl->izq),altura_de_avl(avl->der)) + 1;
   avlizq->altura = max(altura_de_avl(avlizq->izq),altura_de_avl(avlizq->der)) + 1;

   avl = avlizq; 

   avl->cantidad = cantavl;  
   avl->der->cantidad = cantavlsubarbolderinicial + cantavlsubarbolderdeavlizqinicial + 1;  
}

static void rotarI(avl_t &avl)
{
  nat cantavl = cantidad_en_avl(avl);
  nat cantavlsubarbolizqinicial = cantidad_en_avl(avl->izq);
  nat cantsubarbolizqdeavlderinicial = cantidad_en_avl(avl->der->izq);

  avl_t avlder = avl->der;
  avl_t avlsubarbolizqdeavlder = avlder->izq;
  avl->der = avlsubarbolizqdeavlder;
  avlder->izq = avl;

  avl->altura = max(altura_de_avl(avl->izq),altura_de_avl(avl->der)) + 1;
  avlder->altura = max(altura_de_avl(avlder->izq),altura_de_avl(avlder->der)) + 1;
 
  avl = avlder;

  avl->cantidad = cantavl;
  avl->izq->cantidad = cantavlsubarbolizqinicial + cantsubarbolizqdeavlderinicial + 1;
}

static void rotarID(avl_t &avl)
{
  rotarI(avl->izq);
  rotarD(avl);
}

static void rotarDI(avl_t &avl)
{
  rotarD(avl->der);
  rotarI(avl);
}

static info_t *en_orden_rec(info_t *res, nat &tope, avl_t avl)
{
  if (!es_vacio_avl(avl))
  {
    res = en_orden_rec(res, tope, izq_avl(avl));
    res[tope] = raiz_avl(avl);
    tope++;
    res = en_orden_rec(res, tope, der_avl(avl));
  }

  return res;
}

static avl_t a2avl_rec(info_t *infos, int inf, int sup)
{
   avl_t res = NULL;

   if (inf <= sup)
   { 
      res = new rep_avl;  
      if (inf == sup)
        res->altura=1;

      nat medio = (inf + sup) / 2;
      res->dato = infos[medio]; 

      res->izq = a2avl_rec(infos, inf, medio - 1);
      res->der = a2avl_rec(infos, medio + 1, sup);
      
      res->cantidad = res->cantidad + 1;
      res->altura = max(altura_de_avl(res->izq),altura_de_avl(res->der)) + 1;
   }

   return res;
}

static avl_ultimo avl_min_rec(nat h, nat primero)
{
    avl_ultimo res;
    if (h == 0)
    {
       res.avl = NULL;
       res.ultimo = primero - 1;
    } 
    else if (h==1)
    {
      
    }
    else
    {
      
    }

    return res;
}

static void imprimir_raiz (avl_t avl)
{
   printf("%d ",numero_info(avl->dato));
}

// Devuelve un avl_t vacío (sin elementos).
// El tiempo de ejecución es O(1).
avl_t crear_avl()
{
   return NULL;
}

// Devuelve `true' si y sólo si `avl' es vacío (no tiene elementos).
// El tiempo de ejecución es O(1).
bool es_vacio_avl(avl_t avl)
{
   return avl == NULL;
}

// Devuelve un arreglo con los elementos de `avl'.
// Los elementos en el arreglo deben estar en orden creciente según los datos numericos.
info_t *en_orden_avl(avl_t avl)
{
  info_t *res = NULL;
  if (!es_vacio_avl(avl))
  {
    res = new info_t[cantidad_en_avl(avl)];
    nat tope = 0;
    res = en_orden_rec(res, tope, avl);
  }

  return res;
}

// Devuelve un avl_t con los `n' elementos que están en el rango [0 .. n - 1] del arreglo `infos'.
// Los elementos están ordenados de manera creciente estricto (creciente y sin repetidos) según los datos numércos.
// El tiempo de ejecución es O(n).
avl_t arreglo_a_avl(info_t *infos, nat n)
{
   return a2avl_rec(infos, 0, n-1);
}

// Devuelve un avl_t de altura `h' con `n' nodos, siendo `n' la mínima cantidad de elementos que puede tener un avl de altura `h'.
// Los datos numéricos de los elementos van desde 1 hasta `n'.
// El tiempo de ejecución es O(n).
avl_t avl_min(nat h)
{
   avl_ultimo res = avl_min_rec(h, 1);
   return res.avl;  
}

// Devuelve el subárbol que tiene como raíz al nodo con el elemento cuyo dato numérico es `clave'.
// El tiempo de ejecución es O(log n), donde `n' es la cantidad de elementos de `avl'.
avl_t buscar_en_avl(int clave, avl_t avl)
{
   avl_t res;
   if (es_vacio_avl(avl))
     res = crear_avl();
   else
   {
     if (numero_info(avl->dato)<clave)
        res = buscar_en_avl(clave,avl->der);
     else if (numero_info(avl->dato)>clave)
        res = buscar_en_avl(clave,avl->izq);
     else res = avl;
   }

   return res;
}

// Inserta `i' en `avl' respetando la propiedad de orden definida.
// El tiempo de ejecución es O(log n), donde `n' es la cantidad de elementos de `avl'.
// Precondición: es_vacio_avl(buscar_en_avl(numero_info(i), avl), numero_info(i) != INT_MAX.
void insertar_en_avl(info_t i, avl_t &avl)
{
   assert(es_vacio_avl(buscar_en_avl(numero_info(i),avl)));
   if (avl == NULL)
   {
     avl = new rep_avl;
     avl->dato = i;
     avl->izq = NULL;
     avl->der = NULL;
     avl->altura = 1;
     avl->cantidad = 1;
   }
   else if(numero_info(i) < numero_info(raiz_avl(avl)))
   {
       insertar_en_avl(i,avl->izq);
       if (absoluto(altura_de_avl(avl->izq)-altura_de_avl(avl->der))>1)
       {
         if (altura_de_avl(avl->izq->izq)>altura_de_avl(avl->izq->der))
            rotarD(avl);
         else // altura_de_avl(avl->izq->izq) < altura_de_avl(avl->izq->der)
            rotarID(avl); 
       }

       avl->altura=max(altura_de_avl(avl->izq),altura_de_avl(avl->der))+1;
       avl->cantidad=avl->cantidad+1;
   }
   else if (numero_info(i) > numero_info(raiz_avl(avl)))
   {
       insertar_en_avl(i,avl->der);
       if (absoluto(altura_de_avl(avl->izq)-altura_de_avl(avl->der))>1)
       {
          if (altura_de_avl(avl->der->der)>altura_de_avl(avl->der->izq))
             rotarI(avl);
          else // altura_de_avl(avl->der->der) < altura_de_avl(avl->der->izq)
             rotarDI(avl);  
       }

       avl->altura = max(altura_de_avl(avl->izq),altura_de_avl(avl->der))+1;
       avl->cantidad = avl->cantidad+1;   
   }
}

// Devuelve la cantidad de elementos en `avl'.
// El tiempo de ejecución es O(1).
nat cantidad_en_avl(avl_t avl)
{
   nat c = 0;
   if (avl != NULL)
      c = avl->cantidad; 

   return c;
}

// Devuelve la altura de `avl'.
// La altura de un árbol vacío es 0.
// El tiempo de ejecución es O(1).
nat altura_de_avl(avl_t avl){
  nat h = 0;
  if (avl != NULL)
     h = avl->altura;

  return h;
}

// Devuelve el elemento asociado a la raíz de `avl'.
// El tiempo de ejecución es O(1).
// Precondición: ! es_vacio_avl(b).
info_t raiz_avl(avl_t avl)
{
   assert(!es_vacio_avl(avl));
   return avl->dato;
}

// Devuelve el subárbol izquierdo de `avl'.
// El tiempo de ejecución es O(1).
// Precondición: ! es_vacio_avl(avl).
avl_t izq_avl(avl_t avl)
{
   assert(!es_vacio_avl(avl));
   return avl->izq;
}

// Devuelve el subárbol derecho de `avl'.
// El tiempo de ejecución es O(1).
// Precondición: ! es_vacio_avl(avl).
avl_t der_avl(avl_t avl)
{
   assert(!es_vacio_avl(avl));
   return avl->der;
}

// Libera la memoria asignada a `avl' y la de sus elementos.
// El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `avl'.
void liberar_avl(avl_t &avl)
{
   if (avl != NULL)
   {
      liberar_avl(avl->izq);
      liberar_avl(avl->der);
      liberar_info(avl->dato);
      delete avl;
      avl = NULL;
   }
}

// Imprime los datos numéricos de los elementos de cada nivel de `avl'.
// Imprime una línea por nivel, primero el más profundo y al final el que corresponde a la raíz.
// Cada nivel se imprime en orden creciente. Se debe dejar un espacio en blanco después de cada número.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de nodos de `avl'.
void imprimir_avl(avl_t avl)
{
   if (!es_vacio_avl(avl))
   {
      cola_avls_t q = crear_cola_avls();
      encolar(avl, q);
      encolar(NULL, q);
      while(!es_vacia_cola_avls(q))
      {
          avl = frente(q);
          desencolar(q);
          if (es_vacio_avl(avl))
          {
            if (!es_vacia_cola_avls(q))
            {
                printf("\n");
                encolar(NULL, q); 
            }
          }
          else
          {
            if (izq_avl(avl) != NULL)
              encolar(avl->izq, q);
            if (der_avl(avl) != NULL)
              encolar(avl->der, q);
          }

          imprimir_raiz(avl);
      }
      
      if (es_vacia_cola_avls(q))
         printf("\n");
      
      liberar_cola_avls(q);    
    }
}

