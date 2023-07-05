#include "info.cpp"
#include "cadena.cpp"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct rep_binario
{
  info_t dato;
  int altura;
  rep_binario *izq;
  rep_binario *der;
};

typedef struct rep_binario *binario_t;

static void imprimir_raiz (binario_t b)
{
   printf("(%d,", numero_info(b->dato));
   printf("%s)\n", frase_info(b->dato));
}

static int maximo(int n1, int n2)
{ 
   return (n1 >= n2) ? n1: n2;
}

static int maxima_altura(binario_t b1, binario_t b2)
{
    int altura = 0;
    if (es_vacio_binario(b1) && !es_vacio_binario(b2))
      altura = b2->altura;
    else if (!es_vacio_binario(b1) && es_vacio_binario(b2))
      altura = b1->altura;
    else if (!es_vacio_binario(b1) && !es_vacio_binario(b2))
      altura = maximo(b1->altura, b2->altura);
    
    return altura;
}

static void imprimir_binario_con_guion (binario_t b, int numero)
{
   if (!es_vacio_binario(b->der))
      imprimir_binario_con_guion(b->der, numero + 1);
   
   for(int i = 0; i < numero; i++)
   {
      printf("-");
   }   

   imprimir_raiz(b);   

   if (!es_vacio_binario(b->izq))
      imprimir_binario_con_guion(b->izq, numero + 1);   
}

static binario_t copiabinario(binario_t b)
{
  if (es_vacio_binario(b))
     return NULL;
 
  binario_t arbolRes = new rep_binario;
  arbolRes->dato = copia_info(raiz(b));
  arbolRes->altura = b->altura;
  arbolRes->izq = copiabinario(izquierdo(b));
  arbolRes->der = copiabinario(derecho(b));
  
  return arbolRes;
}

static void borrar_mayores_clave(int clave, binario_t &b)
{
  if (!es_vacio_binario(b))
  {
    if (numero_info(raiz(b))>=clave)
    {
      liberar_info(b->dato);
      if (es_vacio_binario(izquierdo(b)) && es_vacio_binario(derecho(b)))
      {
         delete b;
         b = NULL;
      }
      else
      {
        binario_t aux = b;
        if (es_vacio_binario(izquierdo(b)))
        {
           b = derecho(b);
           delete aux;
        }
        else
        {
          if (es_vacio_binario(derecho(b)))
          {
            b = izquierdo(b);
            delete aux;
          }
          else
          {
            b->dato = mayor(izquierdo(b));
            b->izq = remover_mayor(izquierdo(b));
          }
        }
      }
      borrar_mayores_clave(clave,b);
    }
    else
    {
       borrar_mayores_clave(clave, b->izq);
       borrar_mayores_clave(clave, b->der);
       b->altura = maxima_altura(b->izq, b->der) + 1;
    }
  }
}

static void nivel_en_binario_Aux(nat l,binario_t b, cadena_t &c)
{
   if (!es_vacio_binario(b) && l>=1)
   {
      nivel_en_binario_Aux(l-1,izquierdo(b),c);
      if (l == 1)
         c = insertar_al_final(copia_info(b->dato),c);
      if (l > 1)
         nivel_en_binario_Aux(l-1,derecho(b),c);
   }
}

static void linealizacion_aux(binario_t b,cadena_t &c)
{
  if (!es_vacio_binario(b))
  {
     linealizacion_aux(izquierdo(b),c);
     c = insertar_al_final(copia_info(b->dato), c);
     linealizacion_aux(derecho(b),c);
  }
}

static bool es_hoja (binario_t b)
{
  if (!es_vacio_binario(b))
     return (b->izq == NULL && b->der == NULL);
  
  return false;
}

static int absoluto(int n)
{ 
  return (n >= 0 ) ? (n) : (-n); 
}

static void suma_ultimos_pares_aux (nat i, nat &indiceActual, binario_t b, nat &suma) {
  if (!es_vacio_binario(b)) { 
    suma_ultimos_pares_aux(i, indiceActual, b->der, suma);
    if( indiceActual < i && numero_info(b->dato) % 2 == 0) {
      suma = suma + numero_info(b->dato);
      indiceActual++;
    } 
    if (indiceActual <i) {
      suma_ultimos_pares_aux(i, indiceActual, b->izq, suma); 
    }
  }
}

static int diferencias_altura(binario_t b1, binario_t b2)
{
  int diferencia = 0;
  if (es_vacio_binario(b1) && !es_vacio_binario(b2))
    diferencia = b2->altura;
  else if (!es_vacio_binario(b1) && es_vacio_binario(b2))
    diferencia = b1->altura;
  else if (!es_vacio_binario(b1) && !es_vacio_binario(b2))
    diferencia = absoluto(b1->altura - b2->altura);

  return diferencia;
}

static void cadena_a_binario_aux(cadena_t cad, localizador_t *arregloCadena, int longitudCadena, binario_t &arbol)
{
  if (longitudCadena == 0)
     return;
  else if(longitudCadena == 1)
     arbol = insertar_en_binario(copia_info(info_cadena(arregloCadena[longitudCadena-1], cad)), arbol);
  else
  {
    //si es par
    if(longitudCadena % 2 == 0)
    {
      //var
      int largoIzq = longitudCadena/2;
      int largoDer = (longitudCadena/2) - 1;
      localizador_t *izq = new localizador_t[largoIzq];
      localizador_t *der = new localizador_t[largoDer];

      //inserto el dato
      arbol = insertar_en_binario(copia_info(info_cadena(arregloCadena[longitudCadena/2], cad)), arbol);
    
      //lleno izq y der
      for(int i=0; i<longitudCadena/2 ;i++)
      {
         izq[i] = arregloCadena[i];
      }
      
      for(int i=longitudCadena/2 + 1; i<longitudCadena; i++)
      {
        der[i - (longitudCadena/2 + 1)] = arregloCadena[i];
      }
      
      cadena_a_binario_aux(cad, izq, largoIzq, arbol);
      cadena_a_binario_aux(cad, der, largoDer, arbol);

      delete []izq;
      delete []der;
    }
    //es impar
    else
    {
      int largoIzq = longitudCadena/2;
      int largoDer = longitudCadena/2;
    
      localizador_t *izq = new localizador_t[largoIzq];
      localizador_t *der = new localizador_t[largoDer];
      
      //inserto el dato
      arbol = insertar_en_binario(copia_info(info_cadena(arregloCadena[longitudCadena/2], cad)), arbol);
      
      //Lleno izq y der
      for(int i=0; i<longitudCadena/2 ;i++)
      {
        izq[i] = arregloCadena[i];
      }
      
      for(int i=longitudCadena/2 + 1; i<longitudCadena; i++)
      {
        der[i - (longitudCadena/2 + 1)] = arregloCadena[i];
      }

      cadena_a_binario_aux(cad, izq, largoIzq, arbol);
      cadena_a_binario_aux(cad, der, largoDer, arbol);

      delete []izq;
      delete []der;
    }
  }
}

// Devuelve un binario_t vacío (sin elementos). El tiempo de ejecución es O(1).
binario_t crear_binario()
{ 
  return NULL; 
}

// Inserta `i' en `b' respetando la propiedad de orden definida. Devuelve `b'.
// El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de elementos de `b'.
// Precondición: es_vacio_binario(buscar_subarbol(frase_info(i), b).
binario_t insertar_en_binario(info_t i, binario_t b)
{ 
  if (es_vacio_binario(b))
  {
     b = new rep_binario;
     b->izq = b->der = NULL;
     b->dato = i;
     b->altura = 1;
  }
  else
  {
    if (strcmp(frase_info(raiz(b)),frase_info(i)) > 0)
    {
      b->izq = insertar_en_binario(i,izquierdo(b));
      b->altura = maxima_altura(b->izq, b->der) + 1;
    }
    else
    {
      b->der = insertar_en_binario(i,derecho(b));
      b->altura = maxima_altura(b->izq, b->der) + 1;
    }    
  }

  return b; 
} 

// Devuelve el elemento mayor (según la propiedad de orden definida) de `b'.
// El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de elementos de `b'.
// Precondición: ! es_vacio_binario(b).
info_t mayor(binario_t b)
{	
  while(!es_vacio_binario(derecho(b)))
     b = derecho(b);
    
  return raiz(b);	
}

// Remueve el nodo en el que se localiza el elemento mayor de `b' (libera la memoria  asignada al nodo, pero no la del elemento).
// Devuelve `b'.
// El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de elementos de `b'.
// Precondición: ! es_vacio_binario(b).
binario_t remover_mayor(binario_t b)
{
   assert(!es_vacio_binario(b));
   if (b->der == NULL)
   {
      binario_t izq = b->izq;
      delete (b);
      b = izq;
   }
   else
   {
	    b->der = remover_mayor(b->der);
      b->altura = maxima_altura(b->izq, b->der) + 1;
   }	
   
   return b;
}

// Remueve de `b' el nodo en el que el dato de texto de su elemento es `t'.
// Si los dos subárboles del nodo a remover son no vacíos, en sustitución del elemento removido debe quedar el que es el mayor 
// (segun la propiedad de orden definida) en el subárbol izquierdo. Devuelve `b'.
// Libera la memoria del nodo y del elemento.
// El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de elementos de `b'.
// Precondición: !es_vacio_binario(buscar_subarbol(frase_info(i), b).
binario_t remover_de_binario(const char *t, binario_t b)
{
  if (strcmp(frase_info(raiz(b)),t) > 0)
  {
    b->izq = remover_de_binario(t,izquierdo(b));
    b->altura = maxima_altura(b->izq, b->der) + 1;
  }
  else if (strcmp(frase_info(raiz(b)),t) < 0)
  {
    b->der = remover_de_binario(t,derecho(b));
    b->altura = maxima_altura(b->izq, b->der) + 1;  
  }
  else
  {
    liberar_info(b->dato);
    if (es_vacio_binario(izquierdo(b)) && es_vacio_binario(derecho(b)) )
    {
       delete b;
       b = NULL;
    }
    else
    {
      binario_t aux = b;
      if (es_vacio_binario(izquierdo(b)))
      {
         b = derecho(b);
         delete aux;
      }
      else
      {
        if (es_vacio_binario(derecho(b)))
        {
           b = izquierdo(b);
           delete aux;
        }
        else
        {
           b->dato = mayor(izquierdo(b));
           b->izq = remover_mayor(izquierdo(b));
        }   
      }
    }
  }

  return b;
}

// Libera la memoria asociada a `b' y todos sus elementos. Devuelve `b'.
// El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
binario_t liberar_binario(binario_t b)
{
  if (b!= NULL)
  {
    b->izq = liberar_binario(b->izq);
    b->der = liberar_binario(b->der);
    liberar_info(b->dato);
    delete b;
    b = NULL;
  }
  
  return b;
}  

// Devuelve `true' si y sólo si `b' es vacío (no tiene elementos). El tiempo de ejecución es O(1).
bool es_vacio_binario(binario_t b)
{
  return b == NULL; 
}

// Devuelve `true' si y sólo si cada nodo de `b' cumple la condición de balanceo AVL. El árbol vacío cumple la condición.
// Cada nodo se puede visitar una sola vez.
// El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
bool es_AVL(binario_t b)
{ 
  bool ret = true;
  if (!es_vacio_binario(b))
  {
    bool retIzq = es_AVL(b->izq);
    bool retDer = es_AVL(b->der);
    if (diferencias_altura(b->izq, b->der) > 1)
    {
      ret = false;
    }

    ret = ret && retIzq && retDer;
  }

  return ret;
}

// Devuelve el elemento asociado a la raíz de `b'.
// El tiempo de ejecución es O(1).
// Precondición: ! es_vacio_binario(b).
info_t raiz(binario_t b)
{
  return b->dato;
}

// Devuelve el subárbol izquierdo de `b'.
// El tiempo de ejecución es O(1).
// Precondición: ! es_vacio_binario(b).
binario_t izquierdo(binario_t b)
{
  return b->izq;	
}

// Devuelve el subárbol derecho de `b'.
// Precondición: ! es_vacio_binario(b).
// El tiempo de ejecución es O(1).
binario_t derecho(binario_t b)
{
  return b->der;
}

// Devuelve el subárbol que tiene como raíz al nodo con el elemento cuyo dato de texto es `t'.
// Si `t' no pertenece a `b', devuelve el árbol vacío.
// El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de elementos de `b'.
binario_t buscar_subarbol(const char *t, binario_t b)
{
   binario_t res;
   if (es_vacio_binario(b))
     res = crear_binario();
   else
   {
	   int comp = strcmp(t,frase_info(raiz(b)));
	   if (comp < 0)
	      res = buscar_subarbol(t,izquierdo(b));
	   else if (comp > 0)
	      res = buscar_subarbol(t,derecho(b));
	   else
	     res = b;
   }
  
   return res;
}

// Devuelve la altura de `b'.
// La altura de un árbol vacío es 0.
// El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
nat altura_binario(binario_t b)
{
  if (b==NULL) 
    return 0;
  else 
    return 1 + maximo(altura_binario(b->izq), altura_binario(b->der));
}

// Devuelve la cantidad de elementos de `b'.
// El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
nat cantidad_binario(binario_t b)  
{
  if (b==NULL) 
    return 0;
  else 
    return cantidad_binario(izquierdo(b)) + cantidad_binario(derecho(b)) + 1;
}


// Devuelve la suma de los datos numéricos de los últimos `i' elementos (considerados según la propiedad de orden de los árboles binario_t)
// de `b' cuyos datos numéericos sean pares. Si en `b' hay menos de `i' elementos con dato numérico par devuelve la suma de todos los datos numéricos pares de `b'.
// No visita nuevos nodos después que se hayan encontrado los `i' elementos.
// El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
int suma_ultimos_pares(nat i, binario_t b)
{ 
    nat suma = 0;
    nat posicionActual = 0;
    if (!es_vacio_binario(b) && i!=0)
       suma_ultimos_pares_aux(i, posicionActual, b, suma);
    
    return suma;
}

// Devuelve una cadena_t con los elementos de `b' en orden lexicográfico
// creciente según sus datos de texto.
// La cadena_t devuelta no comparte memoria con `b'.
// El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
cadena_t linealizacion(binario_t b)
{
   cadena_t c = crear_cadena();
   if (!es_vacio_binario(b))
      linealizacion_aux(b,c);
  
   return c;
}

// Devuelve un árbol balanceado cuyos elementos son los que están contenidos en `cad'.
// El árbol devuelto no comparte memoria con `cad'.
// El tiempo de ejecución es O(n . log n), siendo `n` la cantidad de elementos de `cad'.
// Precondición: los elementos de `cad' están en orden lexicográfico creciente estricto según sus datos de texto.
binario_t cadena_a_binario(cadena_t cad)
{
  binario_t arbol = NULL;

  int longitudCadena = (int)longitud(cad);
  localizador_t *arregloLocalizadores = new localizador_t[longitudCadena];
  localizador_t iterador = inicio_cadena(cad);

  // lleno arreglo de localizadores
  int i = 0;
  while(es_localizador(iterador))
  {
     arregloLocalizadores[i] = iterador;
     iterador = siguiente(iterador, cad);
     i++;
  }

  // algoritmo principal
  cadena_a_binario_aux(cad, arregloLocalizadores, longitudCadena, arbol);
  delete []arregloLocalizadores;
 
  return arbol;
}

// Devuelve un árbol con copias de los elementos de `b' que cumplen la condición "numero_info(elemento) < clave".
// El árbol resultado no comparte memoria con `b'.
// El tiempo de ejecución es O(n), donde `n' es la cantidad de elementos de `b'.
binario_t menores(int clave, binario_t b)
{
   binario_t arbol = crear_binario();
   if (!es_vacio_binario(b))
   {
      arbol = copiabinario(b);
      borrar_mayores_clave(clave,arbol);
   }
  	
   return arbol;	
}

// Devuelve `true' si y solo si en `b' hay un camino desde la raiz hasta una hoja que sea compatible con `c'.
// Decimos que una cadena y un camino son compatibles si tienen la misma longitud y al recorrer la cadena desde el inicio hasta el final y el camino
// desde la raíz hasta la hoja las frases de los respectivos elementos son iguales.
// El tiempo de ejecución es O(log n) en promedio, donde `n' es la cantidad de elementos de `b'.
bool es_camino(cadena_t c, binario_t b)
{
  if (es_vacio_binario(b) && es_vacia_cadena(c)) 
	   return true;
  else if (es_vacio_binario(b) || es_vacia_cadena(c))
     return false;
  else
	{   
		localizador_t iterador = inicio_cadena(c);
	  binario_t aux = b;
	  
	  while(es_localizador(siguiente(iterador,c)) && !es_hoja(aux) && strcmp( frase_info(info_cadena(iterador,c)), frase_info(raiz(aux)) ) == 0 )   
	  {
	    iterador = siguiente(iterador,c);	  
		  if (strcmp(frase_info(info_cadena(iterador,c)), frase_info(raiz(aux))) > 0)
		     aux = derecho(aux);
		  else
		  {
		    if (strcmp( frase_info(info_cadena(iterador,c)), frase_info(raiz(aux))) < 0)
		      aux = izquierdo(aux);
        else return false;
		  }
	  }

    if(!es_localizador(siguiente(iterador,c)) && es_hoja(aux) && strcmp( frase_info(info_cadena(iterador,c)), frase_info(raiz(aux)) ) == 0)
	     return true;
	  else return false;
  } 	  	
}

// Devuelve una cadena_t con los elementos del nivel `l' de 'b'. La raíz es el nivel 1.
// La cadena_t resultado debe estar ordenada de manera creciente según las frases de sus elementos.
// La cadena_t devuelta no comparte memoria con `b'.
cadena_t nivel_en_binario(nat l, binario_t b)
{
   cadena_t c = crear_cadena();
   nivel_en_binario_Aux(l,b,c);

	 return c;
}

// Imprime los elementos de `b', uno por línea, en orden descendente de la propiedad de orden de los árboles `binario_t'.
// Antes del elemento imprime una cantidad de guiones igual a su nivel.
// El elemento se imprime según la especificación dada en `info_a_texto'.
// El nivel de la raíz es 0. 
// Antes de terminar, se debe imprimir un fin de linea. Si es_vacio_binario(b) sólo se imprime el fin de línea.
// El tiempo de ejecución es O(n . log n) en promedio, donde `n' es la cantidad de elementos de `b'.
void imprimir_binario(binario_t b)
{
  if (!es_vacio_binario(b))
  {
    printf("\n");
    imprimir_binario_con_guion(b,0);
  }
  else printf("\n");
}

