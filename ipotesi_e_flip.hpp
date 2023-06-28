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



namespace Delaunay
{
void trova_nuovo_lato(Triangle ABC, Triangle CBD, Lati oldlato,Lati & newlato,int& l_id);
void aggiorna_adiacenza(Triangle& old,Triangle& nuovo,vector<Triangle>& triangoli);
void Flip(Triangle& ABC, Triangle& CBD, Lati& oldlato,vector<Triangle>& triangoli,vector<Lati>&lati,int& t_id,int& l_id, Point p);
void ipotesi_int(Point punto,Triangle& t,vector<Triangle>&triangoli,vector<Lati>&lati,int& t_id,int& l_id);
}



#endif // __IPOTESIEFLIP_H
