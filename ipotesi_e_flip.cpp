#include "ipotesi_e_flip.hpp"
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
namespace Delaunay
{
void trova_nuovo_lato(Triangle t1, Triangle t2, Lati oldlato,Lati & newlato,int& l_id)  // trova il nuovo lato che serve per il flip
{
   Point a;
   Point b;
   for(int i=0;i<3;i++)
   {  // trovo i due punti che non appartengono al lato in comune ai due triangoli (= vertici opposti al lato adiacente)
       if(t1.points[i]!=oldlato.points[0] && t1.points[i]!=oldlato.points[1]) //se il punto del triangolo 1 è diverso da quelli del lato vecchio
           a = t1.points[i];
       if(t2.points[i]!=oldlato.points[0] && t2.points[i]!=oldlato.points[1])  //se il punto del triangolo 2 è diverso da quelli del lato vecchio
           b = t2.points[i];
   }
   newlato = Lati(a,b,l_id);  // creo il lato con i due punti trovati
   l_id++;  // incremento id perchè è una funzione che chiamo più volte e quindi deve aggiornare l'id del lato nuovo creato
}


void aggiorna_adiacenza(Triangle& old,Triangle& nuovo,vector<Triangle>& triangoli)  //fa ereditare le adiacenze
{
    Lati lato;  // creo un lato
    if(verifica_adiacenza(old,nuovo,lato))  // se i due triangoli sono adiacenti
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
            // viceversa  (????)
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


void Flip(Triangle& ABC, Triangle& CBD, Lati& oldlato,vector<Triangle>& triangoli,vector<Lati>&lati,int& t_id,int& l_id, Point p)
{  // devo trovare il nuovo lato
   removeElement( lati, oldlato);  // se faccio il flip devo rimuovere oldlato dal vettore lati quello in comune
   Lati newlato;
   trova_nuovo_lato(ABC,CBD,oldlato,newlato,l_id);  // trova il nuovo lato che si deve formare
   lati.push_back(newlato);  //aggiungo alla lista lati il nuovo lato creato

   // creo nuovi triangoli
     Triangle ADB = Triangle(newlato.points[0], newlato.points[1], oldlato.points[0],t_id,l_id );  // creo il nuovo triangolo con i punti del nuovo lato e uno del vecchio lato
     t_id++;l_id++;l_id++;l_id++;  // incremento il numero dei lati di 3 e il numero del triangolo di uno
     Triangle ADC = Triangle(newlato.points[0], newlato.points[1], oldlato.points[1],t_id,l_id );
     t_id++;l_id++;l_id++;l_id++;


     removeElement(triangoli, ABC);  // rimuovo i triangoli che c'erano prima del flip
     removeElement(triangoli, CBD);
   // aggiorno le adiacenze
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


void ipotesi_int(Point punto,Triangle& t,vector<Triangle>& triangoli,vector<Lati>& lati,int& t_id,int& l_id)  // gli passo un punto(vertice), il triangolo di cui il punto fa parte
{  // prende trianggolo adiacente a quello fornito ( che non contiene il punto), creo circonferenza circoscritta al triangolo adiacente  e verifico che il punto non sia dentro, se sta dentro faccio il flip
   // non è necessaria una lista perchè verifica sempre e solo per un triangolo
   vector<int> triangoli_da_verificare;
   int id_da_verificare;
   for(int i=0;i<3;i++)
   {
       if(t.adiacenti[i]!=-1)  // se ha dei triangoli adiacenti
       {
           int posizione_adiacente = trova_id(triangoli,t.adiacenti[i]);  // restituisce id del triangolo adiacente a t
           if(triangoli[posizione_adiacente].points[0]!=punto && triangoli[posizione_adiacente].points[1]!=punto && triangoli[posizione_adiacente].points[2]!=punto)  // se il punto non fa parte del triangolo in posizione adiacente
            triangoli_da_verificare.push_back(t.adiacenti[i]);  // inserisco nei triangoli da verificare il triangolo i adiacente a t
            // non devo verificare l'ipotesi tra i trei triangoli piccoli che si formano
       }


   }
   while (triangoli_da_verificare.size()>0)  // finchè ci sono triangoli da verificare
   {
       id_da_verificare = triangoli_da_verificare.back();  // parto dal fondo, ultimo id nella lista dei triangoli da verificare (COME SE FOSSE CODA?)
       int posizione_adiacente = trova_id(triangoli,id_da_verificare);  // trovo posizione del triangolo adiacente
           if(isInsideCircumcircle(triangoli[posizione_adiacente],punto)) // verifica l'ipotesi di Delaunay (punto fuori dalla circonferenza circoscritta al triangolo adiacente)
           {
            Lati l;
            bool a = verifica_adiacenza(t,triangoli[posizione_adiacente],l);  // mi serve per avere il lato adiacente ( so già che sono adiacenti)
            if (a)  // se sono adiacenti
            {
              int posizione_t = trova_id(triangoli,t.id);
              Triangle t1 = triangoli[posizione_t];
              Triangle t2 = triangoli[posizione_adiacente];
              Flip(t1,t2,l,triangoli,lati,t_id,l_id,punto);
            }
           }
       triangoli_da_verificare.pop_back();  // dopo averlo verificato lo elimino dalla lista
   }
}

}

// se passo le cose per referenza è perchè voglio che le modifiche fatte lì rimangano anche dopo
