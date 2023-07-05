#include "cadena.cpp"

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

// Devuelve `true' si y sólo si en `cad' hay un elemento cuyo dato numérico es `i'.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena.
bool pertenece(int i, cadena_t cad)
{
    if (es_vacia_cadena(cad))
        return false;

    localizador_t verificador = inicio_cadena(cad);

    while(es_localizador(verificador) && numero_info(info_cadena(verificador,cad)) != i)
        verificador = siguiente(verificador,cad);

    return es_localizador(verificador) && numero_info(info_cadena(verificador,cad)) == i;
}

// Devuelve la cantidad de elementos de `cad'.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena.
nat longitud(cadena_t cad)
{
    nat contador = 0;

    if (!es_vacia_cadena(cad))
    {
       localizador_t verificador = inicio_cadena(cad);
       
       while(es_localizador(verificador))
       {
           contador = contador + 1;
           verificador = siguiente(verificador,cad);
       }
    }

    return contador;
}

// Devuelve `true' si y sólo si `cad' está ordenada de forma no decreciente (= creciente de manera no estricta) según los datos numéricos de sus elementos.
// Si es_vacia_cadena (cad) devuelve `true'. El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena.
bool esta_ordenada(cadena_t cad)
{
  if (es_vacia_cadena(cad))
      return true;
  
  localizador_t verificador = inicio_cadena(cad);

  while(es_localizador(siguiente(verificador,cad)) && numero_info(info_cadena(verificador,cad)) <= numero_info(info_cadena(siguiente(verificador,cad),cad)) )
       verificador = siguiente(verificador,cad);
 
  return !es_localizador(siguiente(verificador,cad));
}

// Devuelve una cadena_t con todos los elementos de `c1' y `c2' ordenada de manera  creciente segun sus datos numéricos.
// Si hay elementos cuyos datos numéricos son iguales los elementos de `c1' quedan antes de los de `c2'.
// La cadena_t resultado no comparte memoria ni con `c1' ni con `c2'.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena resultado.
// Precondición: esta_ordenada (c1) y esta_ordenada (c2).
cadena_t mezcla(cadena_t c1, cadena_t c2)
{  
    cadena_t resultado =  NULL;
    if ( es_vacia_cadena(c1) ||  es_vacia_cadena(c2))
      resultado = concatenar(c1,c2);
    else
    {
       resultado =  crear_cadena();
       localizador_t loc1 = inicio_cadena(c1);
       localizador_t loc2 = inicio_cadena(c2);
       while (es_localizador(loc2) || es_localizador(loc1))
       {
          info_t nuevoInfoC1 = NULL;
          info_t nuevoInfoC2 = NULL;
          while (es_localizador(loc1) && es_localizador(loc2))
          {
             if (numero_info(info_cadena(loc1,c1)) <= numero_info(info_cadena(loc2,c2)))
             {
                info_t copiaInfoC1 = copia_info(info_cadena(loc1,c1));
                char *fraseCopiaInfoC1 = new char[strlen(frase_info(copiaInfoC1)) + 1];
                strcpy(fraseCopiaInfoC1, frase_info(copiaInfoC1));
                nuevoInfoC1 = crear_info(numero_info(copiaInfoC1), fraseCopiaInfoC1);
                resultado = insertar_al_final(nuevoInfoC1,resultado);
                loc1 = siguiente(loc1,c1);
                liberar_info(copiaInfoC1);
             }
             else
             {
                info_t copiaInfoC2 = copia_info(info_cadena(loc2,c2));
                char *fraseCopiaInfoC2 = new char[strlen(frase_info(copiaInfoC2)) + 1];
                strcpy(fraseCopiaInfoC2, frase_info(copiaInfoC2));
                nuevoInfoC2 = crear_info(numero_info(copiaInfoC2), fraseCopiaInfoC2);
                resultado = insertar_al_final(nuevoInfoC2,resultado);
                loc2 = siguiente(loc2,c2);
                liberar_info(copiaInfoC2);
            }
          }

          if (es_localizador(loc2))
          {
              info_t copiaInfoC2 = copia_info(info_cadena(loc2,c2));
              char *fraseCopiaInfoC2 = new char[strlen(frase_info(copiaInfoC2)) + 1];
              strcpy(fraseCopiaInfoC2, frase_info(copiaInfoC2));
              nuevoInfoC2 = crear_info(numero_info(copiaInfoC2), fraseCopiaInfoC2);
              resultado = insertar_al_final(nuevoInfoC2,resultado);
              liberar_info(copiaInfoC2);
              loc2 = siguiente(loc2,c2);
          }
          else if (es_localizador(loc1))
          {
              info_t copiaInfoC1 = copia_info(info_cadena(loc1,c1));
              char *fraseCopiaInfoC1 = new char[strlen(frase_info(copiaInfoC1)) + 1];
              strcpy(fraseCopiaInfoC1, frase_info(copiaInfoC1));
              nuevoInfoC1 = crear_info(numero_info(copiaInfoC1), fraseCopiaInfoC1);
              resultado = insertar_al_final(nuevoInfoC1,resultado);
              liberar_info(copiaInfoC1);
              loc1 = siguiente(loc1,c1);
          }
      }
  }

  return resultado;
}

// Devuelve el resultado de concatenar `c2' después de `c1'.
// La cadena_t resultado no comparte memoria ni con `c1' ni con `c2'.
// El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos en la cadena resultado.
cadena_t concatenar(cadena_t c1, cadena_t c2)
{ 
  localizador_t iterador = inicio_cadena(c1);
  cadena_t cadRetorno = crear_cadena();

  while(es_localizador(iterador))
  {
     info_t copiaInfoIterador = copia_info(info_cadena(iterador, c1));
     cadRetorno = insertar_al_final(copiaInfoIterador, cadRetorno);
     iterador = siguiente(iterador, c1);
  }

  iterador = inicio_cadena(c2);

  while(es_localizador(iterador))
  {
     info_t copiaInfoIterador = copia_info(info_cadena(iterador, c2));
     cadRetorno = insertar_al_final(copiaInfoIterador, cadRetorno);
     iterador = siguiente(iterador, c2);
  }

  return cadRetorno;
}

// Se ordena `cad' de manera creciente según los datos numéricos de sus elementos. Devuelve `cad'.
// Si es_vacia_cadena (cad) no hace nada.
// El tiempo de ejecución es O(n^2), siendo `n' la cantidad de elementos en la cadena resultado.
cadena_t ordenar(cadena_t cad)
{
    localizador_t loc = inicio_cadena(cad);
    localizador_t busca = NULL;

    if (!esta_ordenada(cad))
    {
      info_t infoDeloc = info_cadena(loc,cad);
      info_t infoDebusca = info_cadena(loc,cad); // por simple inicializacion
      while (es_localizador(loc))
      {
          busca = menor_en_cadena(loc,cad);
          infoDebusca = info_cadena(busca,cad);
          
          if (numero_info(infoDebusca) < numero_info(infoDeloc))
             cad = intercambiar(loc,busca,cad); 

          if (es_localizador(siguiente(loc,cad)))
             infoDeloc = info_cadena(siguiente(loc,cad),cad);
          
          loc = siguiente(loc,cad);
      }
    } 
  
    return cad;
}

// Cambia todas las ocurrencias de `original' por `nuevo' en los elementos de `cad'. No debe quedar memoria inaccesible.
// Devuelve `cad'. El tiempo de ejecución es O(n), siendo `n' la cantidad de elementos enla cadena resultado.
cadena_t cambiar_todos(int original, int nuevo, cadena_t cad)
{
    localizador_t iterador = inicio_cadena(cad);
    localizador_t aux = NULL;

    while(es_localizador(iterador))
    {
        if (numero_info(info_cadena(iterador, cad)) == original)
        {
            aux = siguiente(iterador, cad);
            info_t infoIterador = info_cadena(iterador, cad);

            char *fraseCopiaInfo = new char[strlen(frase_info(infoIterador)) + 1];
            strcpy(fraseCopiaInfo, frase_info(infoIterador));

            info_t nuevoInfo = crear_info(nuevo, fraseCopiaInfo);
            cad = insertar_antes(nuevoInfo, iterador, cad);
            cad = remover_de_cadena(iterador, cad);

            iterador = aux;
        }
        else
        {
          iterador = siguiente(iterador, cad);
        }
    }

    return cad;
}
