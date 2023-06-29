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


namespace dalaunay
{

//creo una lista di lati


void Delaunay()
{

    //string InputFilePath = "C:/Users/matteo/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/Test2.csv";         //
    string InputFilePath = "C:/Users/matteo/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/prova.csv";       //
    vector<Point> punti;


    // LEGGO I DATI
    InputFile(InputFilePath, punti);


    // PRENDO IL CONVEX HULL

    int id_triangoli=0;
    int id_lati=0;
    vector<Triangle> triangoli;
    vector <Lati> lati;
    vector<Point> copia_punti=punti;
    vector<Point> convexhull=ConvexHull(copia_punti);
    triangoli=TriangoliConvexHull(convexhull,lati,id_triangoli,id_lati);

    vector<Point> punti_da_aggiungere;
    for (int i=0;i<int(punti.size());i++)
    {
        bool a= true;
         for (int j=0;j<int(convexhull.size());j++)
         {
            if(punti[i]==convexhull[j])
            {
                a=false;
                break;
            }

         }
         if(a)
            {punti_da_aggiungere.push_back(punti[i]);}

    }

    //////////////////////////////////////////// TRIANGOLI CORRETTI MA ADIACENZE SBAGLIATE!!!!!!


    // CONTROLLO LA POSIZIONE DEL NUOVO PUNTO RISPETTO AI TRIANGOLI
    for(int i=0;i<int(punti_da_aggiungere.size());i++)
    {
        int id=-1;
        if(Posizione(punti_da_aggiungere[i],triangoli,id))
        {
            triangoli_int(triangoli[id],punti_da_aggiungere[i],lati,triangoli,id_triangoli,id_lati);
            //ipotesi_int()
        }
    }

    //elimino i duplicati
    removeDuplicates(lati);
    removeDuplicates(triangoli);

    // output schermo

    cout<<"Lati: "<<endl;
    for (int i=0;i<int(lati.size());i++)
    {
        Point p1 ;
        p1 = lati[i].points[0];
        Point p2 ;
        p2 = lati[i].points[1];
        //cout<<to_string(i+1)<<") "<<"punto di inizio: ("<<p1.x<<","<<p1.y<<")       punto di fine: ("<<p2.x<<","<<p2.y <<")"<<endl;
        cout<<to_string(i+1)<<") "<<"punto di inizio: ("<<p1.id<<      ")     punto di fine: ("<<p2.id<<" )"<<endl;
    }

    // OUTPUT FILE

    //string output="C:/Users/matteo/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/output.csv";
    //CreoFileOutput(output,lati);
    string output="C:/Users/matteo/OneDrive/Desktop/PCS2023_Exercises/Projects/Delaunay/Dataset/outputprova.csv";
    CreoFileOutput(output,lati);
}


/// FUNZIONI


void InputFile(string inputFilePath, vector<Point>& punti)
    {
        ifstream file;
        file.open(inputFilePath);
        if (file.fail()){
            cerr<< "file open failed"<< endl;}

        else{
            cout<<"import ok"<<endl;;
        }
        string line;
        //creo una lista di punti
        getline(file,line);
        int id=0;
        while (getline(file, line))//while file >> file
        {
         istringstream converter(line); //converter

        double x,y;
        converter>>id>>x>>y;
        punti.push_back(Point(x,y,id));
        id++;// creo le istanze Punto nella lista points
        }
    file.close();
}


bool dentro ( Point a, Point b, Point c)
{
    Point AB=b-a;
    Point AC=c-a;
    double z =AB.x*AC.y-AB.y*AC.x;
    if (z<0)
        return false;
    else
        return true;
}


 bool ispointin(Triangle t, Point p)
 {
    if(!isInsideCircumcircle(t,p))
        return false;
    else
    {
        Point a= t.points[0];
        Point b= t.points[1];
        Point c= t.points[2];

        if(dentro( a,  b,  p)&& dentro( b,  c,  p)&& dentro( c, a,  p))
        {
            return true;
        }
        else
            return false;
    }
 }


 bool isInsideCircumcircle(Triangle t, Point q)
 {
     Point a=t.points[0];
     Point b=t.points[1];
     Point c=t.points[2];

     double ax = a.x - q.x;
     double ay = a.y - q.y;
     double bx = b.x - q.x;
     double by = b.y - q.y;
     double cx = c.x - q.x;
     double cy = c.y - q.y;

     double det = ax * (by * (cx*cx+cy*cy) - cy*(bx*bx+by*by)) - bx * (ay*(cx*cx+cy*cy) - cy*(ax*ax+ay*ay)) + cx * (ay*(bx*bx+by*by) - by*(ax*ax+ay*ay));

     return det > 0.0;

 }


bool siintersecano(Point p1, Point p2, Point p3, Point p4)
{
    double det = (p2.x-p1.x)(p4.y-p3.y)-(p2.y-p1.y)(p4.x-p3.x);
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
            px=(p3.x+p4.x)/2;
        // componente y del punto di intersezione
        double py=m1*((q1-q2)/(m2-m1))+q1;
        if (isnan(py))
            py=(p3.y+p4.y)/2;

        Point p(px,py);
        // alpha per la componente x
        double ax=(px-p1.x)/(p2.x-p1.x);
        if (isnan(ax))
            ax=0.1;
        // alpha per la componente y
        double ay=(py-p1.y)/(p2.y-p1.y);
        if (isnan(ay))
            ay=0.1;
        double bx=(px-p3.x)/(p4.x-p3.x);
        if (isnan(bx))
            bx=0.1;
        // alpha per la componente y
        double by=(py-p3.y)/(p4.y-p3.y);
        if (isnan(by))
            by=0.1;
        // gli alpha devono appartenere a [0,1] per far parte del segmento
        if ((ax>0&&ax<1)&&(ay>0&&ay<1)&&(bx>0&&bx<1)&&(by>0&&by<1))
            return true;
        else
            return false;

    }

}


template<typename T>
int trova_posizione(vector<T> vec,T val)
{   // restituisce la posizone di val nella lista vec
    int pos=-1;
    for(int i=0;i<int(vec.size());i++)
    {
        if(vec[i]==val)
            pos=i;
    }
    return pos;
}


int trova_id(vector<Triangle> triangoli,int id)
{// trova la posizione del triangolo nella lista triangoli con id chiesto
    int pos=-1;
    for(int i=0;i<int(triangoli.size());i++)
    {
        if(triangoli[i].id==id)
            pos=i;
    }
    return pos;
}

int trovatriangolo(vector<Triangle> triangoli,Lati l,int id)
{   // restituisce la posizione del triangolo nella lista triangoli che contiene il lato l,con id diverso da id dato
    int pos=-1;
    for(int i=0;i<int(triangoli.size());i++)
    {
        for (int j=0;j<3;j++)
        {
        if(triangoli[i].sides[j]==l && triangoli[i].id!=id)
            pos=i;
        }
    }
    return pos;
}

template<typename T>
void removeElement(vector<T>& vec, T val)
{
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (*it == val) {
            vec.erase(it);
            break;
        }

    }
}
void removeElement_int(vector<int>& vec, int val)
{
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (*it == val) {
            vec.erase(it);
            break;
        }
    }
}

void triangoli_int(Triangle t, Point p,vector <Lati> &lati,vector<Triangle> &triangoli,int& t_id,int& l_id)
{
    Triangle old=t;//devo eliminare t da triangoli
    triangoli.push_back(Triangle(old.sides[0].points[0],old.sides[0].points[1],p,t_id,l_id));t_id++;l_id++;l_id++;l_id++;
    triangoli.push_back(Triangle(old.sides[1].points[0],old.sides[1].points[1],p,t_id,l_id));t_id++;l_id++;l_id++;l_id++;
    triangoli.push_back(Triangle(old.sides[2].points[0],old.sides[2].points[1],p,t_id,l_id));t_id++;l_id++;l_id++;l_id++;

    //ipotesi_int(p,t,triangoli,lati,t_id,l_id);
    int ind=trova_posizione(triangoli,Triangle(old.sides[0].points[0],old.sides[0].points[1],p)); // mi dice la posizione del primo nuovo triangolo creato nella lista triangoli
    for(int i=0;i<3;i++)
    {
        if(old.adiacenti[i]!=-1)
        {
            triangoli[ind+i].adiacenti[0]=old.adiacenti[i];
            // viceversa
            int indice=trova_id(triangoli,old.adiacenti[i]);//posizione in triangoli dell' adiacente al nuovo triangolo
            for(int j=0;j<3;j++)
            {
                if(triangoli[indice].adiacenti[j]==old.id)
                {
                    triangoli[indice].adiacenti[j]=triangoli[ind+i].id;
                }    // aggiorno l'adiacenza dell'altro
            }
        }

        lati.push_back(Lati(old.points[i],p,l_id));
        l_id++;
    }

    triangoli[ind].adiacenti[1]=triangoli[ind+1].id;
    triangoli[ind].adiacenti[2]=triangoli[ind+2].id;
    triangoli[ind+1].adiacenti[1]=triangoli[ind+2].id;
    triangoli[ind+1].adiacenti[2]=triangoli[ind].id;
    triangoli[ind+2].adiacenti[1]=triangoli[ind].id;
    triangoli[ind+2].adiacenti[2]=triangoli[ind+1].id;
    ipotesi_int(p,triangoli[ind],triangoli,lati,t_id,l_id);
    int ind2=trova_posizione(triangoli,Triangle(old.sides[0].points[0],old.sides[0].points[1],p));
    ipotesi_int(p,triangoli[ind2],triangoli,lati,t_id,l_id);
    int ind3=trova_posizione(triangoli,Triangle(old.sides[0].points[0],old.sides[0].points[1],p));
    ipotesi_int(p,triangoli[ind3],triangoli,lati,t_id,l_id);


    removeElement(triangoli,old);

}

  bool Posizione(Point p,vector<Triangle> triangoli,int&id )
  {
    bool in=false;

    for (int j=0;j<int(triangoli.size());j++)
    {
        if(ispointin(triangoli[j],p))
        {
            in=true;
            id=j;
        }
    }
    return in;
  }


 bool verifica_adiacenza(Triangle t1, Triangle t2,Lati &l)
 {//dati due triangoli mi dice se sono adiacenti e mi restituisce il lato tra essi
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            if(t1.sides[i]==t2.sides[j])
            {
                l=t1.sides[i];
                return true;
            }
        }
    }
    return false;
 }

 void trova_nuovo_lato(Triangle t1, Triangle t2, Lati oldlato,Lati & newlato,int& l_id)
 {
    Point a;
    Point b;
    for(int i=0;i<3;i++)
    {
        if(t1.points[i]!=oldlato.points[0]&&t1.points[i]!=oldlato.points[1])
            a=t1.points[i];
        if(t2.points[i]!=oldlato.points[0]&&t2.points[i]!=oldlato.points[1])
            b=t2.points[i];
    }
    newlato=Lati(a,b,l_id);l_id++;
 }

 void aggiorna_adiacenza(Triangle& old,Triangle& nuovo,Lati& l,vector<Triangle>& triangoli,int i,int posizione_adiacente)
 { // fa ereditare le adiacenze

     for(int j=0; j<3; j++)
     {
       if(nuovo.sides[j]==l)
           nuovo.adiacenti[j]=old.adiacenti[i];
       if(triangoli[posizione_adiacente].adiacenti[j]==old.id)
           triangoli[posizione_adiacente].adiacenti[j]=nuovo.id;
     }


 }


 void Flip(Triangle& ABC, Triangle& CBD, Lati& oldlato,vector<Triangle>& triangoli,vector<Lati>&lati,int& t_id,int& l_id)
 {

    // TROVO IL NUOVO LATO
    removeElement( lati, oldlato);

    Lati newlato;
    trova_nuovo_lato(ABC,CBD,oldlato,newlato,l_id);
    lati.push_back(newlato);

    // CREO I NUOVI TRIANGOLI
      Triangle ADB = Triangle(newlato.points[0], newlato.points[1], oldlato.points[0],t_id,l_id ); t_id++;l_id++;l_id++;l_id++;
      Triangle ADC = Triangle(newlato.points[0], newlato.points[1], oldlato.points[1],t_id,l_id ); t_id++;l_id++;l_id++;l_id++;

      for(int i=0; i<3; i++) // per ogni triangolo adiacente ad ABC
           {
               int posizione_adiacente=trova_id(triangoli,ABC.adiacenti[i]); // trova la posizione del triangolo nella lista triangoli con id chiesto
               Lati l;
               if(posizione_adiacente!=-1)
               {
               if(verifica_adiacenza(triangoli[posizione_adiacente],ADB,l))
                   aggiorna_adiacenza(ABC,ADB,l,triangoli,i,posizione_adiacente);
                else if  (verifica_adiacenza(triangoli[posizione_adiacente],ADC,l))
                    aggiorna_adiacenza(ABC,ADC,l,triangoli,i,posizione_adiacente);
                }
               int posizione_adiacente2=trova_id(triangoli,CBD.adiacenti[i]); // trova la posizione del triangolo nella lista triangoli con id chiesto
               Lati l2;
               if(posizione_adiacente2!=-1)
               {
               if(verifica_adiacenza(triangoli[posizione_adiacente2],ADB,l2))
                   aggiorna_adiacenza(CBD,ADB,l2,triangoli,i,posizione_adiacente2);
                else if  (verifica_adiacenza(triangoli[posizione_adiacente2],ADC,l2))
                    aggiorna_adiacenza(CBD,ADC,l2,triangoli,i,posizione_adiacente2);
                }

            }

      //ABC =ADB;
      //CBD =ADC;
      // oppure
      triangoli.push_back(ADB);
      triangoli.push_back(ADC);
      removeElement( triangoli, ABC);
      removeElement( triangoli, CBD);
  }

 void ipotesi_int(Point punto,Triangle& t,vector<Triangle>&triangoli,vector<Lati>&lati,int& t_id,int& l_id)
 {

    vector<int>triangoli_da_verificare;
  ////////////////// POTREBBE ESSERE SIDES[0] SEMPRE
    int id_da_verificare;
    for(int i=0;i<3;i++)
    {
        if(t.adiacenti[i]!=-1)
        {
            int posizione_adiacente=trova_id(triangoli,t.adiacenti[i]);
            if(triangoli[posizione_adiacente].points[0]!=punto&&triangoli[posizione_adiacente].points[1]!=punto&&triangoli[posizione_adiacente].points[2]!=punto)
            triangoli_da_verificare.push_back(t.adiacenti[i]);
        }
    }
  /////////////////////////////
    while (triangoli_da_verificare.size()>0)
    {
        id_da_verificare=triangoli_da_verificare.back();
        int posizione_adiacente=trova_id(triangoli,id_da_verificare);

        if(posizione_adiacente>=0) // NON NECESSARIA
        {
        if(isInsideCircumcircle(triangoli[posizione_adiacente],punto)) // verifica l'ipotesi di dalaunay
        {
            Lati l;
            bool a=verifica_adiacenza(t,triangoli[posizione_adiacente],l);
            if (a)
            {

            // DEVO SVUOTARE LA LISTA???
            triangoli_da_verificare={};// svuoto la lista


            for(int i=0;i<3;i++)
            {
                if(triangoli[posizione_adiacente].adiacenti[i]!=-1)
                {
                    triangoli_da_verificare.push_back(triangoli[posizione_adiacente].adiacenti[i]);
                }
            }

            Triangle t1=t;
            Triangle t2=triangoli[posizione_adiacente];
            Flip(t1,t2,l,triangoli,lati,t_id,l_id);
            //removeElement( lati, l);
            }
        }
        }
        //triangoli_da_verificare.pop_back(); // LOOP
        removeElement_int(triangoli_da_verificare,id_da_verificare);

    }

 }



 template<typename T>
  void removeDuplicates(std::vector<T>& vec) {
      std::vector<T> uniqueElements;
      uniqueElements.reserve(vec.size()); // Ottimizza la capacità del vettore dei valori unici

      for (const auto& element : vec) {
          if (std::find(uniqueElements.begin(), uniqueElements.end(), element) == uniqueElements.end()) {
              uniqueElements.push_back(element);
          }
      }

      vec = uniqueElements;
  }

  void CreoFileOutput(string outputFilePath, vector<Lati> myList)
  {
      ofstream outputFile(outputFilePath); // Apriamo un nuovo file in modalità di scrittura

      if (outputFile.is_open())
      {
          for (const auto& element : myList)
          {
              Point p1 ;
              p1 = element.points[0];
              Point p2 ;
              p2 = element.points[1];

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

 /// CONVEX HULL LIBRARY

  bool UpperLine(const Point& p1,
                 const Point& p2,
                 const Point& p3)
  {
    double dx1 = p2.x - p1.x;
    double dy1 = p2.y - p1.y;
    double dx2 = p3.x - p1.x;
    double dy2 = p3.y - p1.y;

    if (dx1 * dy2 > dy1 * dx2)
      return true;
    else if (dx1 * dy2 < dy1 * dx2)
      return false;
    else
      return (dx1 * dx2 + dy1 * dy2 >= 0);
  }


    void MergeHulls(vector<Point>& sortedV, const unsigned int& cx)

        {
          // Define iterators for the left and right halves of the hull
          auto left = cx;
          auto right = cx + 1;

          // Find the lower tangent of the two hulls
          while (left > 0 && right < sortedV.size() - 1 && !UpperLine(sortedV[left - 1], sortedV[left], sortedV[right]))
          {
            if (sortedV[left - 1].y > sortedV[right + 1].y)
            {
              --left;
            }
            else
            {
              ++right;
            }
          }

          // Merge the two hulls using the lower tangent
          auto newHull = vector<Point>();
          newHull.reserve(sortedV.size());

          // Copy the left half of the hull to the new hull
          for (auto i = cx; i >= left; --i)
          {
            newHull.push_back(sortedV[i]);
          }

          // Copy the right half of the hull to the new hull
          for (auto i = cx + 1; i <= right; ++i)
          {
            newHull.push_back(sortedV[i]);
          }

          // Replace the sorted vector with the new hull
          sortedV.swap(newHull);
        }

    void MergeHull(vector<Point>& sortedV, const unsigned int& sx, const unsigned int& dx)
    {
        unsigned int cx;
        if(sx<dx)
        {
            cx=(sx+dx)/2;
            MergeHull( sortedV, sx, cx);
            MergeHull( sortedV, cx +1, dx);
            MergeHulls(sortedV,cx);

        }


    }


    vector<Point> ConvexHull(vector<Point>& points)
    {
        if(points.size() < 2)
          return points;
        else{
          SortLibrary::MergeSort(points, 0, int(points.size() - 1));

      // compute lower hull
      vector<Point> lowerHull;
      for(int i=0; i<int(points.size()); ++i)
      {
        while(lowerHull.size() >= 2 &&
              !UpperLine(lowerHull[lowerHull.size()-2],
                          lowerHull[lowerHull.size()-1],
                          points[i]))
          lowerHull.pop_back();
        lowerHull.push_back(points[i]);
      }

      // compute upper hull
      vector<Point> upperHull;
      for(int i=points.size()-1; i>=0; --i)
      {
        while(upperHull.size() >= 2 &&
              !UpperLine(upperHull[upperHull.size()-2],
                          upperHull[upperHull.size()-1],
                          points[i]))
          upperHull.pop_back();
        upperHull.push_back(points[i]);
      }

      // remove first and last point from upper hull
      upperHull.pop_back();
      upperHull.erase(upperHull.begin());

      // concatenate the two hulls
      vector<Point> hull(lowerHull);
      hull.insert(hull.end(), upperHull.begin(), upperHull.end());

      return hull;
    }
   }




     double area(Point& p1, Point& p2, Point& p3) {
            return abs(0.5 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y)));
        }

     vector<Point> findMaxTriangle(vector<Point>& points) {
     int A = 0, B = 1, C = 2;
     int n = points.size();
     int bA = A, bB = B, bC = C; // Il "miglior" tripletta di punti
     while (true) { // loop A
       while (true) { // loop B
         while (area(points[A], points[B], points[C]) <= area(points[A], points[B], points[((C+1)%n)])) { // loop C
           C = (C+1) % n;
         }
         if (area(points[A], points[B], points[C]) <= area(points[A], points[(B+1) % n], points[C])) {
           B = (B+1) % n;
           continue;
         } else {
           break;
         }
       }

       if (area(points[A], points[B], points[C]) > area(points[bA], points[bB], points[bC])) {
         bA = A; bB = B; bC = C;
       }

       A = (A+1) % n;
       B = (A+1) % n;
       C = (A+2) % n;
       if (A == 0) {
         break;
       }
     }
     vector<Point> result;
     result.push_back(points[bA]);
     result.push_back(points[bB]);
     result.push_back(points[bC]);
     return result;
     }


     void Flip_convex_hull(Triangle ABC, Triangle CBD, Lati& oldlato,vector<Triangle>& triangoli,vector<Lati>&lati,int& t_id,int& l_id)
     {

        // TROVO IL NUOVO LATO
        removeElement( lati, oldlato);

        Lati newlato;
        trova_nuovo_lato(ABC,CBD,oldlato,newlato,l_id);
        lati.push_back(newlato);

        // CREO I NUOVI TRIANGOLI
        Triangle ADB = Triangle(newlato.points[0], newlato.points[1], oldlato.points[0],t_id,l_id ); t_id++;l_id++;l_id++;l_id++;
        Triangle ADC = Triangle(newlato.points[0], newlato.points[1], oldlato.points[1],t_id,l_id ); t_id++;l_id++;l_id++;l_id++;


        // eredito vecchie adiacenze
        for(int i=0; i<3; i++) // per ogni triangolo adiacente ad ABC
             {
                 int posizione_adiacente=trova_id(triangoli,ABC.adiacenti[i]); // trova la posizione del triangolo nella lista triangoli con id chiesto
                 Lati l;
                 if(posizione_adiacente!=-1)
                 {
                 if(verifica_adiacenza(triangoli[posizione_adiacente],ADB,l))
                     aggiorna_adiacenza(ABC,ADB,l,triangoli,i,posizione_adiacente);
                  else if  (verifica_adiacenza(triangoli[posizione_adiacente],ADC,l))
                      aggiorna_adiacenza(ABC,ADC,l,triangoli,i,posizione_adiacente);
                  }
                 int posizione_adiacente2=trova_id(triangoli,CBD.adiacenti[i]); // trova la posizione del triangolo nella lista triangoli con id chiesto
                 Lati l2;
                 if(posizione_adiacente2!=-1)
                 {
                 if(verifica_adiacenza(triangoli[posizione_adiacente2],ADB,l2))
                     aggiorna_adiacenza(CBD,ADB,l2,triangoli,i,posizione_adiacente2);
                  else if  (verifica_adiacenza(triangoli[posizione_adiacente2],ADC,l2))
                      aggiorna_adiacenza(CBD,ADC,l2,triangoli,i,posizione_adiacente2);
                  }

              }

        //nuova adiacenza
        ADB.adiacenti[2]=ADC.id;
        ADC.adiacenti[0]=ADB.id;


        triangoli.push_back(ADB);
        triangoli.push_back(ADC);
        removeElement( triangoli, ABC);
        removeElement( triangoli, CBD);
      }


     void ipotesi_convex_hull(Point punto,Triangle& t,Lati lato_adiacente,vector<Triangle>&triangoli,vector<Lati>&lati,int& t_id,int& l_id)
     {
        vector<int>triangoli_da_verificare;

        int id_da_verificare;
        for(int i=0;i<3;i++)
        {
            if(t.adiacenti[i]!=-1)
            {
                triangoli_da_verificare.push_back(t.adiacenti[i]);
            }
        }

        while (triangoli_da_verificare.size()>0)
        {
            id_da_verificare=triangoli_da_verificare.back();
            int posizione_adiacente=trova_id(triangoli,id_da_verificare);

            if(posizione_adiacente>=0) // NON NECESSARIA
            {
            if(isInsideCircumcircle(triangoli[posizione_adiacente],punto)) // verifica l'ipotesi di dalaunay
            {
                Lati l;
                bool a=verifica_adiacenza(t,triangoli[posizione_adiacente],l);
                if (a)
                {

                // DEVO SVUOTARE LA LISTA???
                triangoli_da_verificare={};// svuoto la lista


                for(int i=0;i<3;i++)
                {
                    if(triangoli[posizione_adiacente].adiacenti[i]!=-1)
                    {
                        triangoli_da_verificare.push_back(triangoli[posizione_adiacente].adiacenti[i]);
                    }
                }

                Triangle t1=t;
                Triangle t2=triangoli[posizione_adiacente];
                Flip(t1,t2,l,triangoli,lati,t_id,l_id);
                //removeElement( lati, l);
                }
            }
            }
            //triangoli_da_verificare.pop_back(); // LOOP
            removeElement_int(triangoli_da_verificare,id_da_verificare);

        }

     }


     vector<Triangle> TriangoliConvexHull (vector<Point> punti,vector <Lati>& lati,int& t_id,int& l_id)
      {
           vector<Triangle> triangoli;
           //Triangle old;
           //vector<Point> ConvexHullPoints = ConvexHull(punti);
           int size=int(punti.size());
           for(int i=0;i<size;i++)
           {
               Lati l=Lati(punti[i],punti[(i+1)%size],l_id);l_id++;
               lati.push_back(l);
           }

           for(int i=1;i<size-1;i++){
               Triangle nuovoTriangolo=Triangle(punti[0],punti[i],punti[i+1],t_id,l_id);t_id++;l_id++;l_id++;l_id++;

               /////////////////////// int indice=trova_id(triangoli,old.adiacenti[i]);//////////////////////
               if(i==1)
               {

                   //Triangle old = nuovoTriangolo;
                   triangoli.push_back(nuovoTriangolo);
               }
               else
               {

               Lati l=Lati(punti[0],punti[i],l_id);l_id++;
               lati.push_back(l);
               //int indice=trova_id(triangoli, old.id);
               // nuovoTriangolo.adiacenti[0]=triangoli[indice].id;
               //triangoli[indice].adiacenti[2]=nuovoTriangolo.id;

               triangoli.push_back(nuovoTriangolo);
               int indice_nuovoT =int(triangoli.size()-1);
               for(int j=0;j<int(triangoli.size()-1);j++)
               {
                   if(verifica_adiacenza(triangoli[indice_nuovoT],triangoli[j],l))
                   {
                       if(isInsideCircumcircle(triangoli[j],triangoli[indice_nuovoT].points[2]))
                       {


                           Flip_convex_hull(nuovoTriangolo,triangoli[j],l,triangoli,lati,t_id,l_id);

                           // FORSE DEVO METTERE LE ADIACENZE
                           // corrispondenz alato adiacenza
                           //nuovoTriangolo.adiacenti[0]=triangoli[j].id;
                           //triangoli[j].adiacenti[2]=nuovoTriangolo.id;
                           ipotesi_int(triangoli[indice_nuovoT].points[2],triangoli[j],triangoli,lati,t_id,l_id);
                           break;

                       }
                       else
                       {
                           triangoli[indice_nuovoT].adiacenti[0]=triangoli[j].id;
                           triangoli[j].adiacenti[2]=nuovoTriangolo.id;
                       }
                       /*
                       nuovoTriangolo.adiacenti[0]=triangoli[j].id;
                       triangoli[j].adiacenti[2]=nuovoTriangolo.id;
                       triangoli.push_back(nuovoTriangolo);
                       ipotesi_int(nuovoTriangolo.points[2],nuovoTriangolo,triangoli,lati,t_id,l_id);
                       break;
                       */
                   }
               }
               //*/

               /*
               nuovoTriangolo.adiacenti[0]=triangoli[i-2].id; //posiz. 0 quello precedente
               triangoli[i-2].adiacenti[2]=nuovoTriangolo.id; //posiz. 1 quello successivo
               triangoli.push_back(nuovoTriangolo);
               ipotesi_int(nuovoTriangolo.points[2],nuovoTriangolo,triangoli,lati,t_id,l_id);
               */
               }

           }
           return triangoli;
      }

  }
