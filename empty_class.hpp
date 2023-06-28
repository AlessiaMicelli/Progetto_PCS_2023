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



namespace Delaunay
{

class Point{  // definisco la classe Point che ha come attributi la coordinata x e y e il suo identificatore
public:
 double x;
 double y;
 int id;
Point(double x, double y) : x(x), y(y), id(-1) {}  // costruttore che mi chiede solo le coordinate
Point(double x, double y,int id) : x(x), y(y), id(id) {} //costruttore classico
Point() : x(0), y(0), id(0) {} //costruttore vuoto

//  operatori per operazioni sui punti
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


 static double dist(Point& p1,Point&p2) // funzione che calcola la distanza tra due punti, funzione che definisco in una classe ma è valida anche all'esterno della classe
 {
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
 }

};


class Lati
{
public:
    Point points[2];  //array di due punti
    int id;

    Lati(){} // costruttore vuoto
    Lati(Point p1,Point p2) // costruttore dati i due punti del lato
    {
        points[0] = p1;
        points[1] = p2;
        id = -1;
    }

    Lati(Point p1,Point p2,int i) // costruttore dati due punti del lato e il suop identificatore
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



class Triangle
{
  public:
 // attributi
    int id;
    Point  points[3];  // array dei punti di cui è formato ( va bene array perchè ho una dimensione fissa)
    Lati  sides[3];  // array con i lati di cui è formato
    int adiacenti[3] = {-1,-1,-1};  // id dei triangoli a cui è adiacente

 Triangle(Point p1,Point p2,Point p3) // costruttore dati tre punti, me li ordina già in automatico
 {
    points[0] = p1;
    points[1] = p2;
    points[2] = p3;
    ordinapunti();    // metodo che serve a ordinare i punti in senso antiorario
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
    Point AB = points[1] - points[0];
    Point AC = points[2] - points[0];
    double z = AB.x*AC.y - AB.y*AC.x;
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

