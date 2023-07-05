#include "avl.cpp"
#include "iterator.cpp"
#include "info.cpp"
#include "cadena.cpp"

#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct rep_conjunto
{
  avl_t arbol;
};

typedef rep_conjunto *conjunto_t;

// Devuelve un conjunto_t vacío (sin elementos).
// El tiempo de ejecución es O(1).
conjunto_t crear_conjunto()
{
   conjunto_t conj = new rep_conjunto;
   conj->arbol = NULL;

   return conj;
};

// Devuelve un conjunto_t cuyo único elemento es `i'.
// El tiempo de ejecución es O(1).
conjunto_t singleton(info_t i)
{
   conjunto_t conjunto_res = crear_conjunto();
   insertar_en_avl(i, conjunto_res->arbol);

   return conjunto_res;
};

// Devuelve un conjunto_t con los elementos que pertenecen a  `c1' o `c2'.
// Si en ambos conjuntos hay un elemento con el mismo dato  numérico en el conjunto_t devuelto se debe incluir el que pertenece a `c1'.
// El tiempo de ejecucion es O(n1 + n2 + n.log n), siendo `n1' y `n2' la cantidad de elementos de `c1' y `c2' respectivamente y `n' la del conjunto_tresultado.
// El conjunto_t devuelto no comparte memoria ni con `c1' no con `c2'.
conjunto_t union_conjunto(conjunto_t c1, conjunto_t c2)
{
   iterador_t iter_c1 = iterador_conjunto(c1);
   reiniciar_iterador(iter_c1);
   iterador_t iter_c2 = iterador_conjunto(c2);
   reiniciar_iterador(iter_c2);
   iterador_t iter_union = crear_iterador();

   // Lleno iter_union
   while (esta_definida_actual(iter_c1) || esta_definida_actual(iter_c2))
   {
       if (esta_definida_actual(iter_c1) && esta_definida_actual(iter_c2))
        {
          if (numero_info(actual_en_iterador(iter_c1)) == numero_info(actual_en_iterador(iter_c2)))
            agregar_a_iterador(actual_en_iterador(iter_c1), iter_union);
          else if (numero_info(actual_en_iterador(iter_c1)) < numero_info(actual_en_iterador(iter_c2)))
          {
            agregar_a_iterador(actual_en_iterador(iter_c1), iter_union);
            agregar_a_iterador(actual_en_iterador(iter_c2), iter_union);
          }
          else
          {
            agregar_a_iterador(actual_en_iterador(iter_c2), iter_union);
            agregar_a_iterador(actual_en_iterador(iter_c1), iter_union);
          }
          avanzar_iterador(iter_c1);
          avanzar_iterador(iter_c2);
        }
        else if (esta_definida_actual(iter_c1))
        {
          while (esta_definida_actual(iter_c1))
          {
              agregar_a_iterador(actual_en_iterador(iter_c1), iter_union);
              avanzar_iterador(iter_c1);    
          }   
        }
        else
        {
          while (esta_definida_actual(iter_c2))
          {
              agregar_a_iterador(actual_en_iterador(iter_c2), iter_union);
              avanzar_iterador(iter_c2);     
          }       
        } 
   }

   bloquear_iterador(iter_union);
   //Genero arreglo de infos
   nat longitud_iter_union = 0;
   reiniciar_iterador(iter_union);

   while (esta_definida_actual(iter_union))
   {
      longitud_iter_union++;
      avanzar_iterador(iter_union);
   }
  
   info_t *infos = new info_t[longitud_iter_union];
   reiniciar_iterador(iter_union);

   for(nat i = 0; i < longitud_iter_union; i++)
   {
      infos[i] = copia_info(actual_en_iterador(iter_union));
      avanzar_iterador(iter_union);
   }

   //Paso final: Creo el conjunto y llamo arreglo_a_avl
   conjunto_t conj_res = crear_conjunto(); 
   conj_res->arbol = arreglo_a_avl(infos, longitud_iter_union);

   //Elimino todas las estructuras que genere
   liberar_iterador(iter_c1);
   liberar_iterador(iter_c2);
   liberar_iterador(iter_union);
  
   for(nat i = 0; i < longitud_iter_union; i++)
   {
      liberar_info(infos[i]);
   }

   delete[] infos;

   return conj_res;
}

// Devuelve un conjunto_t con los elementos de `c1' cuyos datos numéricos no pertenecen a `c2'.
// El tiempo de ejecucion es O(n1 + n2 + n.log n), siendo `n1' y `n2' la cantidad de elementos de `c1' y `c2' respectivamente y `n' la del conjunto_tresultado.
// El conjunto_t devuelto no comparte memoria ni con `c1' no con `c2'.
conjunto_t diferencia(conjunto_t c1, conjunto_t c2)
{
   iterador_t iter_c1 = iterador_conjunto(c1);
   reiniciar_iterador(iter_c1);
   iterador_t iter_c2 = iterador_conjunto(c2);
   reiniciar_iterador(iter_c2);
   iterador_t iter_diferencia = crear_iterador();

   //Lleno iter_diferencia
   while (esta_definida_actual(iter_c1) || esta_definida_actual(iter_c2))
   {
      if(esta_definida_actual(iter_c1) && esta_definida_actual(iter_c2))
      {
          if (numero_info(actual_en_iterador(iter_c2)) < numero_info(actual_en_iterador(iter_c1)))
              avanzar_iterador(iter_c2);
          else if (numero_info(actual_en_iterador(iter_c2)) > numero_info(actual_en_iterador(iter_c1)))
          {
            agregar_a_iterador(actual_en_iterador(iter_c1), iter_diferencia);
            avanzar_iterador(iter_c1);
          }
          else
          {
            avanzar_iterador(iter_c1);
            avanzar_iterador(iter_c2);
          }
      }
      else if (!esta_definida_actual(iter_c2) && esta_definida_actual(iter_c1))
      {
         while (esta_definida_actual(iter_c1))
         {
            agregar_a_iterador(actual_en_iterador(iter_c1), iter_diferencia);
            avanzar_iterador(iter_c1);  
         }     
      }
   }

   bloquear_iterador(iter_diferencia);
   //Genero arreglo de infos
   nat longitud_iter_diferencia = 0;
   reiniciar_iterador(iter_diferencia);

   while (esta_definida_actual(iter_diferencia))
   {
      longitud_iter_diferencia++;
      avanzar_iterador(iter_diferencia);
   }
  
   info_t *infos = new info_t[longitud_iter_diferencia];
   reiniciar_iterador(iter_diferencia);

   for(nat i = 0; i < longitud_iter_diferencia; i++)
   {
      infos[i] = copia_info(actual_en_iterador(iter_diferencia));
      avanzar_iterador(iter_diferencia);
   }

   //Paso final: Creo el conjunto y llamo arreglo_a_avl
   conjunto_t conj_res = crear_conjunto(); 
   conj_res->arbol = arreglo_a_avl(infos, longitud_iter_diferencia);

   //Elimino todas las estructuras que genere
   liberar_iterador(iter_c1);
   liberar_iterador(iter_c2);
   liberar_iterador(iter_diferencia);
  
   for(nat i = 0; i < longitud_iter_diferencia; i++)
   {
      liberar_info(infos[i]);
   }

   delete[] infos;

   return conj_res;
}

// Devuelve `true' si y sólo `c' es vacío (no tiene elementos).
// El tiempo de ejecución es O(1).
bool es_vacio_conjunto(conjunto_t c)
{
   return c->arbol==NULL;
};

// Devuelve un iterador_t de los elementos de `c'.
// En la recorrida del iterador devuelto los datos numéricos aparecerán en orden creciente.
// El tiempo de ejecución es O(n), siendo `n' es la cantidad de elementos de `c'.
iterador_t iterador_conjunto(conjunto_t c)
{
  iterador_t res = crear_iterador();
  info_t *infos = en_orden_avl(c->arbol);
  for (nat i = 0; i < cantidad_en_avl(c->arbol); i++)
     agregar_a_iterador(infos[i],res);
  
  bloquear_iterador(res);
  delete[] infos;
  
  return res; 
};

// Libera la memoria asignada a `c' y la de todos sus elementos.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos de `c'.
void liberar_conjunto(conjunto_t &c)
{
  liberar_avl(c->arbol);
  delete c;

  c = NULL;
};

// Devuelve un conjunto_t con los `n' elementos que están en en el rango [0 .. n - 1] del arreglo `infos'.
// Los elementos están ordenados de manera creciente estricto (creciente y sin repetidos) según los datos numércos.
// El tiempo de ejecución es O(n).
conjunto_t arreglo_a_conjunto(info_t *infos, nat n)
{
  conjunto_t conjunto_res = crear_conjunto();
  conjunto_res->arbol = arreglo_a_avl(infos, n);

  return conjunto_res;
};

// Devuelve `true' si y sólo si `info' es un elemento de `c'.
// El tiempo de ejecución es O(log n), siendo `n' es la cantidad de elementos de `c'.
bool pertenece_conjunto(info_t info, conjunto_t s)
{
  avl_t avl = buscar_en_avl(numero_info(info),s->arbol);
  if (es_vacio_avl(avl))
     return false;

  return !strcmp(frase_info(raiz_avl(avl)), frase_info(raiz_avl(avl))) != 0;
}