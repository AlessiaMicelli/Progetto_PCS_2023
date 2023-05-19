#include "empty_class.hpp"


#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <cmath>


using namespace std;


namespace dalaunay
{

vector<Lati> sides;  //creo una lista di lati

void Delaunay()
{
    cout<<"bhifrebigerr";
    vector<Point> points;


    // LEGGO I DATI


    ifstream file;
    file.open("./test2.csv");
    if (file.fail())
        cout<<"errore";
    else
        cout<<"import ok";
    string line;
    //creo una lista di punti
    getline(file,line);
    while (getline(file, line))//while file >> file
    {
       stringstream ss; //converter
       ss << line;
       int id;
       ss>>id;
       double x,y;
       ss>>x>>y;
       points.push_back(Point(x,y)); // creo le istanze Punto nella lista points
    }
    file.close();


    // PRENDO IL TRIANGOLO DI AREA MAGGIORE


    //mergesort ??? o meglio trovare il triangolo di area meaasima O(n^3)????
    // PRENDO I DUE PUNTI PIù LONTANI
    //confronto tutte le coppie O(n^2)


    Point * p1;
    Point * p2;
    Point * p3;
    Point &a=points[0];
    Point &b=points[1];
    Point &c=points[2];

    Distanzamax(points, a, b, c);

    p1=&a;
    p2=&b;
    p3=&c;

    vector<Triangle> triangles;  // creo una lista di triangoli
    triangles.push_back(Triangle(p1,p2,p3)); // aggiungo il primo triangolo
    vector<Lati> lati;  // creo una lista di lati
    lati.push_back(Lati(p1,p2)); // aggiungo i primi 3 lati
    lati.push_back(Lati(p2,p3));
    lati.push_back(Lati(p3,p1));

    vector<Point*> punti_di_bordo; // andrebbe poi aggiornata!!!!!
    punti_di_bordo.push_back(p1);
    punti_di_bordo.push_back(p2);
    punti_di_bordo.push_back(p3);

    vector<Lati*> lati_di_bordo; // andrebbe poi aggiornata!!!!!
    lati_di_bordo.push_back(triangles[0].sides[0]);
    lati_di_bordo.push_back(triangles[0].sides[1]);
    lati_di_bordo.push_back(triangles[0].sides[2]);

    //creo una lista di punti senza p1 p2 p3
    vector<Point*> point_da_aggiungere;
    for (int i=0;i<int(points.size());i++)
    {
        if(&points[i]==p1||&points[i]==p2||&points[i]==p3)
        {
            continue;
        }
        else
        {
            point_da_aggiungere.push_back(&points[i]);
        }
    }

    // CONTROLLO LA POSIZIONE DEL NUOVO PUNTO RISPETTO AI TRIANGOLI

    for (int i=0;i<int(point_da_aggiungere.size());i++)// per ogni punto
    {
        bool in=false;

        for (int j=0;j<int(triangles.size());j++) // per ogni triangolo
        {
            if(ispointin(triangles[j],point_da_aggiungere[i])) // se il punto sta dentro il triangolo (dereferenzio ())
            {
                // creo 3 nuovi triangoli
                in=true;
                Triangle old=triangles[j];               
                triangles[j]=Triangle(old.points[0],old.points[1],point_da_aggiungere[i]);
                triangles.push_back(Triangle(old.points[1],old.points[2],point_da_aggiungere[i]));
                triangles.push_back(Triangle(old.points[2],old.points[0],point_da_aggiungere[i]));

                // aggiorno le adiacenza dei triangoli
                triangles[j].triangoli_adiacenti.push_back(&triangles[j+1]);
                triangles[j].triangoli_adiacenti.push_back(&triangles[j+2]);
                triangles[j+1].triangoli_adiacenti.push_back(&triangles[j]);
                triangles[j+1].triangoli_adiacenti.push_back(&triangles[j+2]);
                triangles[j+2].triangoli_adiacenti.push_back(&triangles[j+1]);
                triangles[j+2].triangoli_adiacenti.push_back(&triangles[j]);

                // devo verificare ipotesi dalaunay?!
                ///////////////////////////////////////////////////////////////////////////////////////////////////


                break; // esco dal ciclo for
            }

        }
///////////////////////////////////// MODIFICO QUI /////////////////////////////////////

        if(!in) // se il punto è esterno a tutti
        {
           vector<Lati> nuovilati;//vettore con i nuovi lati che non intersecano
           for (int j=0;j<int(punti_di_bordo.size());j++)
           {
                bool intersezione=false;
                Lati l= Lati(point_da_aggiungere[i],punti_di_bordo[j]); // creo un nuovo lato

                for (int k=0;k<int(lati_di_bordo.size());k++) // verifico che l non intersechi nessun lato esterno
                {
                    if(siIntersecano(*l.points[0],*l.points[1],*lati_di_bordo[k]->points[0],*lati_di_bordo[k]->points[1]))
                    {
                        // se l interseca almeno un lato esterno
                        intersezione=true;

                    }
                }

                if(!intersezione) // se non interseca nulla aggiungo il lato
                {
                    lati.push_back(l); //serve?
                    lati_di_bordo.push_back(&l);
                    nuovilati.push_back(l);
                }
           }

           for(int j=0;j<int(nuovilati.size());j++) // per tutti i nuovi lati creati (validi)
           {
               for (int k=j;k<int(nuovilati.size());k++)
               {
                   if(nuovilati[j].points[1]->isconnect(nuovilati[k].points[1])) // trovo il terzo punto del trinagolo che voglio creare
                   {
                       Triangle t=Triangle(nuovilati[j].points[1],nuovilati[k].points[1],point_da_aggiungere[i]);
                       triangles.push_back(t);
                       for (int z=0;z<3;z++)
                       {
                           t.sides[z]->triangoli.push_back(&t);
                       }
                   }
               }

           }
        punti_di_bordo.push_back(point_da_aggiungere[i]);
        }

    }


/*
        if(!in) // se il punto è esterno a tutti
        {
           for (int j=0;j<int(triangles.size());j++)  // per ogni triangolo
           {
               for(int k=0;k<3;k++)
               {
                Lati l= Lati(point_da_aggiungere[i],triangles[j].points[k]);


                // [3] DA FARE : ( sotto in creolati )
                // sfutto la lista punti_di_bordo
                //verifico che non crea intersezioni
                // se non interseca aggiungo


                sides.push_back(l);

               }

               //fare una funzione crea triangoli al posto di tre if????

               if ((point_da_aggiungere[i]->isconnect(triangles[j].points[0]))&&(point_da_aggiungere[i]->isconnect(triangles[j].points[1])))
               {
                   triangles.push_back(Triangle(point_da_aggiungere[i],triangles[j].points[0],triangles[j].points[1]));
               }
               else if ((point_da_aggiungere[i]->isconnect(triangles[j].points[0]))&&(point_da_aggiungere[i]->isconnect(triangles[j].points[2])))
               {
                   triangles.push_back(Triangle(point_da_aggiungere[i],triangles[j].points[0],triangles[j].points[2]));
               }
               else if ((point_da_aggiungere[i]->isconnect(triangles[j].points[1]))&&(point_da_aggiungere[i]->isconnect(triangles[j].points[2])))
               {
                   triangles.push_back(Triangle(point_da_aggiungere[i],triangles[j].points[1],triangles[j].points[2]));
               }
           }
        punti_di_bordo.push_back(point_da_aggiungere[i]);
        }
    }


*/


    //finita la mesh
    // IPOTESI DI DELAUNAY


    for (int i=0;i<int(point_da_aggiungere.size());i++)// per ogni punto
    {
        for(int j=0;j<int(point_da_aggiungere[i]->edges.size());j++)// per ognilato di cui fa parte
        {
            for (int k=0;k<int(point_da_aggiungere[i]->edges[j]->triangoli.size());k++) // per ogni trinagolo con quel lato
            {
                for(int z=0;z<int(point_da_aggiungere[i]->edges[j]->triangoli[k]->triangoli_adiacenti.size());z++) // per ogni triangolo adiacente a un triangolo di cui fa parte
                {
                    if(isInsideCircumcircle(*point_da_aggiungere[i]->edges[j]->triangoli[k]->triangoli_adiacenti[z], *point_da_aggiungere[i]))
                    {
                        Flip(*point_da_aggiungere[i]->edges[j]->triangoli[k]->triangoli_adiacenti[z],*point_da_aggiungere[i]->edges[j]->triangoli[k]);
                    }
                }
            }
        }
    }



    // OUTPUT

    for(int i=0;i<int(lati.size());i++)
    {
        cout<<lati[i].points[0]<<endl;
        cout<<lati[i].points[1]<<endl;
    }


}



/// FUNZIONI



Lati * Point::isconnect(Point*p) // verifico se è collegato con p
{
    for (auto l : edges)// per ogni lato l
    {
       if((l->points[0]==this&&p==l->points[1])||(l->points[1]==this&&p==l->points[0]))
       {
           return l;
       }

    }
    return nullptr;
}


void Distanzamax(vector<Point> points,Point& p1,Point& p2,Point& p3)
{
 double max_dist = 0.0;
 double max_altezza = 0.0;
 vector<double> distanze;

 // faccio la mergesort per avere algoritmo più efficiente

 for (int i=0;i<int(points.size());i++)  // ciclo sul vettore point che contiene i punti
 {
    for (int j=0;j<int(points.size());j++)  // doppio ciclo
    {
        if (j!=i)
        {
            double distance=Point::dist(points[i],points[j]);
            distanze.push_back(distance);
            if(distance>max_dist)
            {
                max_dist = distance;
                p1 = points[i];
                p2 = points[j];

            }

        }
    }

 }
 // devo trovare il punto p3 alla massima disanza punto retta per p1 p2
 double m = ((p2.y - p1.y)/(p2.x - p1.x));
 double q = -m*p1.x+ p1.y;
 for (int i=0;i<int(points.size());i++)
 {
    if (points[i]!= p1 && points[i]!= p2)
    {
        double d = (abs(points[i].y - (m*points[i].x+q))/(sqrt(1+m*m)));
        if (d > max_altezza)
        {
            max_altezza = d;
            p3 = points[i];
        }
    }
 }
}


bool dentro ( Point a, Point b, Point c)
{
    Point AB=a-b;
    Point AC=a-c;
    double z =AB.x*AC.y-AB.y*AC.x;
    if (z<0)
        return false;
    else
        return true;
}


 bool ispointin(Triangle t, Point &p)
 {
    if(!isInsideCircumcircle(t,p))
        return false;
    else
    {

        // trovo i due punti più vicini a b
        // FUNZIONE MATTE

        Point * a= t.points[0];
        Point * b= t.points[1];
        Point * c= t.points[2];


        if(dentro( *a, * b,  p)&& dentro( *b, * c,  p)&& dentro( *c, * a,  p))
        {
            return true;
        }
        else
            return false;
    }
 }


 bool isInsideCircumcircle(Triangle t, Point& q)
 {
     Point a=*t.points[0];
     Point b=*t.points[1];
     Point c=*t.points[2];

     double ax = a.x - q.x;
     double ay = a.y - q.y;
     double bx = b.x - q.x;
     double by = b.y - q.y;
     double cx = c.x - q.x;
     double cy = c.y - q.y;

     double det = ax * (by * (cx*cx+cy*cy) - cy*(bx*bx+by*by)) - bx * (ay*(cx*cx+cy*cy) - cy*(ax*ax+ay*ay)) + cx * (ay*(bx*bx+by*by) - by*(ax*ax+ay*ay));

     return det > 0.0;

 }


bool siIntersecano(Point& p1, Point& p2, Point& p3, Point& p4)
{
    int det = (p2.x-p1.x)(p4.y-p3.y)-(p2.y-p1.y)(p4.x-p3.x);
    if (det==0)//caso rette parallele
        return false;
    else
    {
        // retta p1p2
        double m1 = ((p2.y - p1.y)/(p2.x - p1.x));
        if (isnan(m1))// retta parallela all'asse x
            m1=10e10;
        double q1 = -m1*p1.x+ p1.y;
        //retta p3p4
        double m2 = ((p4.y - p3.y)/(p4.x - p3.x));
        if (isnan(m2))// retta parallela all'asse x
            m2=10e10;
        double q2 = -m2*p3.x+ p3.y;
        // componente x del punto di intersezione
        double px=(q1-q2)/(m2-m1);
        if (isnan(px))
            px=(p3.x+p4.x)/2;////////////////////////////////////////////
        // componente y del punto di intersezione
        double py=m1*((q1-q2)/(m2-m1))+q1;
        if (isnan(py))
            py=(p3.y+p4.y)/2;

        Point p(px,py);
        // alpha per la componente x
        double ax=(px-p1.x)/(p2.x-p1.x);
        if (isnan(ax))
            ax=0;
        // alpha per la componente y
        double ay=(py-p1.y)/(p2.y-p1.y);
        if (isnan(ay))
            ay=0;
        // gli alpha devono appartenere a [0,1] per far parte del segmento
        if ((ax>=0&&ax<=1)&&(ay>=0&&ay<=1))
            return true;
        else
            return false;
    }
}



/*
void creotriangoli(Point p1,Point p2)
{
    for (int i=0; i< int(p2.edges.size());i++)
    {
        if(*p2.edges[i]->points[0]==p2)
        {
            Point p=*p2.edges[i]->points[1];
        }
        else
        {Point p=*p2.edges[i]->points[0];}


    }
}
*/

/*
void creolati(Triangle &t)
{
    Lati * l1=t.points[0]->isconnect(t.points[1]);
    if (l1==nullptr) // se non eseiste il lato
    {

        sides.push_back(Lati(t.points[0],t.points[1])); // creo i nuovi lati del triangolo


        // [3] DA FARE:
        //creare funzione che verifica le intersezioni
        // aggiungo il lato e il nuovo triangolo


    }


}
*/


/// DA VERIFICARE


// Verifica l'ipotesi di Delaunay tra due triangoli adiacenti

bool isDelaunayValid( Point& a,  Point& b,  Point& c,  Point& d)
{
    // Calcola i quadrati delle distanze tra i punti
    double abSquared = std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
    double acSquared = std::pow(a.x - c.x, 2) + std::pow(a.y - c.y, 2);
    double adSquared = std::pow(a.x - d.x, 2) + std::pow(a.y - d.y, 2);
    double bcSquared = std::pow(b.x - c.x, 2) + std::pow(b.y - c.y, 2);
    double bdSquared = std::pow(b.x - d.x, 2) + std::pow(b.y - d.y, 2);
    // Verifica l'ipotesi di Delaunay
    if ((abSquared * acSquared * adSquared) + (abSquared * bcSquared * bdSquared) >
        (acSquared * bcSquared * bdSquared) + (abSquared * acSquared * bdSquared)) {
        return false;
    }
    return true;

}
/*
void flip(Triangle& ABC, Triangle& CBD) {

   Point a = *ABC.points[0];
   Point b = *ABC.points[1];
   Point c = *ABC.points[2];
   Point d = *CBD.points[0];
   Point e = *CBD.points[1];
   Point f = *CBD.points[2];

   Lati oldlato;
   Lati newlato;
   int k = 0;
   for (int i=0; i<3;i++) {
       for (int j=0; j<3; j++) {
           if (ABC.points[i] == CBD.points[j]) {
               oldlato.points[k] = ABC.points[i];  //lato adiacente
               k++;}

        }
    }
    for (int i=0; i<2;i++){
        for (int j=0; j<3; j++){
            if (oldlato.points[i] != ABC.points[j])
            {
                newlato.points[i] =  ABC.points[j];
            } // copiare uguale per il triangolo CBD
        }  // chiamando i costuttore triangolo a partire dal nuovo punto di newlato crea i triangoli
    }

}
*/

void Flip(Triangle& ABC, Triangle& CBD) {

   Lati oldlato;
   Lati newlato;

   int k = 0;
   for (int i=0; i<3;i++) {                // punti triangolo ABC
       for (int j=0; j<3; j++) {           // punti triangolo CBD
           if (ABC.points[i] == CBD.points[j])
           {
               oldlato.points[k] = ABC.points[i];     //salvo in oldlato il lato adiacente
               k++;

           }
           // se segui il ragionamento del ciclo non dovrebbe sovrascrivere:
           //  A = C, A = B, A = D,
           //  B = C, B = B (SI), B = D,
           //  C = C (SI), C = B, C = D,   -> oldato è points[0] = B, points[1] = C
        }
    }


    for (int j=0; j<3; j++){

        if (oldlato.points[0] != ABC.points[j]&&oldlato.points[1] != ABC.points[j])
        {   // prendo il vertice opposto in ABC al lato adiacente
            newlato.points[0] =  ABC.points[j];    // lo salvo in newlato[0]

        };

    }

    for (int j=0; j<3; j++)
    {
        if (oldlato.points[0] != CBD.points[j]&&oldlato.points[1] != CBD.points[j])
        {   // prendo il vertice opposto in CBD al lato adiacente
            newlato.points[1] =  CBD.points[j];    // lo salvo in newlato[1]
        };

    // chiamando i costuttore triangolo a partire dal nuovo punto di newlato crea i triangoli
    }
    // sovrascrivo i triangoli
    ABC = Triangle(newlato.points[0], newlato.points[1], oldlato.points[0] ); //ADB
    CBD = Triangle(newlato.points[0], newlato.points[1], oldlato.points[1] ); //ADC

}




 /// DA AGGIUNGERE
 /// per migliorare


/*

 /// da mettere in ispointin
Point p_giusto1;
Point p_giusto2;

void FunzioneMatte(*p0, *p1, *p2, *p3, &p_giusto1, &p_giusto2)
{

    vector<double> m[3];
    vector<double> q[3];
    vector<double> d[3];
    vector<Point> p[3];
    p.push_back(p1,p2,p3);


    for (int i=0; i<3; i++)
    {
        m[i] = ((p[(i+1)%3].y() - p[i].y())/(p[(i+1)%3].x() - p[i].x()));
        q[i] = -m[i]*p[i].x() + p[i].y();
        d[i] = (abs(p3.y() - (m[i]*p3.x()+q[i]))/(sqrt(1+m[i]*m[i])));
    }

    double minimo=0.0;
    for (int i=0; i<3; i++)
    {
      if (d[i]<minimo)
        minimo=d[i];
    }

    if (minimo==d[2])
    {
        p_giusto1 = &points[0];
        p_giusto2 = &points[2];
    }
    else if (minimo==d[0])
    {
        p_giusto1 = &points[0];
        p_giusto2 = &points[1];
    }
    else
    {
        p_giusto1 = &points[1];
        p_giusto2 = &points[2];
    }
}
*/

/*

double Merge(vector<double> distanze,
           const unsigned int sx,
           const unsigned int cx,
           const unsigned int dx)
{ unsigned int i = sx; unsigned int j = cx+1;
  double massimo = 0.0;
  double massimo_generale = 0.0;
  while ((i <= cx) && (j <= dx))
  {
    if (distanze[i] >= distanze[j])
    {
        massimo = distanze[i]; j = j+1;
    }
    else
    {
      massimo = distanze[j]; i = i+1;
    }
    if (massimo > massimo_generale)
    {
        massimo_generale = massimo;
    }
    }
  return massimo_generale;
   }

double MergeSort(vector<double> distanze,
               const unsigned int sx,
               const unsigned int dx)

{if (sx < dx)
   {
   unsigned int centro = (sx+dx)/2;
   unsigned int meta1 = (sx+centro)/2;
   unsigned int meta2 = (centro+1+dx)/2;
   double a = Merge( distanze, sx, meta1, centro );
   double b = Merge( distanze, centro+1, meta2, dx );
   return (a >b ? a : b);

    }

}


*/






}
