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
void Delaunay()
{
    string InputFilePath = "C:/Users/sarab/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/Test2.csv";
    vector<Point> punti;  // vettore di punti

    // leggo i dati dal file
    InputFile(InputFilePath,punti);

    // prendo il convexhull
    int id_triangoli = 0;
    int id_lati = 0;
    vector<Triangle> triangoli;  // creo un vettore di triangoli
    vector <Lati> lati; // creo un vettore di lati
    vector <Point> convexhull =  ConvexHull(punti);  // dati i punti mi restituisce il Convehull di tali punti (ossia il loro inviluppo convesso)
    triangoli = TriangoliConvexHull(convexhull,lati,id_triangoli,id_lati);  // dato il ConvexHull, crea una prima triangolazione che verifica l'ipotesi di delaunay

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
        int posizione = -1;  // rappresenta la posizione nel vettore, non il suo id
        if(Posizione(punti_da_aggiungere[i],triangoli,posizione))
        {
            triangoli_int(triangoli[posizione],punti_da_aggiungere[i],lati,triangoli,id_triangoli,id_lati);  // collega un punto con i vertici del triangolo e verfifica l'ipotesi
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
    string output = "C:/Users/sarab/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/fileoutput.csv";
    CreoFileOutput(output,lati);
}
}




