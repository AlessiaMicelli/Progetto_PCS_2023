// empty_class.hpp

#ifndef __EMPTY_H
#define __EMPTY_H

#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include <cmath>
#include <vector>
#include <map>
#include <Eigen/Eigen>
#include<list>
#include <iostream>


using namespace std ;
using namespace Eigen;


namespace dalaunay{

class Point{  // definisco la classe Point che ha come attributi la coordinata x e y e il suo identificatore
public:
 double x;
 double y;
 int id;
Point(double x, double y) : x(x), y(y), id(0) {}  // costruttore che mi chiede solo le coordinate
Point(double x, double y,int id) : x(x), y(y), id(id) {} //costruttore
Point() : x(0), y(0), id(0) {} //costruttore vuoto

//  operatori per operazioni sui punti (definiscono le operazioni classiche per la classe)
 Point operator-(const Point & right)
    {
       return Point(x - right.x,y - right.y);
    }
 Point operator+(const Point & right)
    {
       return Point(x + right.x,y + right.y);
    }
 inline bool operator==( const Point p2) const
 {
    return (x==p2.x && y==p2.y);
 }
 inline bool operator!=( const Point p2) const
 {
    return (x!=p2.x || y!=p2.y);
 }


 static double dist(Point& p1,Point&p2) // funzione definita in una classe che vale anche all'esterno della classe (calcola la distanza tra due punti)
 {
    return (p1.x-p2.x)(p1.x-p2.x)+(p1.y-p2.y)(p1.y-p2.y);
 }

};


class Lati{
public:
    int id;
    Point points[2];

    Lati(){} // costruttore vuoto
    Lati(Point p1,Point p2) // costruttore dati i due punti del lato
    {
        points[0] = p1;
        points[1] = p2;
        id = 0;
    }

    Lati(Point p1,Point p2,int i) // costruttore dati due punti del lato e il suo identificatore
    {
        points[0] = p1;
        points[1] = p2;
        id = i;
    }

    // operatori per operazioni sui lati
    inline bool operator==( const Lati l) const
    {
       return ((points[0]==l.points[0] || points[0]==l.points[1])&&(points[1]==l.points[0] || points[1]==l.points[1]));
    }
    inline bool operator!=( const Lati l) const
    {
       return ((points[0]!=l.points[0] && points[0]!=l.points[1])||(points[1]!=l.points[0] && points[1]!=l.points[1]));
    }

};



class Triangle{
  public:
 // attributi
    int id;
    Point  points[3];  // array di punti di cui è formato
    Lati  sides[3];  // array di lati di cui è formato
    int adiacenti[3] = {-1,-1,-1};  // id dei triangoli a cui è adiacente

 Triangle(Point p1,Point p2,Point p3) // costruttore dati tre punti
 {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    ordinapunti();  //funzione che ordina i vertici in senso antiorario (applico metodo nel costruttore)
    sides[0] = Lati(points[0],points[1]);
    sides[1] = Lati(points[1],points[2]);
    sides[2] = Lati(points[2],points[0]);
 }
 Triangle(Point p1,Point p2,Point p3,int t,int l) // costruttore dati tre punti, il suo id e l'id dei suoi lati
 {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    id = t;
    ordinapunti();
    sides[0] = Lati(points[0],points[1],l);
    sides[1] = Lati(points[1],points[2],l+1);
    sides[2] = Lati(points[2],points[0],l+2);
 }

 Triangle(Lati l1,Lati l2,Lati l3) // costruttore dati tre lati
 {
    sides[0] = l1;
    sides[1] = l2;
    sides[2] = l3;
    points[0] = l1.points[0];
    points[1] = l2.points[0];
    points[2] = l3.points[0];
    ordinapunti();
 }

 Triangle(){} // costruttore vuoto

void ordinapunti () // restituisce il triangolo ordinato in senso antiorario
 {
    Point AB = points[1]-points[0];
    Point AC = points[2]-points[0];
    double z = AB.x*AC.y-AB.y*AC.x;
    if (z<0)
    {
        Point  tmp = points[2];
        points[2] = points[1];
        points[1] = tmp;
    }

 }

// operatori per operazioni sui triangoli
 inline bool operator==( const Triangle t2) const
 {
    return ((points[0]==t2.points[0]||points[0]==t2.points[1]||points[0]==t2.points[2])&& (points[1]==t2.points[0]||points[1]==t2.points[1]||points[1]==t2.points[2])&& (points[2]==t2.points[0]||points[2]==t2.points[1]||points[2]==t2.points[2]));
 }
 inline bool operator!=( const Triangle t2) const
 {
    return ((points[0]!=t2.points[0]&&points[0]!=t2.points[1]&&points[0]!=t2.points[2])|| (points[1]!=t2.points[0]&&points[1]!=t2.points[1]&&points[1]!=t2.points[2])|| (points[2]!=t2.points[0]&&points[2]!=t2.points[1]&&points[2]!=t2.points[2]));
 }

 };


 void Delaunay();
}



#endif //__EMPTY_H



//////////////////////////////////////////////////////////////////////////////////////////////
/// empty_class.cpp

#include "empty_class.hpp"
#include "convex_hull.hpp"
#include "funzioni.hpp"
#include "funzioni_triangoli.hpp"


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

namespace dalaunay
{
void Delaunay()
{
    string InputFilePath = "C:/Users/sarab/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/Test2.csv";
    vector<Point> punti;

    // leggo i dati dal file
    InputFile(InputFilePath,punti);

    // prendo il convexhull
    int id_triangoli = 0;
    int id_lati = 0;
    vector<Triangle> triangoli;  // creo vettore dinamico di triangoli (mi serve dimensione generica)
    vector <Lati> lati; // creo un vettore dinamico di lati
    vector <Point> convexhull =  ConvexHull(punti);  // dati i punti mi restituisce il Convehull di tali punti (restituisce il vettore dinamico di punti del perimetro)
    triangoli = TriangoliConvexHull(convexhull,lati,id_triangoli,id_lati);  //crea una prima trinagolazione che rispetta l'ipotesi di delaunay

    vector<Point> punti_da_aggiungere;
    for (int i=0;i<int(punti.size());i++)   // per ogni punto, se il punto non fa parte del convexHull lo aggiungo nella lista dei punti da aggiungere
    {
        bool a = true;
         for (int j=0;j<int(convexhull.size());j++)
         {
            if(punti[i] == convexhull[j])
            {
                a=false;
                break;
            }
         }
         if(a)
            {
             punti_da_aggiungere.push_back(punti[i]);
            }

    }


    // cerco dentro a quale triangolo si trova il punto
    for(int i=0;i<int(punti_da_aggiungere.size());i++)
    {
        int posizione = -1;
        if(Posizione(punti_da_aggiungere[i],triangoli,posizione)) //restituisce in posizione (referenza)l'indice del traingol nel vector Triangle
        {
            triangoli_int(triangoli[posizione],punti_da_aggiungere[i],lati,triangoli,id_triangoli,id_lati);  //collega il punto dato con i vertici del triangolo in cui si trova e verifica l'ipotesi di delaunay
        }

    }


    // output schermo
    cout<<"Lati: "<<endl;
    for (int i=0;i<int(lati.size());i++)
    {
        Point p1;
        p1 = lati[i].points[0];
        Point p2;
        p2 = lati[i].points[1];
        cout<<to_string(i+1)<<") "<<"punto di inizio: ("<<p1.x<<","<<p1.y<<")       punto di fine: ("<<p2.x<<","<<p2.y <<")"<<endl;
    }

    // output file
    string output = "C:/Users/sarab/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/outputprova.csv";
    CreoFileOutput(output,lati);
}
}

////////////////////////////////////////////////////////////////
// funzioni.hpp

#ifndef __FUNZIONI_H
#define __FUNZIONI_H


#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include <cmath>
#include <vector>
#include <map>
#include <Eigen/Eigen>
#include<list>
#include <iostream>
#include "empty_class.hpp"


using namespace std ;
using namespace Eigen;


namespace dalaunay
{

void CreoFileOutput(string outputFilePath, vector<Lati>mylist);
void InputFile(string inputFilePath,vector<Point> &punti);


#endif // __FUNZIONI_H


////////////////////////////////////////////////
// funzioni.cpp

#include "funzioni.hpp"
#include "template.hpp"


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


namespace dalaunay
{
    void InputFile(string inputFilePath, vector<Point>& punti)
        {
            ifstream file; // apro file in modalità lettura
            file.open(inputFilePath);
            if (file.fail())
            {
                cerr<< "file open failed"<< endl;
            }

            else
            {
                cout<<"import ok"<<endl;
            }
            string line;
            getline(file,line);  // leggo ogni linea del file
            int id = 0;
            while (getline(file,line))//while file >> file
            {
             istringstream converter(line); //converter
             double x,y;
             converter>>id>>x>>y;
             punti.push_back(Point(x,y,id)); // inserisco il nuovo punto letto nella lista punti

            }
        file.close();
    }

    // stampo tutti i lati della triangolazione finale
    void CreoFileOutput(string outputFilePath, vector<Lati> myList)
    {
        ofstream outputFile(outputFilePath); // Apriamo un nuovo file in modalità di scrittura

        if (outputFile.is_open())
        {
            outputFile<<"X1 Y1 X2 Y2"<<endl;
            for (int i=0; i<<myList.size(); i++)             {
                Point p1 ;
                p1 = myList[i].points[0];
                Point p2 ;
                p2 = myList[i].points[1];

                outputFile << p1.x  << " " << p1.y << " " << p2.x << " " << p2.y << "\n"; // Scrivo ogni elemento della lista separato da uno spazio nel file, mettendo separate le componenti delle x e delle y dei punt finali e iniziali del lato
            }

            outputFile.close(); // Chiudiamo il file dopo aver terminato la scrittura
            cout << "Lista scritta nel file con successo!" << std::endl;
        }
        else
        {
            cout << "Impossibile aprire il file." << endl;
        }

    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sorting.hpp
#ifndef __SORTING_H
#define __SORTING_H

#include "iostream"
#include "list"
#include "Eigen/Eigen"
#include "map"
#include <vector>

using namespace std;
using namespace Eigen;

namespace SortLibrary {

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





//////////////////////////////////////////////////////////////////////////
// convex_hull.hpp
#ifndef __CONVEXHULL_H
#define __CONVEXHULL_H


#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include <cmath>
#include <vector>
#include <map>
#include <Eigen/Eigen>
#include<list>
#include <iostream>
#include "empty_class.hpp"
#include "funzioni.hpp"
#include "ipotesi_e_flip.hpp"
#include "funzioni_triangoli.hpp"
#include "sorting.hpp"

using namespace std ;
using namespace Eigen;

namespace dalaunay
{

  constexpr double max_tolerance(const double& x, const double& y)
  {
    return x > y ? x : y;
  }

  inline double normSquared(const double& x, const double& y)
  {
    return x * x + y * y;
  }

  inline ostream& operator<<(ostream& os, const Point& p2)
  {
    os << p2.id;
    return os;
  }

  inline bool operator>(const Point& p1, const Point& p2)
  {
    return p1.x > p2.x ;
  }

  inline bool operator<=(const Point& p1, const Point& p2)
  {
    return !(p1 > p2);
  }

  void MergeHulls(vector<Point>& sortedV,
                  const unsigned int& cx);


  void MergeHull(vector<Point>& sortedV,
                  const unsigned int& sx,
                  const unsigned int& dx);

  bool UpperLine( const Point& p1,
                  const Point& p2,
                  const Point& p3);

  vector<Point> ConvexHull(vector<Point>& points);
  vector<Triangle> TriangoliConvexHull (vector<Point> punti,vector <Lati>& lati,int& t_id,int& l_id);


}

#endif // __CONVEXHULL_H

/////////////////////////////////////////////////////////////
// convex_hull.cpp

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


namespace dalaunay {

bool UpperLine(const Point& p1,          //  dati tre punti restituisce true se la linea
               const Point& p2,
               const Point& p3)
{
  double dx1 = p2.x - p1.x;
  double dy1 = p2.y - p1.y;
  double dx2 = p3.x - p1.x;
  double dy2 = p3.y - p1.y;
  if (dx1*dy2 > dy1*dx2)
    return true;
  else if (dx1*dy2 < dy1*dx2)
    return false;
  else
    return (dx1*dx2+dy1*dy2 >= 0);  // faccio il prodotto scalare per vedere se i tre punti sono allineati
}


  void MergeHulls(vector<Point>& sortedV, const unsigned int& cx)
      {
        // definisco gli iteratori per la metà sinistra e destra del vettore sortedV
        auto left = cx;
        auto right = cx + 1;
        // trovo il valore minore dei due hulls
        //se upperline=false non ho ancora trovato il punto
        while (left>0 && right<sortedV.size()-1 && !UpperLine(sortedV[left-1],sortedV[left], sortedV[right]))
        {
          if (sortedV[left-1].y > sortedV[right+1].y)  // se quello di sinistra è maggiore di quello a destra, decremento l'iteratore lest
          {
            --left;
          }
          else  // altrimenti incremento right
          {
            ++right;
          }
        }

        // faccio il merge dei due hulls
        vector<Point> newHull; //nuovo vettore che conterrà le due parti
        newHull.reserve(sortedV.size());  // il vettore newHull deve essere in grado di contenere tanti elementi quanto la dimensione del vettore sortedV
        // copio la metà sinistra dell'hull nel nuovo hull
        for (auto i=cx;i>=left;--i)
        {
          newHull.push_back(sortedV[i]);
        }
        // copio la metà sinistra dell'hull nel nuovo hull
        for (auto i=cx+1;i<=right;++i)
        {
          newHull.push_back(sortedV[i]);
        }
        // sostuisco il vettore ordinato con il nuovo hull
        sortedV.swap(newHull);
      }


  void MergeHull(vector<Point>& sortedV, const unsigned int& sx, const unsigned int& dx)  // chiamo ricorsivamente la funzione stessa sulla metà sinistra e poi sulle metà destra
  {
      unsigned int cx;
      if(sx<dx)
      {
          cx = (sx + dx)/2;
          MergeHull( sortedV, sx, cx);
          MergeHull( sortedV, cx+1, dx);
          MergeHulls(sortedV, cx);    // unisco le due metà con la funzione definita precedentemente
      }
  }


   vector<Point> ConvexHull(vector<Point>& points)
  {
      if(points.size() < 2)
        return points;
      else{
        SortLibrary::MergeSort(points,0,int(points.size()-1));


    // eseguo il lowerHull (punti del convex hull inferiore -pavimento-)
    vector<Point> lowerHull;
    for(unsigned int i=0;i<points.size();++i)
    {
      while(lowerHull.size()>= 2 && !UpperLine(lowerHull[lowerHull.size()-2],lowerHull[lowerHull.size()-1],points[i]))
        {
          lowerHull.pop_back();
        }
      lowerHull.push_back(points[i]);
    }

    // eseguo l'upperHull (punti del convex hull superiore -tetto-)
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

    // concateno i due hulls nel vettore hulls
    vector<Point> hull(lowerHull); //metto tutto lowerhull dentro
    hull.insert(hull.end(), upperHull.begin(), upperHull.end()); //prendo tutti gli elementi di UpperHull (dal primo all'ultimo)e li inserisco alla fine di hull

    return hull;
  }
 }

    //prende in input i punti del convex hull salvati in vector<Point> punti, vector <Lati>& lati inizialmente è vuoto
    vector<Triangle> TriangoliConvexHull (vector<Point> punti,vector <Lati>& lati,int& t_id,int& l_id)   //li passo come referenze perchè li voglio avere in output aggiornati
         {
              vector<Triangle> triangoli;
              int size = int(punti.size());
              for(int i=0;i<size;i++)  //crea i lati del bordo
              {
                Lati l = Lati(punti[i],punti[(i+1)%size],l_id);
                l_id++;
                lati.push_back(l);
              }
              for(int i=1;i<size-1;i++)
              {
                  Triangle nuovoTriangolo = Triangle(punti[0],punti[i],punti[(i+1)%size],t_id,l_id);  //parto dal punto 0 e lo collego al secondo e al terzo punto consecutivi a lui

                  t_id++;l_id++;l_id++;l_id++; //indici che vado a incrementare per dare id idversi a triangoli diversi
                  if(i==1)
                  {
                      triangoli.push_back(nuovoTriangolo);  //c'è un solo trinagolo (non ho bisogno di verificare le ipotesi di D)
                  }
                  else
                  {
                  Lati l = Lati(punti[0],punti[i],l_id);  //creo e aggiungo il nuovo lato al vettore dei lati
                  l_id++;
                  lati.push_back(l); //il lato più esterno lo aggiungo all'iterazione successiva
                  triangoli.push_back(nuovoTriangolo); //aggiungo il nuovo triangolo
                  int indice_nuovoT = int(triangoli.size()-1);  //salvo indice del nuovo triangolo
                  for(int j=0;j<int(triangoli.size()-1);j++)
                  {
                     if(verifica_adiacenza(triangoli[indice_nuovoT],triangoli[j],l))  //restituisce true se c'è un lato in comune tra i due triangoli in input (l è la referenza al lato in comune)
                      {
                        for(int k=0;k<3;k++)
                        {
                          if(triangoli[indice_nuovoT].sides[k]==l)  //array di dim 3 dove ogni elemento contiene l'ide del triangolo adiacenti al lato corrispondente al lato i
                            triangoli[indice_nuovoT].adiacenti[k]=triangoli[j].id;
                          if(triangoli[j].sides[k]==l)  //aggiorno le adiacenze anche per i triangoli precedenti
                            triangoli[j].adiacenti[k]=triangoli[indice_nuovoT].id;
                        }
                      }
                  }
                  ipotesi_int(triangoli[indice_nuovoT].points[2],triangoli[indice_nuovoT],triangoli,lati,t_id,l_id);  //verifico ipotesi di D
                  }
              }
              return triangoli; //resituiso il vettore di triangoli
    }
}

////////////////////////////////////////////////////////////////////////////////
// funzioni_triangoli.hpp
#ifndef __FUNZIONITRIANGOLI_H
#define __FUNZIONITRIANGOLI_H


#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include <cmath>
#include <vector>
#include <map>
#include <Eigen/Eigen>
#include<list>
#include <iostream>
#include "empty_class.hpp"
#include "convex_hull.hpp"
#include "funzioni.hpp"
#include "ipotesi_e_flip.hpp"



using namespace std ;
using namespace Eigen;



namespace dalaunay{

bool dentro ( Point a, Point b, Point c);
bool isInsideCircumcircle(Triangle t, Point q);
bool ispointin(Triangle t, Point p);
int trova_id(vector<Triangle> triangoli,int id);
void removeElement_int(vector<int>& vec, int val);
void triangoli_int(Triangle t, Point p,vector <Lati> &lati,vector<Triangle> &triangoli,int& t_id,int& l_id);
bool Posizione(Point p,vector<Triangle> triangoli,int& id );
bool verifica_adiacenza(Triangle t1, Triangle t2,Lati &l);
void aggiorna_adiacenza(Triangle& old,Triangle& nuovo,vector<Triangle>& triangoli);
int trova_posizione(vector<T> vec,T val);
}


#endif //__FUNZIONITRIANGOLI_H


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////7
//funzioni_triangoli.cpp
#include "empty_class.hpp"
#include "convex_hull.hpp"
#include "funzioni.hpp"
#include "ipotesi_e_flip.hpp"
#include "funzioni_triangoli.hpp"
#include "template.hpp"




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


namespace dalaunay
{
bool dentro ( Point a, Point b, Point c)   // ritorna falso se z è minore di zero (cioè se è destra)
{
    Point AB = b-a;
    Point AC = c-a;
    double z = AB.x*AC.y-AB.y*AC.x; //calcolo il determinante
    if (z<0)
        return false;
    else
        return true;
}


 bool ispointin(Triangle t, Point p)   // dice se il punto p sta dentro il triangolo t
 {
    if(!isInsideCircumcircle(t,p)) //verifica se il punto sta dentro il cerchio circoscritto al triangolo
        return false;
    else
    {
        Point a = t.points[0];
        Point b = t.points[1];
        Point c = t.points[2];
        if(dentro(a,b,p) && dentro(b,c,p) && dentro(c,a,p))   //sto verificando che oltre a stare dentro il cerchio, p stia dentro il triangolo, chiamando 3 volte dentro
        {
            return true;
        }
        else
            return false;
    }
 }


 bool isInsideCircumcircle(Triangle t, Point q)  // ritorna vero se il punto è dentro il cerchio circoscritto al triangolo
 {
     Point a = t.points[0];
     Point b = t.points[1];
     Point c = t.points[2];

     double ax = a.x - q.x;
     double ay = a.y - q.y;
     double bx = b.x - q.x;
     double by = b.y - q.y;
     double cx = c.x - q.x;
     double cy = c.y - q.y;

     double det = ax * (by * (cx*cx+cy*cy) - cy*(bx*bx+by*by)) - bx * (ay*(cx*cx+cy*cy) - cy*(ax*ax+ay*ay)) + cx * (ay*(bx*bx+by*by) - by*(ax*ax+ay*ay));

     return det > 0.0;

 }


int trova_id(vector<Triangle> triangoli,int id)  // trova la posizione del triangolo nel vector triangoli con id chiesto
{
    int pos = -1;
    for(int i=0;i<int(triangoli.size());i++)  // ciclo sulla lunghezza della lista
    {
        if(triangoli[i].id==id)  // se l'id di quel triangolo corrisponde a quello cercato mi resistuisce la posizione
            pos = i;
    }
    return pos;
}

template<typename T>
  int trova_posizione(vector<T> vec,T val) // restituisce la posizone del triangolo val nel vettore vec di triangoli
  {
      int pos = -1;
      for(int i=0;i<int(vec.size());i++)
      {
          if(vec[i]==val)
              pos = i;
      }
      return pos;
  }


void triangoli_int(Triangle t, Point p,vector <Lati> &lati,vector<Triangle> &triangoli,int& t_id,int& l_id)  // crea i 3 triangoli interni al triangolo t collegando p con i vertici del triangolo t
{
    Triangle old=t;   //devo eliminare t (trinagolo grande) da triangoli
    triangoli.push_back(Triangle(old.sides[0].points[0],old.sides[0].points[1],p,t_id,l_id));  //aggiunge il triangolo formato dal punto p e dai punti del primo lato del triangolo adiacente a quello dato
    t_id++;l_id++;l_id++;l_id++;  // incremento id dei triangoli di 1 e dei lati di 3
    triangoli.push_back(Triangle(old.sides[1].points[0],old.sides[1].points[1],p,t_id,l_id));
    t_id++;l_id++;l_id++;l_id++;
    triangoli.push_back(Triangle(old.sides[2].points[0],old.sides[2].points[1],p,t_id,l_id));
    t_id++;l_id++;l_id++;l_id++;

    int ind = trova_posizione(triangoli,Triangle(old.sides[0].points[0],old.sides[0].points[1],p)); // mi dice la posizione del primo nuovo triangolo creato nella lista triangoli
    for(int i=0;i<3;i++) //per ognuno dei 3 triangoli nuovi assegno le adiacenze
       {
           if(old.adiacenti[i]!=-1)  // se il vecchio triangolo ha un triangolo adiacente
            //devo capire qual è l'id del triangolo adiacente, a quale lato di old era adiacente e sostituisco l'id del nuovo alle adiacenze del triangolo adiacente
           {
               triangoli[ind+i].adiacenti[0] = old.adiacenti[i]; //trasmissione adiacenze dal vecchio ai nuovi
               // viceversa
               int indice = trova_id(triangoli, old.adiacenti[i]);//posizione in triangoli dell' adiacente al vecchio triangolo
               for(int j=0;j<3;j++)
               {
                   if(triangoli[indice].adiacenti[j]==old.id)
                   {
                       triangoli[indice].adiacenti[j] = triangoli[ind+i].id;
                   }    // aggiorno l'adiacenza dei triangoli intorno con quelli nuovi
               }
           }

           lati.push_back(Lati(old.points[i],p,l_id));
           l_id++;
       }
       removeElement(triangoli,old);   //elimino il triangolo grande dal vettore triangoli di triangoli
       // basterebbe fare ind-1
       ind = trova_posizione(triangoli,Triangle(old.sides[0].points[0],old.sides[0].points[1],p));

       // salvo le adiacenze tra i 3 nuovi triangoli tra di loro
       for(int i=0;i<3;i++)
       {
          for (int j=0;j<3;j++)
          {
              if(triangoli[ind].sides[j]==triangoli[ind+1].sides[i])
              {
                  triangoli[ind].adiacenti[j] = triangoli[ind+1].id;
                  triangoli[ind+1].adiacenti[i] = triangoli[ind].id;
              }
              if(triangoli[ind].sides[j]==triangoli[ind+2].sides[i])
              {
                  triangoli[ind].adiacenti[j] = triangoli[ind+2].id;
                  triangoli[ind+2].adiacenti[i] = triangoli[ind].id;
              }
              if(triangoli[ind+1].sides[j]==triangoli[ind+2].sides[i])
              {
                  triangoli[ind+1].adiacenti[j] = triangoli[ind+2].id;
                  triangoli[ind+2].adiacenti[i] = triangoli[ind+1].id;
              }
          }
       }
       // verifichiamo l'ipotesi per tutti e 3 i triangoli nuovi
       ipotesi_int(p,triangoli[ind],triangoli,lati,t_id,l_id);
       //devo ritrovare le posizioni dei triangoli perchè quando faccio dei flip ed elimino dei triangoli scalano in avanti
       int ind2 = trova_posizione(triangoli,Triangle(old.sides[1].points[0],old.sides[1].points[1],p,t_id,l_id));
       ipotesi_int(p,triangoli[ind2],triangoli,lati,t_id,l_id);
       int ind3 = trova_posizione(triangoli,Triangle(old.sides[2].points[0],old.sides[2].points[1],p,t_id,l_id));
       ipotesi_int(p,triangoli[ind3],triangoli,lati,t_id,l_id);
   }


  bool Posizione(Point p,vector<Triangle> triangoli,int&id )  // dice l'id del triangolo in cui il punto p è dentro
  {
    bool in = false;
    for (int j=0;j<int(triangoli.size());j++)
    {
        if(ispointin(triangoli[j],p))
        {
            in = true;
            id = j;
        }
    }
    return in;
  }


 bool verifica_adiacenza(Triangle t1, Triangle t2,Lati &l)  //dati due triangoli mi dice se sono adiacenti e mi restituisce il lato tra essi come referenza
 {
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(t1.sides[i]==t2.sides[j])  // cicla sui lati dei due triangoli, se sono uguali restituisce true e mi salva il lato
            {
                l = t1.sides[i];
                return true;
            }
        }
    }
    return false;
 }

// dati due triangoli uno prima e uno dopo il flip, trovo il lato che hanno in comune e mi salvo l'adicenza dal vecchio al nuovo

}


////////////////////////////////////////////////////////////////////////////////////////////////
/// ipotesi_flip.hpp

#ifndef __IPOTESIEFLIP_H
#define __IPOTESIEFLIP_H

#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include <cmath>
#include <vector>
#include <map>
#include <Eigen/Eigen>
#include<list>
#include <iostream>
#include "empty_class.hpp"
#include "convex_hull.hpp"
#include "funzioni.hpp"
#include "funzioni_triangoli.hpp"



using namespace std ;
using namespace Eigen;



namespace dalaunay
{
void trova_nuovo_lato(Triangle ABC, Triangle CBD, Lati oldlato,Lati & newlato,int& l_id);
void Flip(Triangle& ABC, Triangle& CBD, Lati& oldlato,vector<Triangle>& triangoli,vector<Lati>&lati,int& t_id,int& l_id, Point p);
void ipotesi_int(Point punto,Triangle& t,vector<Triangle>&triangoli,vector<Lati>&lati,int& t_id,int& l_id);
}



#endif // __IPOTESIEFLIP_H

//////////////////////////////////////////////////////////////////////////////////////////////
/// ipotesi_flip.cpp
#include "ipotesi_e_flip.hpp"
#include "empty_class.hpp"
#include "convex_hull.hpp"
#include "funzioni.hpp"
#include "funzioni_triangoli.hpp"
#include "template.hpp"



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
namespace dalaunay
{

//VERIFICA IPOTESI: una volta che divido il triangolo in 3 sottotrinagoli, mi trovo il triangolo adiacente a ciascuno di essi e faccio
//il cerchio circoscritto a tale triangoli e verifico se il cerchio contiene oppure no il vertice del triangolo che non fa parte del lato in comune col triangolo adiacente

void ipotesi_int(Point punto,Triangle& t,vector<Triangle>&triangoli,vector<Lati>&lati,int& t_id,int& l_id)
{
   // non è necessaria una lista perchè verifica sempre e solo per un triangolo
   vector<int> triangoli_da_verificare; //conterrà gli id
   int id_da_verificare;
   for(int i=0;i<3;i++)
   {
       if(t.adiacenti[i]!=-1)  // se ha dei triangoli adiacenti
       {
           int posizione_adiacente = trova_id(triangoli,t.adiacenti[i]);  // restituisce posizione del triangolo adiacente a t con id salvato in adiaenti[i]
           if(triangoli[posizione_adiacente].points[0]!=punto && triangoli[posizione_adiacente].points[1]!=punto && triangoli[posizione_adiacente].points[2]!=punto)  // se il punto non fa parte del triangolo in posizione adiacente
            triangoli_da_verificare.push_back(t.adiacenti[i]);  // non devo verificare l'ipotesi per i lati che condivide con i due triangolini che ho creato (cerco triangoli che non hanno punto p come vertice)
       }
   }
   while (triangoli_da_verificare.size()>0)  // finchè ci sono triangoli da verificare
   {
       id_da_verificare = triangoli_da_verificare.back();  // parto dal fondo (cioè prendo l'ultimo id)
       int posizione_adiacente = trova_id(triangoli,id_da_verificare);
           if(isInsideCircumcircle(triangoli[posizione_adiacente],punto)) // non verifica l'ipotesi di Delaunay: punto sta dentro il cerchio circoscritto al triangolo adicante?
           {
            Lati l;
            bool a = verifica_adiacenza(t,triangoli[posizione_adiacente],l); //mi interessa sapere che l è il dato adiacente
            if (a) //sono sempre adiacenti
            {
              int posizione_t = trova_id(triangoli,t.id);
              Triangle t1 = triangoli[posizione_t];
              Triangle t2 = triangoli[posizione_adiacente];
              Flip(t1,t2,l,triangoli,lati,t_id,l_id,punto);
            }
           }
        triangoli_da_verificare.pop_back();  //tolgo da vector di int l'id del triangolo che ho già verificato
   }
}

void trova_nuovo_lato(Triangle t1, Triangle t2, Lati oldlato,Lati & newlato,int& l_id)  // trova il nuovo lato che serve per il flip
// trovo i due punti che non appartengono al lato in comune
{
   Point a;
   Point b;
   for(int i=0;i<3;i++)
       //in entrambi i triangoli trovo il vertice che non appartiene al lato in comune
   {
       if(t1.points[i]!=oldlato.points[0] && t1.points[i]!=oldlato.points[1]) //se il punto del triangolo 1 è diverso da quelli del lato vecchio
           a = t1.points[i];
       if(t2.points[i]!=oldlato.points[0] && t2.points[i]!=oldlato.points[1])  //se il punto del triangolo 2 è diverso da quelli del lato vecchio
           b = t2.points[i];
   }
   newlato = Lati(a,b,l_id);  // creo il lato con i due punti trovati
   l_id++;  // incremento id perchè è una funzione che chiamo più volte e quindi deve aggiornare l'id del lato nuovo creato
}


void Flip(Triangle& ABC, Triangle& CBD, Lati& oldlato,vector<Triangle>& triangoli,vector<Lati>&lati,int& t_id,int& l_id, Point p)
{  // devo creare il nuovo lato
   removeElement( lati, oldlato);  // se faccio il flip devo rimuovere oldlato dalla lista lati
   Lati newlato;
   trova_nuovo_lato(ABC,CBD,oldlato,newlato,l_id);  // trova il nuovo lato che si deve formare
   lati.push_back(newlato);  //aggiungo alla lista lati il nuovo lato creato

   // creo nuovi triangoli
     Triangle ADB = Triangle(newlato.points[0], newlato.points[1], oldlato.points[0],t_id,l_id );  // creo il nuovo triangolo con i punti del nuovo lato e uno del vecchio lato
     t_id++;l_id++;l_id++;l_id++;  // incremento il numero dei lati di 3 e il numero del triangolo di uno
     Triangle ADC = Triangle(newlato.points[0], newlato.points[1], oldlato.points[1],t_id,l_id );
     t_id++;l_id++;l_id++;l_id++;


     removeElement( triangoli, ABC);  // rimuovo i triangoli che c'erano prima del flip
     removeElement( triangoli, CBD);
   // aggiorno le adiacenzed
     aggiorna_adiacenza(ABC,ADB,triangoli);
     aggiorna_adiacenza(ABC,ADC,triangoli);
     aggiorna_adiacenza(CBD,ADB,triangoli);
     aggiorna_adiacenza(CBD,ADC,triangoli);
   // creo le adiacenze tra i due nuovi triangoli creati
     Lati l;
     verifica_adiacenza(ADB,ADC,l);  // verifico se i due triangoli sono adiacenti
     for (int i=0;i<3;i++)
     {
         if(ADB.sides[i]==l) // se il lato i del nuovo triangolo ABD è quello in comune
             ADB.adiacenti[i] = ADC.id;  // metto come adiacente il triangolo ADC
         if(ADC.sides[i]==l)
             ADC.adiacenti[i] = ADB.id;
     }
     triangoli.push_back(ADB);  // inserisco i due nuovi triangoli nella lista
     triangoli.push_back(ADC);
     ipotesi_int(p,ADB,triangoli,lati,t_id,l_id);  // verifivo che sia rispettata l'ipotesi di Delaunay per ogni triangolo aggiunto
     ipotesi_int(p,ADC,triangoli,lati,t_id,l_id);
 }



void aggiorna_adiacenza(Triangle& old,Triangle& nuovo,vector<Triangle>& triangoli)  //fa ereditare le adiacenze quando faccio il flip
{
    Lati lato;  // creo un lato
    if(verifica_adiacenza(old,nuovo,lato))  //devo trovare il lato in comune
      {
       int a;
       for(int i=0;i<3;i++)
        {
          for (int j=0;j<3;j++)
          {
            if(nuovo.sides[j]==old.sides[i] && nuovo.sides[j]==lato)  // se un lato del nuovo triangolo è uguale a uno del vecchio ed è uguale a quello in comune
             {
                nuovo.adiacenti[j] = old.adiacenti[i];  // inserisco quel lato negli adiacenti a quelli del  nuovo triangolo
                a = i;  // mi salvo la posizione
             }
           }
         }
            // aggiorno anche le adiacenze del triangolo adiacente (cioè ora è adiacente al triangolo post flip)
       int posizione = trova_id(triangoli,old.adiacenti[a]);
       for (int j=0; j<3; j++)
           {
             if(triangoli[posizione].adiacenti[j]==old.id)
              {
                triangoli[posizione].adiacenti[j] = nuovo.id;
              }
           }
         }
}

}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main_programm.cpp
#include "empty_class.hpp"
#include "convex_hull.hpp"
#include "funzioni.hpp"
#include "ipotesi_e_flip.hpp"
#include "funzioni_triangoli.hpp"
#include "template.hpp"



int main()
{
    dalaunay::Delaunay();
  return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// test_empty.hpp
#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H



#include <gtest/gtest.h>
#include "empty_class.hpp"
#include "funzioni_triangoli.hpp"
#include "funzioni.hpp"
#include "ipotesi_e_flip.hpp"
#include "convex_hull.hpp"
#include "template.hpp"

#include "iostream"
#include "sorting.hpp"
#include "vector"



using namespace testing;
using namespace dalaunay;
using namespace std;




TEST(TestDelaunay, TestOrdinaPunti)
{

    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);

    dalaunay::Triangle Triangolo(p1,p2,p3);
    dalaunay::Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(Triangolo, TriangoloOrdinato);
}


//TEST FUNZIONI TRIANGOLI

TEST(TestDelaunay,TestDentro)
{
    Point p1(0.0,0.0);
    Point p2(1.0,1.0);
    Point psx(0.333,1.0);

    EXPECT_TRUE(dentro(p1,p2,psx));
}

TEST(TestDelaunay,TestFuori)
{
    Point p1(0.0,0.0);
    Point p2(1.0,1.0);
    Point psx(1.0,0.3333);

    EXPECT_FALSE(dentro(p1,p2,psx));
}

TEST(TestDelaunay,TestIsInsideCircumCircle)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point pbetween(0.6,0.6);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_TRUE(isInsideCircumcircle(TriangoloOrdinato,pbetween));
}

TEST(TestDelaunay,TestIsPointOutside)
{
    dalaunay::Point p1(0.0,0.0);
    dalaunay::Point p2(0.0,1.0);
    dalaunay::Point p3(1.0,0.0);
    dalaunay::Point pext(5.0,6.0);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(ispointin(TriangoloOrdinato,pext), false);
}

TEST(TestDelaunay,TestIsPointOutside2)
{
    Point p1(0.915736,0.138624);
    Point p2(0.0971318,0.917194) ;
    Point p3(0.0357117,0.254282);
    Point pext(0.814724,0.276025) ;
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(ispointin(TriangoloOrdinato,pext), false);
}

TEST(TestDelaunay,TestIsPointInside)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point pint(0.25,0.25);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(ispointin(TriangoloOrdinato,pint), true);
}

TEST(TestDelaunay,TestIsPointBetween)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point pbetween(0.6,0.6);
    Triangle TriangoloOrdinato(p1,p3,p2);

    EXPECT_EQ(ispointin(TriangoloOrdinato,pbetween), false);
}


TEST(TestDelaunay,TestTrovaId)
{
    Point p1(0.0,0.0);
    Point p2(1.0,0.0);
    Point p3(0.0,1.0);
    Triangle abc (p1,p2,p3);
    abc.id=2;
    Point p4(2.0, 1.0);
    Triangle cbd (p3,p2,p4);
    cbd.id = 4;
    Point p5(10.0,10.0);
    Point p6(11.0,11.0);
    Point p7(12.0,12.0);
    Triangle def (p5,p6,p7);
    def.id = 5;
    vector<Triangle> ListaTriangoliOutput;
    ListaTriangoliOutput.push_back(abc);
    ListaTriangoliOutput.push_back(cbd);
    ListaTriangoliOutput.push_back(def);


    EXPECT_EQ(trova_id(ListaTriangoliOutput, 2), 0);
}

TEST(TestDelaunay,TestTrovaTriangolo)
{
    Point p1(0.0,0.0);
    Point p2(1.0,0.0);
    Point p3(0.0,1.0);
    Lati lato(p2,p3);
    Triangle abc (p1,p2,p3);
    abc.id=2;
    Point p4(2.0, 1.0);
    Triangle cbd (p3,p2,p4);
    cbd.id=4;
    vector<Triangle> ListaTriangoliOutput;
    ListaTriangoliOutput.push_back(abc);
    ListaTriangoliOutput.push_back(cbd);

    EXPECT_EQ(trovatriangolo(ListaTriangoliOutput,lato,2), 1);
}

TEST(TestDelaunay,TestTriangoliInt){
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point pInt(0.25,0.25);
    int i=1;
    int j=0;

    Triangle Triangolo(p1,p3,p2);

    //vector<Triangle> ListaTriangoli;
    //vector<Triangle> ListaTriangoliOutput;
    vector <Lati> lati;
    vector<Triangle> ListaTriangoli;
    ListaTriangoli.push_back(Triangolo);

    Triangle Triangolo1(p1,p3,pInt);
    Triangle Triangolo2(pInt,p3,p2);
    Triangle Triangolo3(p1,pInt,p2);
    vector<Triangle> ListaTriangoliOutput;
    ListaTriangoliOutput.push_back(Triangolo1);
    ListaTriangoliOutput.push_back(Triangolo2);
    ListaTriangoliOutput.push_back(Triangolo3);
    triangoli_int(Triangolo,pInt,lati,ListaTriangoli,i,j);//,lati

    EXPECT_EQ(ListaTriangoli,ListaTriangoliOutput);
}

TEST(TestDelaunay,TestPosizione)
{
    Point p1(0.0,0.0);
    Point p2(1.0,0.0);
    Point p3(0.0,1.0);
    Triangle abc (p1,p2,p3);
    abc.id=2;
    Point p4(2.0, 1.0);
    Triangle cbd (p3,p2,p4);
    cbd.id = 4;
    Point p5(10.0,10.0);
    Point p6(11.0,11.0);
    Point p7(12.0,12.0);
    Triangle def (p5,p6,p7);
    def.id = 5;
    Point p8(100.0,100.0);
    vector<Triangle> ListaTriangoliOutput;
    ListaTriangoliOutput.push_back(abc);
    ListaTriangoliOutput.push_back(cbd);
    ListaTriangoliOutput.push_back(def);
    int a = 4;

    EXPECT_EQ(Posizione(p8,ListaTriangoliOutput,a), false);
}

TEST(TestDelaunay, TestAggiorna_Adiacenze)
{

//    tu fai flip e hai due triangoli vecchi e due nuovi
//    vuoi trovare tra vecchio e nuovo il lato in comune e salvare nell'adiacenza del triangolo nuovo, nella posizione giusta,
//    l'adiacenza che aveva il triangolo vecchio in quella posizione,
//    quindi per il test devi definire l'adiacenza nel triangolo old e vedere che quell'adiacenza la salvi in triangolo nuovo


    Point p1(0.0,1.0,1);
    Point p4(1.0,0.0,4);
    Point p3(2.0,1.0,3);
    Point p2(2.0,2.0,2);

    Lati l1 =Lati(p3,p2,1);  // lato adiacente t1 t2
    Lati l2 =Lati(p2,p1,2);
    Lati l3 =Lati(p1,p3,3);
    Lati l4 =Lati(p4,p3,4);
    Lati l5 =Lati(p2,p4,5);

    Triangle old = Triangle(l1,l2,l3);
    old.id = 1;
    old.adiacenti[0] = 3  ;    // vogliamo che in new, nella posizione del lato l1 (che è quello adiacente con old), si conservi l'adiacenza di old con il triangolo 3
    Triangle nuovo = Triangle(l4,l1,l5);
    nuovo.id = 2;

    vector<Triangle> triangoli;
    triangoli.push_back(old);
    triangoli.push_back(nuovo);

    aggiorna_adiacenza(old,nuovo,triangoli);
    // Verifica del risultato
    //cout << "conservazione di 3 adiacente a old in nuovo: " << nuovo.adiacenti[0] << ", " << nuovo.adiacenti[1] << ", " << nuovo.adiacenti[2] << std::endl;

    EXPECT_EQ(nuovo.adiacenti[1],3);
   }

TEST(TestDelaunay,TestVerifica_adiacenza)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,1.0);
    Point p4(2.0,0.0);
    Triangle t1 = Triangle(p1,p2,p3);
    Triangle t2 = Triangle(p1,p4,p3);
    Lati l;
    bool a = verifica_adiacenza(t1,t2,l);
    EXPECT_EQ(a,true);
}

TEST(TestDelaunay, TestTrovaNuovoLato)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,1.0);
    Point p4(2.0,0.0);
    Triangle t1 = Triangle(p1,p2,p3);
    Triangle t2 = Triangle(p1,p4,p3);
    Lati oldlato = Lati(p2,p3,1);
    Lati l1 = Lati(p1,p4,2);
    Lati lato;
    int id;
    trova_nuovo_lato(t1, t2, oldlato,lato, id);
    EXPECT_EQ(l1, lato);

}





// TEST FUNZIONI

TEST(TestDelaunay, TestInput)
{
    vector <Point> points;
    string InputFilePath = "C:/Users/sarab/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/Test2.csv";
    vector<Point> punti{
    Point(0.814723686393179, 0.276025076998578),
    Point( 0.905791937075619, 0.679702676853675),
    Point( 0.126986816293506, 0.655098003973841),
    Point( 0.913375856139019, 0.162611735194631),
    Point( 0.63235924622541, 0.118997681558377),
    Point( 0.0975404049994095, 0.498364051982143),
    Point( 0.278498218867048, 0.959743958516081),
    Point( 0.546881519204984, 0.340385726666133),
    Point( 0.957506835434298, 0.585267750979777),
    Point( 0.964888535199277, 0.223811939491137),
    Point( 0.157613081677548, 0.751267059305653),
    Point( 0.970592781760616 ,0.255095115459269),
    Point( 0.957166948242946, 0.505957051665142),
    Point( 0.485375648722841, 0.699076722656686),
    Point( 0.8002804688888, 0.890903252535798),
    Point( 0.141886338627215, 0.959291425205444),
    Point( 0.421761282626275, 0.547215529963803),
    Point( 0.915735525189067, 0.138624442828679),
    Point( 0.792207329559554, 0.149294005559057),
    Point( 0.959492426392903, 0.257508254123736),
    Point( 0.655740699156587, 0.840717255983663),
    Point( 0.0357116785741896, 0.254282178971531),
    Point( 0.849129305868777, 0.814284826068816),
    Point( 0.933993247757551, 0.243524968724989),
    Point( 0.678735154857773, 0.929263623187228),
    Point( 0.757740130578333, 0.349983765984809),
    Point( 0.743132468124916, 0.196595250431208),
    Point( 0.392227019534168, 0.251083857976031),
    Point( 0.655477890177557, 0.616044676146639),
    Point( 0.171186687811562, 0.473288848902729),
    Point( 0.706046088019609, 0.351659507062997),
    Point( 0.0318328463774207, 0.830828627896291),
    Point( 0.27692298496089, 0.585264091152724),
    Point( 0.0461713906311539, 0.54972360829114),
    Point( 0.0971317812358475, 0.91719366382981),
    Point( 0.823457828327293, 0.285839018820374),
    Point( 0.694828622975817, 0.757200229110721),
    Point( 0.317099480060861, 0.753729094278495),
    Point( 0.950222048838355, 0.380445846975357),
    Point( 0.0344460805029088, 0.567821640725221),
    Point( 0.438744359656398, 0.0758542895630636),
    Point( 0.381558457093008, 0.0539501186666071),
    Point( 0.765516788149002, 0.530797553008973),
    Point( 0.795199901137063, 0.779167230102011),
    Point( 0.186872604554379, 0.934010684229183),
    Point( 0.489764395788231, 0.12990620847373),
    Point( 0.445586200710899, 0.568823660872193),
    Point( 0.646313010111265, 0.469390641058206),
    Point( 0.709364830858073, 0.0119020695012414),
    Point(0.754686681982361, 0.337122644398882)};

    InputFile(InputFilePath,points);
    EXPECT_EQ(points,punti);
}

TEST(TestDelaunay, TestFileOutput)
{
    Point p1(0.0,0.0);
    Point p2(0.0,1.0);
    Point p3(1.0,0.0);
    Point p4(1.0,1.0);
    Point p5(2.0,2.0);
    Point p6(3.4,3.1);
    vector<Lati> myList{Lati(p1, p2), Lati(p3, p4), Lati(p5, p6)};
    string outputFilePath = "C:/Users/sarab/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/fileLati.csv";
    CreoFileOutput(outputFilePath, myList);
}


// TEST IPOTESI E FLIP

TEST(TestDelaunay, TestFlip)
{
    vector<Lati> lati;
    vector<Triangle> ListaTriangoli;
    Point p1(0.0,0.0,0);
    Point p2(1.0,0.0,1);
    Point p3(0.0,1.0,2);
    Triangle ABC (p1,p2,p3,0,0);
    Point p4(2.0, 1.0,3);
    Triangle CBD (p3,p2,p4,1,3);
    ListaTriangoli.push_back(ABC);
    ListaTriangoli.push_back(CBD);
    Lati l;
    verifica_adiacenza(ABC,CBD,l);
    int id = 0;
    Flip(ABC, CBD,l,ListaTriangoli,lati,id,id,p1);
    Triangle ADB (p1,p4,p2,2,8);
    Triangle ADC (p1,p4,p3,3,11);
    EXPECT_EQ(l,Lati(p2,p3));
}

TEST(TestDelaunay, TestIpotesi_int)
{
    vector<Point> punti = {Point(0.0,0.0),Point(0.3,1.0),Point(1.0,1.0),Point(2.0,2.0),Point(0.5,2.0),Point(1.1,1.1),Point(0.7,4.0),Point(4.0,0.0),Point(2.2,1.1),Point(0.2,1.4)};
    vector<Lati> lati;
    int t_id;
    int l_id;
    vector<Point> puntiConvexHull = ConvexHull(punti);
    vector<Triangle> triangoli = TriangoliConvexHull(puntiConvexHull,lati,t_id,l_id);
    Point p1(0.9,0.5);
    vector<Lati> lati_ipotesi;
    int t_id_ipotesi;
    int l_id_ipotesi;
    Triangle t;
    ipotesi_int(p1,t, triangoli, lati_ipotesi, t_id_ipotesi, l_id_ipotesi);
    int num = triangoli.size();
    int count = 0;
    for (int i=0; i<num; i++)
    {
        bool a = isInsideCircumcircle(triangoli[i],p1);
        if (a)
            count++;
    }
    EXPECT_EQ(num,count);
}
#endif // __TEST_EMPTY_H




//TEST CONVEXHULL
#ifndef __TEST_CONVEXHULL_H
#define __TEST_CONVEXHULL_H
using namespace testing;
using namespace dalaunay;
using namespace std;
using namespace SortLibrary;



TEST(TestSorting, TestMergeSort)
{
  vector<int> v = {44, 25, 10, 31, 25, 48, 37, 43, 18, 48, 27};
  SortLibrary::MergeSort<int>(v, 0, v.size()-1);
  vector<int> sortedV = {10, 18, 25, 25, 27, 31, 37, 43, 44, 48, 48};
  EXPECT_EQ(v, sortedV);
}

TEST(TestConvexHull, Test1Point)
{
  unsigned int numPoints = 0;
  vector<Point> points = { Point(2, 2, numPoints++) };

  vector<Point> convexHull = ConvexHull(points);

  string convexHullToString;
  ostringstream convert(convexHullToString);

  for(unsigned int i = 0; i < convexHull.size(); i++)
    convert << convexHull[i] << " ";

  string rightConveHull = "0 ";

  EXPECT_EQ(convert.str(), rightConveHull);

}

TEST(TestConvexHull, Test2Points)
{
    unsigned int numPoints = 0;
    vector<Point> points = {Point(2, 2, numPoints++),
                            Point(2.8, 1.7, numPoints++)};

    vector<Point> convexHull = ConvexHull(points);

    string convexHullToString;
    ostringstream convert(convexHullToString);

    for(unsigned int i = 0; i < convexHull.size(); i++)
            convert << convexHull[i] << " ";

    string rightConveHull = "0 1 ";

    EXPECT_EQ(convert.str(), rightConveHull);

}

TEST(TestConvexHull, Test3Points)
{
  unsigned int numPoints = 0;
  vector<Point> points = {Point(2, 2, numPoints++),
                          Point(3, 5, numPoints++),
                          Point(2.8, 1.7, numPoints++)};

  vector<Point> convexHull = ConvexHull(points);

  string convexHullToString;
  ostringstream convert(convexHullToString);

  for(unsigned int i = 0; i < convexHull.size(); i++)
    convert << convexHull[i] << " ";

  string rightConvexHull = "0 2 1 ";

  EXPECT_EQ(convert.str(), rightConvexHull);

}

TEST(TestConvexHull, TestXPoints)
{
  unsigned int numPoints = 0;
  vector<Point> points = {Point(2, 2, numPoints++),
                          Point(2.8, 1.7, numPoints++),
                          Point(3, 5, numPoints++),
                          Point(7, 3, numPoints++),
                          Point(7.2, 5, numPoints++),
                          Point(11, 4.3, numPoints++),
                          Point(12, 5.2, numPoints++),
                          Point(14, 3, numPoints++)
                          };

  vector<Point> convexHull = ConvexHull(points);

  string convexHullToString;
  ostringstream convert(convexHullToString);

  for(unsigned int i = 0; i < convexHull.size(); i++)
    convert << convexHull[i] << " ";

  string rightConveHull = "0 1 7 6 2 ";

  EXPECT_EQ(convert.str(), rightConveHull);

}

TEST(TestConvexHull, TestXFloatingPoints)
{
  unsigned int numPoints = 0;
  vector<Point> points = {Point(0.3215348546593775, 0.03629583077160248, numPoints++),
                          Point(0.02402358131857918, -0.2356728797179394, numPoints++),
                          Point(0.04590851212470659, -0.4156409924995536, numPoints++),
                          Point(0.3218384001607433, 0.1379850698988746, numPoints++),
                          Point(0.11506479756447, -0.1059521474930943, numPoints++),
                          Point(0.2622539999543261, -0.29702873322836, numPoints++),
                          Point(-0.161920957418085, -0.4055339716426413, numPoints++),
                          Point(0.1905378631228002, 0.3698601009043493, numPoints++),
                          Point(0.2387090918968516, -0.01629827079949742, numPoints++),
                          Point(0.07495888748668034, -0.1659825110491202, numPoints++),
                          Point(0.3319341836794598, -0.1821814101954749, numPoints++),
                          Point(0.07703635755650362, -0.2499430638271785, numPoints++),
                          Point(0.2069242999022122, -0.2232970760420869, numPoints++),
                          Point(0.04604079532068295, -0.1923573186549892, numPoints++),
                          Point(0.05054295812784038, 0.4754929463150845, numPoints++),
                          Point(-0.3900589168910486, 0.2797829520700341, numPoints++),
                          Point(0.3120693385713448, -0.0506329867529059, numPoints++),
                          Point(0.01138812723698857, 0.4002504701728471, numPoints++),
                          Point(0.009645149586391732, 0.1060251100976254, numPoints++),
                          Point(-0.03597933197019559, 0.2953639456959105, numPoints++),
                          Point(0.1818290866742182, 0.001454397571696298, numPoints++),
                          Point(0.444056063372694, 0.2502497166863175, numPoints++),
                          Point(-0.05301752458607545, -0.06553921621808712, numPoints++),
                          Point(0.4823896228171788, -0.4776170002088109, numPoints++),
                          Point(-0.3089226845734964, -0.06356112199235814, numPoints++),
                          Point(-0.271780741188471, 0.1810810595574612, numPoints++),
                          Point(0.4293626522918815, 0.2980897964891882, numPoints++),
                          Point(-0.004796652127799228, 0.382663812844701, numPoints++),
                          Point(0.430695573269106, -0.2995073500084759, numPoints++),
                          Point(0.1799668387323309, -0.2973467472915973, numPoints++),
                          Point(0.4932166845474547, 0.4928094162538735, numPoints++),
                          Point(-0.3521487911717489, 0.4352656197131292, numPoints++),
                          Point(-0.4907368011686362, 0.1865826865533206, numPoints++),
                          Point(-0.1047924716070224, -0.247073392148198, numPoints++),
                          Point(0.4374961861758457, -0.001606279519951237, numPoints++),
                          Point(0.003256207800708899, -0.2729194320486108, numPoints++),
                          Point(0.04310378203457577, 0.4452604050238248, numPoints++),
                          Point(0.4916198379282093, -0.345391701297268, numPoints++),
                          Point(0.001675087028811806, 0.1531837672490476, numPoints++),
                          Point(-0.4404289572876217, -0.2894855991839297, numPoints++)};

  vector<Point> convexHull = ConvexHull(points);

  string convexHullToString;
  ostringstream convert(convexHullToString);

  for(unsigned int i = 0; i < convexHull.size(); i++)
    convert << convexHull[i] << " ";

  string rightConveHull = "32 39 6 23 37 30 14 31 ";

  EXPECT_EQ(convert.str(), rightConveHull);

}

//MANCANO TEST TRIANGOLI CONVEXHULL



#endif  //__TEST_CONVEXHULL_H
