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



namespace Delaunay{

bool dentro ( Point a, Point b, Point c);
bool isInsideCircumcircle(Triangle t, Point q);
bool ispointin(Triangle t, Point p);
int trova_id(vector<Triangle> triangoli,int id);
void triangoli_int(Triangle t, Point p,vector <Lati> &lati,vector<Triangle> &triangoli,int& t_id,int& l_id);
bool Posizione(Point p,vector<Triangle> triangoli,int& id );
bool verifica_adiacenza(Triangle t1, Triangle t2,Lati &l);
int trova_posizione(vector<Triangle> vec,Triangle t);
template <typename T>
void removeElement(vector<T>& vec, T val) // rimuove l'elemento val dalla lista vec
{
    for (auto it=vec.begin();it!=vec.end();++it)
    {
        if (*it==val)
        {
            vec.erase(it);
            break;
        }

    }
}
}

#endif //__FUNZIONITRIANGOLI_H
