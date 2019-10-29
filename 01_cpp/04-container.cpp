/*
(c) 2019 M. Werner - Part of the GIS++ tutorial 
- https://www.martinwerner.de/teaching/spatial-cpp
- https://github.com/mwernerds/spatial-cpp

Program: Hello world in C
Compile: g++ -Wall  -o 04-container 04-container.cpp
*/

#include<iostream>
#include<vector>
#include<list>
#include<map>
#include<unordered_map>

// indexing support:
#include <boost/range/adaptor/indexed.hpp>
using  boost::adaptors::indexed;

#include <boost/assign.hpp>
using  namespace boost::assign;

#include <boost/assign/list_inserter.hpp> // for 'insert()'


int main(int argc, char **argv)
{
    // some simple things with basic type int
    std::vector<int> v;
    v.push_back(42);
    v.push_back(11);
    // classical
    for (size_t i=0; i< v.size(); i++)
    {
	std::cout << "Item " << i << ": " << v[i] << std::endl;
    }
    
    // semi-classical
    std::vector<int>::const_iterator it = v.cbegin();
    for (;it != v.cend(); ++it)
    {
	std::cout << "Item: " << *it << std::endl;
    }
    // modern
    for(const auto elem: v)
    {
	std::cout << "Item (modern): " << elem << std::endl;
    }
    // and how do I get indices? Boost is there for your

    for (const auto elem: v | indexed())
    {
	std::cout << "Item " << elem.index() << " has value " << elem.value() << std::endl;
    }

    // okay, but there is more readability possible:

    v += 1,2,3,4,5,6,7;
    for (const auto elem: v | indexed())
    {
	std::cout << "Item " << elem.index() << " has value " << elem.value() << std::endl;
    }
    // brace initialization
    std::vector<int> v2 {1,3,5,7};
    for (auto value:v2)
      std::cout << value << std::endl;

    // contiguity property, using pointers on vectors
    int *p = v2.data();

    std::cout << *p++ << std::endl;
    std::cout << *p++ << std::endl;
    std::cout << *p++ << std::endl;
    std::cout << *p++ << std::endl;
    // lists

    std::list<double> l1;
    l1.push_back(42.11);
    l1.push_front(13.2);


    // our first generic algorithm to use: copy
    
    std::copy(l1.begin(), l1.end(),  std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;


    // and a map
    std::map<std::string, int> m{ {"key1",35}, {"key2",46}};
    for (const auto i: m)
    {
	// i is now a std::pair<string,int>
	std::cout << i.first << "=>" << i.second << std::endl;
    }


    // and some more boost fun (boost::assign brings insert)

    
    std::map<std::string,int> months;  
    insert( months )
        ( "january",   31 )( "february", 28 )
        ( "march",     31 )( "april",    30 )
        ( "may",       31 )( "june",     30 )
        ( "july",      31 )( "august",   31 )
        ( "september", 30 )( "october",  31 )
        ( "november",  30 )( "december", 31 );

    // and more nice things: tie

    for (auto item : months)
    {
	std::string month; int days;        //  item
	std::tie(month, days) = item;
       std::cout << "Month " << month << " has " << days << " days." << std::endl;
    }

    // and now an unordered map (hashed, it is not orderpreserving!)
    std::unordered_map<std::string, int> months2;
    insert(months2)( "january",   31 )( "february", 28 )
        ( "march",     31 )( "april",    30 )
        ( "may",       31 )( "june",     30 )
        ( "july",      31 )( "august",   31 )
        ( "september", 30 )( "october",  31 )
        ( "november",  30 )( "december", 31 );
    
    std::cout << "===" << std::endl;
    for (auto item : months)
    {
	std::string month; int days;        //  item
	std::tie(month, days) = item;
       std::cout << "Month " << month << " has " << days << " days." << std::endl;
    }    
    return 0;
}
