#ifndef __SORTING_H
#define __SORTING_H

#include "iostream"
#include "list"
#include "Eigen/Eigen"
#include "map"
#include <vector>

using namespace std;
using namespace Eigen;

namespace SortLibrary
{
  template<typename T>  // le function template vengono definite nell'hpp
  void Merge(vector<T>& v,
              const unsigned int& sx,
              const unsigned int& cx,
              const unsigned int& dx)
  {
    unsigned int i = sx; unsigned int j = cx+1; unsigned int k = 0;
    vector<T> b(v.size());
    while ((i <= cx) && (j <= dx))  // finchè ho ancora da controllare le due parti
    {
      if (v[i] <= v[j])
      {
        b[k] = v[i];
        i = i+1;
      }
      else
      {
        b[k] = v[j];
        j = j+1;
      }
      k = k+1;
      }
      for ( ; i <= cx; i = i+1, k = k+1)  // quando ho terminato una parte ricopio la rimanente dell'altra
          b[k] = v[i];
      for ( ; j <= dx; j = j+1, k = k+1)
          b[k] = v[j];
      for (i = sx; i <= dx; i = i+1)  // ricopio tutto ciò che ho inserito nel vettore d'appoggio b nel vettore v
          v[i] = b[i-sx];
  }


  template<typename T>
  void MergeSort(vector<T>& v,
                  const unsigned int& sx,  // indici degli elementi all'inizio e alla fine del vettore
                  const unsigned int& dx)

  {if (sx < dx)  // finchè ho ancora un vettore e non un punto
     {
     unsigned int centro = (sx+dx)/2;
     MergeSort( v, sx, centro );  // ordino i due sottovettori
     MergeSort( v, centro+1, dx );
     Merge( v, sx, centro, dx );  // prende i due sottovettori ordinati e ne crea uno ordinato, si applica a due chiamate del MergeSort
     }
  }
}

#endif // __SORTING_H
