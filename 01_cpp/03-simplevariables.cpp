/*
(c) 2019 M. Werner - Part of the GIS++ tutorial 
- https://www.martinwerner.de/teaching/spatial-cpp
- https://github.com/mwernerds/spatial-cpp

Program: Hello world in C
Compile: g++ -Wall -o 03-simplevariables 03-simplevariables.cpp
*/

#include<iostream>
#include<cstring>

int main(int argc, char **argv)
{
    // some simple things with basic type int
    int i=2;
    int j;
    j = 5;
    auto k = j+i;
    std::cout << "k=" << k << std::endl;
    // some type casting
    std::cout << "Integer Division: " << i / j << std::endl;
    std::cout << "Floating Point (C version):" << ((double) i ) / j << std::endl;
    std::cout << "Floating Point (modern):"    << static_cast<double>(i) / j << std::endl;
    
    // some control structures
    if(i < 42)
    {
	std::cout << "i is smaller than 42" << std::endl;
    }

    // inline if
    std::cout << "The number i is" << ((i % 2) == 1?"uneven":"even") << std::endl;;

    // for
    for (size_t i=0; i< 42; i++)
    {
	std::cout << ".";
	std::cout.flush();
    }
    std::cout << std::endl;

    // while
    while (i != 42)
    {
	std::cout  << i++ << "->";
    }
    std::cout << 42 << std::endl;

    // strings (classical, C)

    char s1[1024];
    char s2[1024];
    snprintf(s1,1024,"Left String");
    snprintf(s2,1024,"Right String");

    char buf[2048];
    strncpy(buf, s1, 2048);
    strncat(buf, s2,2048);
    std::cout << buf << std::endl;

    // typical old bug-prone char processing
    char *p = &buf[0];
    while (*p != '\0')
      if (*p++ == 'S')
         std::cout << "Found an S at " << static_cast<void *>(p) << std::endl;

    // modern C++ strings
    std::string s ("Left String");
    s += "Right String";
    std::cout << s << std::endl;   
    
    
    return 0;
}
