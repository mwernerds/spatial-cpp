/*
(c) 2019 M. Werner - Part of the GIS++ tutorial 
- https://www.martinwerner.de/teaching/spatial-cpp
- https://github.com/mwernerds/spatial-cpp

Program: Points
Compile: g++ -I $(BOOST_DIR) -Wall -std=c++11  -o 00_points 00_points.cpp
*/


#include<iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

using namespace boost::geometry;

// declare some points. There are type templates in the library.

typedef model::d2::point_xy<double> point_a;

// but what if we have old c-style libraries around? The array case:

#include <boost/geometry/geometries/adapted/c_array.hpp>

BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian) //<-- this tags the type with a coordinate system! nice.

int main(int argc, char **argv)
{

    point_a p(42,11), q(43,12);
    std::cout << "The distance of P and Q is: " << distance(p,q) << std::endl;

    int p2[2]={42,11}, q2[2]={43,12};
    std::cout << "The distance of P2 and Q2 is: " << distance(p2,q2) << std::endl;

    // but what if we use a spheroid Earth model?
    
    
    return 0;
}
