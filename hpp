#ifndef __EMPTY_H
#define __EMPTY_H

#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
using namespace std ;


namespace dalaunay {

class Lati; // la chiamo cosi Point la legge

class Point{
public:
 double x;
 double y;
 std::vector<Lati *  > edges={}; // vettore di puntatori a lati


Point(double x, double y) : x(x), y(y) {} //costruttore
Point() : x(0), y(0) {} //costruttore vuoto

//  possono essere utili
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

 void Distanzamax(vector<Point>points,Point& p1,Point& p2,Point& p3);


 static double dist(Point& p1,Point&p2) // funzione che calcola la distanza tra due punti
 {
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
 }

 Lati * isconnect(Point*p); // funzione che mi dice quale lato collega il punto a p

};



class Lati{
public:
    Point * points[2]; // attributo
    Lati(){} // costruttore vuoto
    Lati(Point* p1,Point* p2) // costruttore
    {
        points[0]=p1;
        points[1]=p2;
    }

};



class Triangle{
  public:
 // attributi
 Point * points[3];
 Lati * sides[3];


 Triangle(Point *p1,Point *p2,Point *p3) // costruttore
 {
    points[0]=p1;
    points[1]=p2;
    points[2]=p3;
    ordinapunti();

 }
 Triangle(){} // costruttore vuoto

void ordinapunti () // restituisce il triangolo ordinato in senso antiorario
 {
    Point AB=*points[1]-*points[0];
    Point AC=*points[2]-*points[0];
    double z =AB.x*AC.y-AB.y*AC.x;
    if (z<0)
    {
        Point * tmp= points[2];
        points[2]=points[1];
        points[1]=tmp;
    }

 }





 bool ispointin( Point &p) // passo il puntatore e quindi potrei modificare l'originale, più veloce la chiamata
 {
    // [2] DA FARE --> vedere suggerimento 5
    Triangle t=*this;
    if(!t.isInsideCircumcircle(p)) //cancello &t come inpunt e chiamo t.
        return false;
    else
    {

        // trovo i due punti più vicini a b
        Point * a= points[0];
        Point * b= points[1];
        Point * c= points[2];


        if(dentro( *a, * b,  p)&& dentro( *b, * c,  p)&& dentro( *c, * a,  p))
        {
            return true;
        }
        else
            return false;
    }


 }


//bool isInsideCircumcircle(  Point& q); //const, elimino da input Triangle* t,

 bool isInsideCircumcircle( Point& q) // elimino t da input
 {
     Triangle t=*this;
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
     // dovrebbe essere giusta


     return det > 0.0;

 }


bool dentro ( Point a, Point b, Point c) // restituisce il triangolo ordinato in senso antiorario
 {
    Point AB=a-b;
    Point AC=a-c;
    double z =AB.x*AC.y-AB.y*AC.x;
    if (z<0)
        return false;
    else
        return true;
 }




 inline bool operator==( const Triangle t2) const
 {
    return (points[0]==t2.points[0]&& points[1]==t2.points[1]&& points[2]==t2.points[2]);
 }



};
void Distanzamax(std::vector<Point >points,Point& p1,Point& p2,Point& p3);

void Flip(Triangle ABC, Triangle CBD, Triangle& ABD, Triangle& ACD);

double Merge(vector<double>& distanze,
           const unsigned int& sx,
           const unsigned int& cx,
           const unsigned int& dx);



double MergeSort(vector<double>& distanze,
               const unsigned int& sx,
               const unsigned int& dx);




void Delaunay();


}


#endif // PROG_H






