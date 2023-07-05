#include "cadena.cpp"
#include "info.cpp"

#include <string.h>
#include <stddef.h>
#include <assert.h>

typedef unsigned int nat;

struct rep_hash
{
   cadena_t *datos;
   nat cantidad;
   nat capacidad;
};

typedef rep_hash *hash_t

static nat ubicacionHash(int clave,nat capacidad)
{ 
   int resultado = (clave) % int (capacidad);
   if (resultado < 0)
      resultado = capacidad + resultado;

   return resultado;
}

// Crea un hash_t de asociaciones numero->texto. Podrá haber hasta `tamanio' asociaciones.
hash_t crear_hash(nat tamanio)
{
   hash_t h = new rep_hash;
   h->cantidad = 0;
   h->capacidad = tamanio;
   h->datos = new cadena_t[tamanio];
   for (nat i=0; i< tamanio; i++)
      h->datos[i] = crear_cadena();
  
   return h;
}

// Inserta en `h' la asociación entre `clave' y `valor'.
// El tiempo de ejecución es O(1).
// Precondición: !esta_lleno_hash(t) y !existe_asociacion(clave, h).
void asociar_en_hash(int clave, char *valor, hash_t &h)
{
   assert(!esta_lleno_hash(h));
   assert(!existe_asociacion(clave,h));
 
   cadena_t cad = h->datos[ubicacionHash(clave,h->capacidad)];
   info_t info = crear_info(clave,valor);
   cad = insertar_al_final(info,cad);
   
   h->cantidad ++;
}

// Sustituye en `h' el anterior valor asociado a `clave' por `valor'.
// El tiempo de ejecución es O(1).
// Precondición: existe_asociacion(clave, h).
void actualizar_hash(int clave, char *valor, hash_t &h)
{
   assert(existe_asociacion(clave,h));
   cadena_t cad = h->datos[ubicacionHash(clave, h->capacidad)];
   localizador_t loc = inicio_cadena(cad);
   while (numero_info(info_cadena(loc,cad)) != clave)
   {
      loc = siguiente(loc,cad);
   }

   info_t info = crear_info(clave,valor);
   cad = insertar_antes(info,loc,cad);
   cad = remover_de_cadena(loc,cad);
}

// Elimina de `h' la asociación entre `clave' y algún valor y libera la memoria asignada a mantener esa asociación.
// El tiempo de ejecución es O(1) en promedio.
// Precondición: existe_asociacion(clave, h).
void eliminar_de_hash(int clave, hash_t &h)
{
   assert(existe_asociacion(clave,h));
   cadena_t cad = h->datos[ubicacionHash(clave, h->capacidad)];
   localizador_t loc = inicio_cadena(cad);
   while (numero_info(info_cadena(loc,cad))!=clave)
   {
      loc = siguiente(loc,cad);
   }

   remover_de_cadena(loc,cad);
   h->cantidad --;
}

// Libera la memoria asignada a `h' y todos sus elementos.
void liberar_hash(hash_t &h)
{
   for(nat i = 0; i < h->capacidad; i++)
   {
      liberar_cadena(h->datos[i]);
      h->datos[i] = NULL;
   }

   delete [] h->datos;
   delete h;
   h = NULL;
}

// Devuelve `true' si y sólo si en `h' hay una asociación entre `clave' y algún valor.
// El tiempo de ejecución es O(1) en promedio.
bool existe_asociacion(int clave, hash_t h)
{
   cadena_t cad = h->datos[ubicacionHash(clave,h->capacidad)];
   localizador_t loc = inicio_cadena(cad);
   while (es_localizador(loc) && numero_info(info_cadena(loc,cad)) != clave)
   {
      loc = siguiente(loc,cad);
   }

   return es_localizador(loc) && numero_info(info_cadena(loc,cad))==clave;
}

// Devuelve el valor correspondiente a la asociacion de `clave' en `h'.
// Precondición: existe_asociacion(clave, h).
// El tiempo de ejecución es O(1) en promedio.
char *valor_en_hash(int clave, hash_t h)
{
   assert(existe_asociacion(clave, h));
   cadena_t cad = h->datos[ubicacionHash(clave, h->capacidad)];
   localizador_t loc = inicio_cadena(cad);

   while (numero_info(info_cadena(loc, cad)) != clave)
   {
      loc = siguiente(loc, cad);
   }

   return frase_info(info_cadena(loc, cad));
}

// Devuelve `true' si y sólo si `h' tiene `tamanio' elementos.
// El tiempo de ejecución es O(1).
bool esta_lleno_hash(hash_t h)
{
   return h->cantidad >= h->capacidad;
}