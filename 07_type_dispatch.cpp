/*
(c) 2019 M. Werner - Part of the GIS++ tutorial 
- https://www.martinwerner.de/teaching/spatial-cpp
- https://github.com/mwernerds/spatial-cpp

Program: Template Specialization Example
Compile: g++ -Wall -std=c++17 -o 06_template 06_template.cpp
*/

#include<iostream>

namespace detail{
// the generic one, add a bool which will be specialized
template<typename T, bool b>
T mul2(T v, const std::integral_constant<bool,b> &)
{
	std::cout << "Using Multiplication Operator" << std::endl;
	return v*2;
}

template<typename T>
T mul2(T v, const std::true_type &)
{
	std::cout << "Using Bit Shift" << std::endl;
	return v << 1;
}
}//detail

template<typename T>
inline T mul2(T v)
{
	return detail::mul2(v,std::is_integral<T>());
}



int main(void)
{
	int i=42;
	float f = 4.2;
	std::cout << "mul2(i)" <<std::endl;
	std::cout	<< mul2(i) << std::endl;
	std::cout << "mul2(f)" <<std::endl;
	std::cout	<< mul2(f) << std::endl;
	std::cout << "char: " << mul2<char>(42) << std::endl;
	std::cout << "size_t: " << mul2<size_t>(42) << std::endl;
	std::cout << "long double: " << mul2<long double>(42) << std::endl;
	return 0;

	
}