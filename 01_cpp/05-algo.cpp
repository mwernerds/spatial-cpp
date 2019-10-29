/*
(c) 2019 M. Werner - Part of the GIS++ tutorial 
- https://www.martinwerner.de/teaching/spatial-cpp
- https://github.com/mwernerds/spatial-cpp

Program: Hello world in C
Compile: g++ -Wall -std=c++17  -o 05-algo 05-algo.cpp
*/

#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<unordered_map>

#include<algorithm>
#include<numeric>

#include<chrono>

#include <boost/range/adaptor/indexed.hpp>
#include <boost/assign.hpp>
#include <boost/assign/list_inserter.hpp> // for 'insert()'


// you will need this header for the C++17 parallel version, but it is not yet available everywhere.
//#include<execution>

using  boost::adaptors::indexed;
using namespace boost::assign;



template <typename T>
std::ostream &operator<< (std::ostream &os, const std::vector<T>  & data)
{
    os << "(@" <<  (void *) &data[0] << "): ";
    std::copy(data.begin(), data.end(), std::ostream_iterator<int>(os, " "));
    return os;
}


int main(int argc, char **argv)
{
    // some algorithm templates in use

    std::vector<int> v;
    v += 1,2,3,4,5,6,7;
    std::cout << v << std::endl;
    // let us randomize v
    std::random_shuffle(v.begin(), v.end());
    std::cout << v << std::endl;
    // sort
    std::sort(v.begin(), v.end());
    // in c++ 20, you can see a vector as a range and do
    // std::ranges::sort(v)
    std::cout << v << std::endl;
    // show only even entries
    std::random_shuffle(v.begin(), v.end());

    auto uneven = std::count_if(v.begin(), v.end(), [](int a) {return (a%2 == 1);});
    std::cout << "Found " << uneven << " uneven numbers." << std::endl;

    // Okay, but how does this help us? Let us search for a number in an ordered list in three ways
    // and do some timing on simple, classical, and C++ style solutions.

    std::vector<long> many(1024*1024);
    std::iota(many.begin(), many.end(),0);
//    std::cout << many << std::endl;

    {
    unsigned long sink;
    auto start = std::chrono::high_resolution_clock::now();
    auto it = many.begin();
    for (sink=0; it != many.end(); ++it){  sink += *it;};
    
    auto end = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double> diff = end-start;
     std::cout << "Summed up to " << sink << " in " << diff.count() << "seconds" << std::endl;
    }

    // faster (!) and easier to read
    { 
    auto start = std::chrono::high_resolution_clock::now();
    auto sink = std::accumulate(many.begin(), many.end(), 0ul); 
    auto end = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double> diff = end-start;
     std::cout << "Summed up to " << sink << " in " << diff.count() << "seconds" << std::endl;
    }

    // similar: let us search for the location of the value 12345
    
    { 
    auto start = std::chrono::high_resolution_clock::now();
    size_t i;
    for(i=0; i<many.size();i++)
    {
	if (many[i] == 123456)
	   break;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double> diff = end-start;
     std::cout << "Found it at " << i << " in " << diff.count() << "seconds" << std::endl;
    }

    
    // algorithm version (using sorted property, that is binary search)
    { 
    auto start = std::chrono::high_resolution_clock::now();
    auto it = std::lower_bound(many.begin(), many.end(),123456);
    auto end = std::chrono::high_resolution_clock::now();
     std::chrono::duration<double> diff = end-start;
     std::cout << "Found it at " << std::distance(many.begin(),it) << " in " << diff.count() << "seconds" << std::endl;
    }

    // in c++17: MapReduce?!
    // Map:
    std::transform(many.begin(), many.end(), many.begin(), [](int v) -> int {return v*2;});

    std::cout << "transform: ";
    std::copy_n(many.begin(), 10 , std::ostream_iterator<int>(std::cout, " "));
    std::cout << "..." << std::endl;

    // accumulate (e.g., sequential)
     { 
	auto start = std::chrono::high_resolution_clock::now();
	auto sink = std::accumulate(many.begin(), many.end(), 0ul); 
	auto end = std::chrono::high_resolution_clock::now();
	 std::chrono::duration<double> diff = end-start;
	 std::cout << "Summed up to " << sink << " in " << diff.count() << "seconds" << std::endl;
    }

    // reduce (e.g. parallel, accumulate out of order, c++17, specified execution strategy)
/* 
this is C++17 and not yet widely available. But how cool is this: just tell it to be parallel and it does ;-)

    { 
	auto start = std::chrono::high_resolution_clock::now();
	auto sink = std::reduce(std::execution::par, many.begin(), many.end(), 0ul); 
	auto end = std::chrono::high_resolution_clock::now();
	 std::chrono::duration<double> diff = end-start;
	 std::cout << "Summed up to " << sink << " in " << diff.count() << "seconds" << std::endl;
    }



// and a complete example of the simple algorithm std::sort in C++17:

using namespace std;
vector<int> v = ...

// standard sequential sort
sort(v.begin(), v.end());

// explicitly sequential sort
sort(sequential, v.begin(), v.end());

// permitting parallel execution
sort(par, v.begin(), v.end());

// permitting vectorization as well
sort(par_vec, v.begin(), v.end());
*/  
    
    return 0;
}
