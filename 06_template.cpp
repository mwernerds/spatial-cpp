/*
(c) 2019 M. Werner - Part of the GIS++ tutorial 
- https://www.martinwerner.de/teaching/spatial-cpp
- https://github.com/mwernerds/spatial-cpp

Program: Template Specialization Example
Compile: g++ -Wall -std=c++17 -o 06_template 06_template.cpp
*/

#include<iostream>

namespace my{
// the generic one
template<typename T>
T mul2(T v)
{
	std::cout << "Using Multiplication Operator" << std::endl;
	return v*2;
}

template<>
int mul2(int v)
{
	std::cout << "Using Bit Shift" << std::endl;
	return v << 1;
}


}//my



int main(void)
{
	int i=42;
	float f = 4.2;
	std::cout << "mul2(i)" <<std::endl;
	std::cout	<< my::mul2(i) << std::endl;
	std::cout << "mul2(f)" <<std::endl;
	std::cout	<< my::mul2(f) << std::endl;
	return 0;

	
}