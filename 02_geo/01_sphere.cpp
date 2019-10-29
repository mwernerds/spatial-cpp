/*
(c) 2019 M. Werner - Part of the GIS++ tutorial 
- https://www.martinwerner.de/teaching/spatial-cpp
- https://github.com/mwernerds/spatial-cpp

Program: Points
Compile: g++ -I $(BOOST_DIR) -Wall -std=c++11  -o 01_sphere 01_sphere.cpp
*/


#include<iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>

using namespace boost::geometry;


typedef boost::geometry::model::point
    <
        double, 2, boost::geometry::cs::spherical_equatorial<boost::geometry::degree>
    > spherical_point;


typedef boost::geometry::model::point
    <
        double, 3, boost::geometry::cs::cartesian
    > cartesian_point;


typedef boost::geometry::model::point
    <
        double, 2, boost::geometry::cs::geographic<boost::geometry::degree>
    > geographic_point;

    
spherical_point amsterdam(4.90, 52.37);
spherical_point paris(2.35, 48.86);

double const earth_radius = 6371; // now km


std::ostream &operator<< (std::ostream &os, const cartesian_point &p)
{
    os << "(" << get<0>(p) << ";" << get<1>(p) << ";" << get<2>(p)<< ")";
    return os;
}
std::ostream &operator<< (std::ostream &os, const geographic_point &p)
{
    os << "(" << get<0>(p) << ";" << get<1>(p) << ")";
    return os;
}




int main(int argc, char **argv)
{
    std::cout << "Distance in miles: " << distance(amsterdam, paris) * earth_radius << std::endl;
    cartesian_point paris3d, amsterdam3d;
    transform(paris, paris3d);
    transform(amsterdam, amsterdam3d);
    
    std::cout << "Paris 3D (on unit sphere)" << paris3d << std::endl;
    std::cout << "Amsterdam 3D (on unit sphere)" << amsterdam3d << std::endl;
    std::cout << "Distance (drill hole): " << distance(paris3d,amsterdam3d)*earth_radius << " miles" << std::endl;

    // Let us test some intuitive things like north to south pole

    // transformer: a functional trick
    auto tf = [](const spherical_point &p)->cartesian_point { cartesian_point ret; transform(p,ret); return ret;};
    
    std::cout << "North Pole: " << tf(spherical_point(0,90)) << std::endl;
    std::cout << "South Pole: " << tf(spherical_point(0,-90)) << std::endl;
    std::cout << "Distance: " << distance(tf(spherical_point(0,90)), tf(spherical_point(0,-90)))*earth_radius << std::endl;


    // an "external" cast without recalculating (which is not supported!)
    auto cast = [](const spherical_point &p) {return geographic_point(get<0>(p),get<1>(p));};
    std::cout << "Amsterdam (geo)" << cast(amsterdam) << std::endl;
    std::cout << "Paris (geo)" << cast(paris) << std::endl;
    // Geodesic distance
    std::cout << distance(cast(amsterdam),cast(paris)) << std::endl;

    return 0;
}
