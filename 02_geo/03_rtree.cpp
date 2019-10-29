/*
(c) 2019 M. Werner - Part of the GIS++ tutorial 
- https://www.martinwerner.de/teaching/spatial-cpp
- https://github.com/mwernerds/spatial-cpp

Program: R-Tree
Compile: g++ -I $(BOOST_DIR) -Ofast -march=native  -Wall -std=c++11  -o 03_rtree 03_rtree.cpp
*/

#include<iostream>
#include<fstream>
#include <boost/geometry.hpp>
#include <boost/algorithm/string.hpp>
#include<chrono>

#include<set>

#include <boost/range/adaptor/indexed.hpp>
using  boost::adaptors::indexed;
#include <boost/range/adaptor/transformed.hpp>
using  boost::adaptors::transformed;
#include <boost/function_output_iterator.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<double, 2, bg::cs::cartesian> point;
typedef bg::model::box<point> box;
typedef bg::model::linestring<point> linestring;
typedef bg::model::polygon<point, false, false> polygon; // ccw, open polygon
typedef bg::model::multi_polygon<polygon> multi_polygon; // ccw, open polygon
typedef std::pair<box, size_t> value; // <- this is what the R-tree will hold


typedef bgi::rtree< value, bgi::rstar<16, 4> > rtree;

std::vector<std::pair<polygon, size_t>> dataset;


struct value_maker
{
    template<typename T>
    inline value operator()(T const& v) const
    {
	box b;
	bg::envelope(v.value().first,b);
        return value(b, v.index());
    }
};

std::ostream &operator<< (std::ostream &os, box &b)
{
    os << "(" << bg::get<0>(b.min_corner()) << ";" << bg::get<1>(b.min_corner()) << ")" << "-->"
	  << "(" << bg::get<0>(b.max_corner()) << ";" << bg::get<1>(b.max_corner()) << ")" ;
    return os;
}

point  operator + (const point &p, const point &b)
{
    return bg::make<point>(bg::get<0>(p)+bg::get<0>(b),bg::get<1>(p)+bg::get<1>(b));
    
}
point  operator - (const point &p, const point &b)
{
    return bg::make<point>(bg::get<0>(p)-bg::get<0>(b),bg::get<1>(p)-bg::get<1>(b));
    
}


point random_point_in_box(box b)
{
    double tau1 = static_cast<double> (std::rand()) / RAND_MAX;
    double tau2 = static_cast<double> (std::rand()) / RAND_MAX;
    return bg::make<point>(
	bg::get<0>(b.min_corner())+ tau1 * (bg::get<0>(b.max_corner())-bg::get<0>(b.min_corner())),
	bg::get<1>(b.min_corner())+ tau2 * (bg::get<1>(b.max_corner())-bg::get<1>(b.min_corner())));

}


int main(int argc, char **argv)
{
    std::srand(std::time(0)); //use current time as seed for random generator
    
// Load the OSM polygons and explode each multipolygon into polygons to be added to the index.
    box roi(point(0,0),point(0,0));
    { // scope for timing
    auto start = std::chrono::high_resolution_clock::now();
   
    std::ifstream ifs("washington_dc_osm_buildings.wkt");
    std::string line;

    
    while(std::getline(ifs, line))
    {
	// split at ";"
	std::vector<std::string> entries;
	boost::split(entries, line, [](char c){return c == ';';});
	size_t osm_id = boost::lexical_cast<size_t>(entries[0]);

	// remove "
	 entries[1].erase(remove_if(entries[1].begin(), entries[1].end(), [](const char& c) {
        return c=='"';   }), entries[1].end());
	multi_polygon mp;
	bg::read_wkt(entries[1],mp);
	for (auto &p: mp) // each building part!
	{
	    bg::correct(p);
	    dataset.push_back(std::make_pair(p,osm_id));
	    if (bg::get<0>(roi.min_corner()) == 0){ // bad hack
		bg::envelope(p,roi);
	    }else{
		box q;
		bg::envelope(p,q);
		bg::expand(roi,q);
	    }
	}
    }
    std::cout << "Dataset contains " << dataset.size() << " polygons" << std::endl;
    std::cout << "MBR of dataset: " << roi << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << " Load CSV in " << diff.count() << "seconds" << std::endl;
    } // loading scope
    // now load this into an R-tree: variant 1: sequential insert

    { // sequential r-tree population
    auto start = std::chrono::high_resolution_clock::now();
    rtree rt;
    
    for (const auto &d:dataset |indexed())
    {
	box b;
	bg::envelope(d.value().first, b); // create MBR
	rt.insert(value(b,d.index()));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << " Sequential R-Tree in " << diff.count() << "seconds" << std::endl;
    }
    rtree rt2; // just to have it later
    // variant 2: bulk-load
    { // bulk load scope
    auto start = std::chrono::high_resolution_clock::now();
    rt2 = rtree (dataset | indexed()
                       | transformed(value_maker()));
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end-start;
    std::cout << " Bulk-Load R-Tree in " << diff.count() << "seconds" << std::endl;
    } // bulk load scope
    
    // Let us now do a kNN (k = 10) query for a single random point
    std::vector<value> result;
    auto p = random_point_in_box(roi);
    // for reproducibilty:
    p = bg::make<point>(-76.8117, 38.812);
    std::cout << "Anchor: " << bg::wkt(p) << std::endl;
    rt2.query(bgi::nearest(p, 10), std::back_inserter(result));
    // two issues to be resolved: first, kNN is not ordered, second, r-tree knows bbox only
    std::sort(result.begin(), result.end(), [p](const value & a, const value & b){
	return bg::distance(dataset[a.second].first,p) < bg::distance(dataset[b.second].first,p );
    });
    
    for (const auto &v:result | indexed())
    {
	auto id = v.value().second;
	std::cout << v.index() << "\t" << dataset[id].second << "\t" << bg::distance (p, dataset[id].first) << std::endl;
    }

    // and again something for QGIS:
    // we will take a random point, then 10-nearest neighbors (BBOX), then double the distance for a range query.
    // with all these results, we will write a single CSV containing classified buildings.
    result.clear();
    point anchor = bg::make<point> (-76.99017,38.88970);
    std::cout << "Anchor: " << bg::wkt(anchor) << std::endl;
    rt2.query(bgi::nearest(anchor, 200), std::back_inserter(result));
    double radius = 0.03;

    box range_query_box;
    range_query_box.min_corner() = anchor - bg::make<point>(radius,radius);
    range_query_box.max_corner() = anchor + bg::make<point>(radius,radius);

    std::cout << "Range Query Box:" << range_query_box << std::endl;

    // function_output_iterator is a nice tool as well:
    std::set<size_t> knnids;
    std::ofstream ofs("range_knn.csv");
    ofs <<  std::setprecision(std::numeric_limits<double>::digits10);
    ofs << "wkt;role" << std::endl;
    // first write all kNN with their ID
    for (auto r:result)
    {
	const auto &item = dataset[r.second];
	ofs << bg::wkt(item.first) <<";" << 1 << std::endl;
	knnids.insert(r.second);
    }
        
    
    rt2.query(bgi::within(range_query_box), boost::make_function_output_iterator([&](value const& v)
    {
	const auto &item = dataset[v.second];
	if (knnids.find(v.second) == knnids.end()){
	   ofs << bg::wkt(item.first) << ";" << 2 << std::endl;
	}
    }
    ));
    
    return 0;
}
