/*
(c) 2019 M. Werner - Part of the GIS++ tutorial 
- https://www.martinwerner.de/teaching/spatial-cpp
- https://github.com/mwernerds/spatial-cpp

Program: Points
Compile: g++ -I $(BOOST_DIR) -Wall -std=c++11  -o 02_simplefeatures 02_simplefeatures.cpp
*/

#include<iostream>
#include<fstream>
#include <boost/geometry.hpp>

namespace bg = boost::geometry;

typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef bg::model::linestring<point> linestring;
typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon

// Note: the higher order objects usually have all features of a random_access container (or actually are some)

int main(int argc, char **argv)
{
    // let us make a nice triangle
    point a(0,0),b(1,1),c(2,0);
    polygon triangle;
    bg::append(bg::exterior_ring(triangle),a);
    bg::append(bg::exterior_ring(triangle),b);
    bg::append(bg::exterior_ring(triangle),c);
    std::cout << bg::wkt(a) << std::endl;
    std::cout << bg::wkt(b) << std::endl;
    std::cout << bg::wkt(c) << std::endl;
    std::cout << bg::wkt(triangle) << std::endl;

    // point in polygon (within relation)
    std::cout << std::boolalpha; // we want all bools to be written as true/false

    bg::correct(triangle); // make it CCW!
    std::cout << "Corrected Geometry: " << bg::wkt(triangle) << std::endl;
    std::cout << "Within: " << bg::within (bg::make<point>(1.0,0.5),triangle) << std::endl;

    // Let us now create our first file for QGIS:
    //
    // Algorithm in short:
    // 1) take two random (integer) triangles in [0,0]-[10,10].
    // 2) compute most important relations
    // 3) write a CSV file using WKT containing the two polygons for inspection

    std::srand(std::time(0)); //use current time as seed for random generator

    auto random_point = []()->point {return bg::make<point>(static_cast<int> (std::rand()%10),static_cast<int> (std::rand()%10));};

    //std::cout << bg::wkt(random_point()) << std::endl;
    polygon A,B;
    for (size_t i=0; i<3; i++) bg::append(bg::exterior_ring(A),random_point());
    for (size_t i=0; i<3; i++) bg::append(bg::exterior_ring(B),random_point());

    // remove these lines to see catastrophic results including negative area!
    bg::correct(A);
    bg::correct(B);

    std::cout << bg::wkt(A) << std::endl;
    std::cout << bg::wkt(B) << std::endl;

    // DE-9IM Matrix
    bg::de9im::matrix matrix = boost::geometry::relation(A,B);
    std::string code = matrix.str();
    std::cout << "relation: " << code << std::endl;
    // generic relate operation:
   
    bg::de9im::mask mask("T*F**F***"); // within
    auto p = random_point();
    bool check = bg::relate(p, A, mask);
    std::cout << "A random point " << bg::wkt(p) << " related: " << check << std::endl;

    // some algorithms relations:

    std::cout << "area: " << bg::area(A) << std::endl;
    std::cout << "covered_by: " <<  bg::covered_by (A,B) << std::endl;
    std::cout << "disjoint: " <<  bg::disjoint(A,B) << std::endl;
    std::cout << "equals: " <<  bg::equals(A,B) << std::endl;
    std::cout << "intersects: " <<  bg::intersects   (A,B) << std::endl;
    std::cout << "overlaps: " <<  bg::overlaps (A,B) << std::endl;
    std::cout << "touches: " <<  bg::touches (A,B) << std::endl;
    std::cout << "within: " <<  bg::within (A,B) << std::endl;

   
    // write CSV
    {
    std::ofstream ofs("geometry.csv");
    ofs << "wkt" << std::endl;
    ofs << bg::wkt(A)<< std::endl;
    ofs << bg::wkt(B) << std::endl;
    ofs.close();
    }


    
    // and now let us test within with random floating points
    {
    std::ofstream ofs("points.csv");
    ofs << "wkt; within" << std::endl;
    for (size_t i=0; i < 500; i++)
    {
	auto p = bg::make<point>( static_cast<double>(std::rand())/RAND_MAX*10.0,static_cast<double>(std::rand())/RAND_MAX*10.0);
	auto rel1 = bg::within(p,A);
	auto rel2 = bg::within(p,B);
	int score = (rel1 << 1) + rel2;
	ofs << bg::wkt(p) << ";" << score << std::endl;
    }
    }
    
    
    return 0;
}
