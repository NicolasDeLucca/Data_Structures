#include <assert.h>
#define CAP_MAP 30

struct asociacion_t
{
   char clave;
   char valor;
};

// Declaración del tipo `mapping_t. Es de tipo puntero a `rep_map'.
// struct rep_map es la representación de `mapping_t'
struct rep_map
{
   // el rango del arreglo es desde 0 hasta CAPACIDAD - 1
   asociacion_t datos[CAP_MAP];
   // cantidad indica la cantidad de elementos = posición donde se insertaria el nuevo
   int cantidad;
};

typedef struct rep_map *mapping_t;

// Devuelve el indice en (*m).datos donde está clave, o -1 si no está.
static int buscar(char clave, mapping_t m)
{
   int i = (*m).cantidad - 1;
   while (i >= 0)
   {
      if ((*m).datos[i].clave == clave) break;
      else i--;
   }

   return i;
}

// Devuelve un elemento de tipo `mapping_t' vacío (sin elementos) que puede mantener hasta `CAP_MAP' asociaciones.
mapping_t crear_map()
{
    mapping_t m = new rep_map;
    (*m).cantidad = 0;

    return m;
}

// Libera la memoria asignada a `m'.
void liberar_map(mapping_t &m)
{ 
    delete m; 
}

// Devuelve `true' si y solo si `m' tiene CAP_MAP asociaciones.
bool es_lleno_map(mapping_t m)
{ 
    return (*m).cantidad == CAP_MAP;
}

// Devuelve `true' si y solo si `clave' tiene un valor asociado en `m'.
bool es_clave(char clave, mapping_t m)
{ 
    return buscar(clave, m) != -1; 
}

// Devuelve `true' si y solo si `valor' está asociado a una clave en `m'.
bool es_valor(char valor, mapping_t m)
{
   int i = (*m).cantidad - 1;
   while (i >= 0)
   {
     if ((*m).datos[i].valor == valor) break;
     else i--;
   }

   return i >= 0;
}

// Devuelve el resultado de agregar en `m' la asociación (clave,valor).
// Precondición: !es_lleno_map(m) && !es_clave(clave, m).
mapping_t asociar(char clave, char valor, mapping_t m)
{
   assert(!es_lleno_map(m) && !es_clave(clave, m));
  
   (*m).datos[(*m).cantidad].clave = clave;
   (*m).datos[(*m).cantidad].valor = valor;
   (*m).cantidad++;
  
   return m;
}

// Devuelve el valor que en `m' está asociado a `clave'.
// Precondición: es_clave(clave, m)
char valor(char clave, mapping_t m)
{
    assert(es_clave(clave, m));
    return (*m).datos[buscar(clave, m)].valor;
}

// Devuelve el resultado de remover de `m' la asociación de `clave' con su valor.
// Precondición: es_clave(clave, m)
mapping_t desasociar(char clave, mapping_t m)
{
   assert(es_clave(clave, m));
   for (int i = buscar(clave, m); i < (*m).cantidad - 1; i++)
      (*m).datos[i] = (*m).datos[i + 1];

    (*m).cantidad--;
   
    return m;
}
