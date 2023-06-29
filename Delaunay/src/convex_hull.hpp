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

namespace Delaunay
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


  bool UpperLine( const Point& p1,
                  const Point& p2,
                  const Point& p3);

  vector<Point> ConvexHull(vector<Point>& points);
  vector<Triangle> TriangoliConvexHull (vector<Point> punti,vector <Lati>& lati,int& t_id,int& l_id);
}

#endif // __CONVEXHULL_H

