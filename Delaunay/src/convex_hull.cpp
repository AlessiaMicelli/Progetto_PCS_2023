#include "convex_hull.hpp"
#include "empty_class.hpp"


#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <map>
#include <unordered_set>


using namespace std;



namespace Delaunay
{
double tol = 1e-10;
bool UpperLine(const Point& p1,          //  dati tre punti, restituisce un booleano che dice se la linea definita dai tre punti forma un angolo superiore o inferiore
               const Point& p2,
               const Point& p3)
{
  double dx1 = p2.x - p1.x;
  double dy1 = p2.y - p1.y;
  double dx2 = p3.x - p1.x;
  double dy2 = p3.y - p1.y;
  if (dx1*dy2 - dy1*dx2 > tol )
    return true;
  else if (dx1*dy2 < dy1*dx2 + tol)
    return false;
  else
    return (dx1*dx2+dy1*dy2 >= tol);  // faccio prodotto scalare per vedere se i tre punti sono allineati, se prodotto scalare maggiore di zero allora l'angolo è superiore, altrimenti è inferiore
}


   vector<Point> ConvexHull(vector<Point>& points)
  {
      if(points.size() < 2)
        return points;
      else{
        SortLibrary::MergeSort(points,0,int(points.size()-1));  //ordina i punti in base alla x


    // eseguo il lowerHull, punti inferiore del ConvexHull
    vector<Point> lowerHull;
    for(unsigned int i=0;i<points.size();++i)
    {
      while(lowerHull.size()>= 2 && !UpperLine(lowerHull[lowerHull.size()-2],lowerHull[lowerHull.size()-1],points[i])) //punto aggiunto insieme al punto corrente points[i] forma un angolo superiore rispetto alla linea definita dai due punti precedenti nell'hull inferiore.
        {
          lowerHull.pop_back();
        }
      lowerHull.push_back(points[i]);
    }

    // eseguo l'upperHull, punti superiori del ConvexHull
    vector<Point> upperHull;
    for(int i=points.size()-1;i>=0;--i)
    {
      while(upperHull.size() >= 2 && !UpperLine(upperHull[upperHull.size()-2],upperHull[upperHull.size()-1],points[i]))
        {
          upperHull.pop_back();
        }
          upperHull.push_back(points[i]);
    }

    // rimuovo il primo e l'ultimo punto dall'upperHull perchè sono presenti anche nell'hull inferiore
    upperHull.pop_back();
    upperHull.erase(upperHull.begin());

    // concateno i due hulls
    vector<Point> hull(lowerHull); // inserisco tutto lowerHull dentro
    hull.insert(hull.end(), upperHull.begin(), upperHull.end());  // inserisco alla fine del hull il vettore upperHull
    return hull;
  }
 }

    //i punti che prende in input sono quelli del convexhull, inizialmente lati è vuoto
   // forse posso togliere size
    vector<Triangle> TriangoliConvexHull (vector<Point> punti,vector <Lati>& lati,int& t_id,int& l_id)
         {
              vector<Triangle> triangoli;
              int size = int(punti.size());
              for(int i=0;i<size;i++)  // crea i lati del bordo
              {
                Lati l = Lati(punti[i],punti[(i+1)%size],l_id);
                l_id++;
                lati.push_back(l);
              }
              for(int i=1;i<size-1;i++)  // crea triangoli e lati interni
              {
                  Triangle nuovoTriangolo = Triangle(punti[0],punti[i],punti[(i+1)%size],t_id,l_id);// al primo giro collego i punti 0,1,2
                  t_id++;l_id++;l_id++;l_id++;
                  if(i==1)
                  {
                      triangoli.push_back(nuovoTriangolo);
                  }
                  else
                  {
                  Lati l = Lati(punti[0],punti[i],l_id);  // nuovo lato che ho creato
                  l_id++;
                  lati.push_back(l);  // aggiugno il lato ai lati
                  triangoli.push_back(nuovoTriangolo);
                  int indice_nuovoT = int(triangoli.size()-1);
                  for(int j=0;j<int(triangoli.size()-1);j++)
                  {
                      if(verifica_adiacenza(triangoli[indice_nuovoT],triangoli[j],l)) // dati due triangoli mi dice se c'è un lato in comune tra loro, devo salvare adiacenze a quello nuovo(indicetriangoloT) e a quello vecchio(j)
                      {
                        for(int k=0;k<3;k++)  //mi trova lato è uguale a l
                        {
                            // primo if mi verifica adiacenza al triangolo nuovo, il secondo al triangolo vecchio
                          if(triangoli[indice_nuovoT].sides[k]==l)  // nella posizione i dell'array adiacenti di triangoli, voglio l'id del triangolo adiacente al lato i
                            triangoli[indice_nuovoT].adiacenti[k] = triangoli[j].id;
                          if(triangoli[j].sides[k]==l)
                            triangoli[j].adiacenti[k] = triangoli[indice_nuovoT].id;
                        }
                      }
                  }
                  ipotesi_int(triangoli[indice_nuovoT].points[2],triangoli[indice_nuovoT],triangoli,lati,t_id,l_id);  // verifico l'ipotesi
                  }
              }
              return triangoli;
    }
}


