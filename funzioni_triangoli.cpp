#include "empty_class.hpp"
#include "convex_hull.hpp"
#include "funzioni.hpp"
#include "ipotesi_e_flip.hpp"
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


namespace Delaunay
{
bool dentro ( Point a, Point b, Point c)   // ritorna falso se è minore di zero, cioè è a destra ( io voglio sia a sinistra quindi vero, cioè c è a sinistra del lato ab)
{
    Point AB = b-a;
    Point AC = c-a;
    double det = AB.x*AC.y - AB.y*AC.x;
    if (det<0)
        return false;
    else
        return true;
}


 bool ispointin(Triangle t, Point p)   // dice se il punto p sta dentro il triangolo t
 {
    if(!isInsideCircumcircle(t,p))  // se è fuori dalla circonferenza circoscritta, non sta nel triangolo
        return false;
    else
    {
        Point a = t.points[0];
        Point b = t.points[1];
        Point c = t.points[2];
        if(dentro(a,b,p) && dentro(b,c,p) && dentro(c,a,p))   //se il punto sta a sinistra di tutti i lati
        {
            return true;
        }
        else
            return false;
    }
 }


 bool isInsideCircumcircle(Triangle t, Point q)  // ritorna vero se il punto è dentro la circonferenza circoscritta
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


int trova_id(vector<Triangle> triangoli,int id)  // trova la posizione del triangolo nella lista triangoli con id chiesto
{
    int pos = -1;
    for(int i=0;i<int(triangoli.size());i++)  // ciclo sulla lunghezza della lista
    {
        if(triangoli[i].id==id)  // se l'id di quel triangolo corrisponde a quello cercato mi resistuisce la posizione
            pos = i;
    }
    return pos;
}


int trova_posizione(vector<Triangle> vec,Triangle t) // restituisce la posizone di val nella lista dei triangoli (serve perchè non c'è corrispondenza tra id e indice nel vettore)
{  // voglio che sia aggionrnato non solo il triangolo nuovo ma anche quello nella lista
    int pos = -1;
    for(int i=0;i<int(vec.size());i++)
    {
        if(vec[i]==t)
            pos = i;
    }
    return pos;
}


void triangoli_int(Triangle t, Point p,vector <Lati> &lati,vector<Triangle> &triangoli,int& t_id,int& l_id)  // gli do un punto interno al triangolo e questa mi crea i tre triangoli collegando il punto a ogni vertice del triangolo
{
    Triangle old = t;   //devo eliminare t da triangoli
    triangoli.push_back(Triangle(old.sides[0].points[0],old.sides[0].points[1],p,t_id,l_id));  // prendo un lato, quindi i suoi punti inziali e finali e il punto p quindi ho 3 punti -> 1 triangolo
    t_id++;l_id++;l_id++;l_id++;  // incremento id dei triangoli di 1 e dei lati di 3
    triangoli.push_back(Triangle(old.sides[1].points[0],old.sides[1].points[1],p,t_id,l_id));
    t_id++;l_id++;l_id++;l_id++;
    triangoli.push_back(Triangle(old.sides[2].points[0],old.sides[2].points[1],p,t_id,l_id));
    t_id++;l_id++;l_id++;l_id++;

    int ind = trova_posizione(triangoli,Triangle(old.sides[0].points[0],old.sides[0].points[1],p)); // mi dice la posizione del primo nuovo triangolo creato nella lista triangoli
    for(int i=0;i<3;i++)  // voglio che i triangoli nuovi conservino le adiacenze di quello old
       {
           if(old.adiacenti[i]!=-1)  // se il vecchio triangolo ha un triangolo adiacente
           {
               triangoli[ind+i].adiacenti[0] = old.adiacenti[i];  //
               int indice = trova_id(triangoli,old.adiacenti[i]);//posizione in triangoli dell' adiacente al vecchio triangolo
               for(int j=0;j<3;j++)
               {
                   if(triangoli[indice].adiacenti[j]==old.id)  // trovo in quale posizione di adiacenti c'er l'id di old e metto l'id del nuovo triangolo
                   {
                       triangoli[indice].adiacenti[j] = triangoli[ind+i].id;  // aggiorno l'adiacenza di triangoli intorno con quelli nuovi
                   }
               }
           }

           lati.push_back(Lati(old.points[i],p,l_id));  // aggiungo i lati creati
           l_id++;
       }
       removeElement(triangoli,old);  // rimuovo old dal vettore triangoli
       // basterebbe fare ind-1
       ind = trova_posizione(triangoli,Triangle(old.sides[0].points[0],old.sides[0].points[1],p));

       // salvo le adiacenze con la corretta corrispondenza adiacenza lato dei tre nuovi triangoli interni
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
        // verifico che che l'ipotesi sia verificata per i nuovi triangoli creati ( posso in questo modo fare dei flip, e aggiugno i nuovi triangoli in coda, quindi i triangoli scalano, perciò devo trovare le posizioni all'interno del vettore
       ipotesi_int(p,triangoli[ind],triangoli,lati,t_id,l_id);
       int ind2 = trova_posizione(triangoli,Triangle(old.sides[1].points[0],old.sides[1].points[1],p,t_id,l_id));
       ipotesi_int(p,triangoli[ind2],triangoli,lati,t_id,l_id);
       int ind3 = trova_posizione(triangoli,Triangle(old.sides[2].points[0],old.sides[2].points[1],p,t_id,l_id));
       ipotesi_int(p,triangoli[ind3],triangoli,lati,t_id,l_id);
   }


  bool Posizione(Point p,vector<Triangle> triangoli,int&id )  // dice l'id del triangolo in cui il punto p è dentro, o false se non è dentro nessun triangolo
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


 bool verifica_adiacenza(Triangle t1, Triangle t2,Lati &l)  //dati due triangoli mi dice se sono adiacenti e mi restituisce il lato tra essi
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
}

// creo triangolok, agigorno adiacenze e aggiungo al vettore triangoli
// creo triangolo, lo aggiungo al vettore dei triangoli,
