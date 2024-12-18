#include <iostream>

void foo()
{ 
	unsigned int x0 = 5, y0 = 5; 
	std::cout << "(unsigned int) 5 + (unsigned int) 5 = " << x0 + y0 << "\n";
	std::cout << "(unsigned int) 5 - (unsigned int) 5 = " << x0 - y0 << "\n";
	std::cout << "(unsigned int) 5 / (unsigned int) 5 = " << x0 / y0 << "\n";
	int x1 = 4, y1 = 4; 
	std::cout << "(int) 4 + (int) 4 = " << x1 + y1 << "\n";
	std::cout << "(int) 4 - (int) 4 = " << x1 - y1 << "\n";
	std::cout << "(int) 4 / (int) 4 = " << x1 / y1 << "\n";
	double x2 = 7.4, y2 = 7.4; 
	std::cout << "(double) 7.4 + (double) 7.4 = " << x2 + y2 << "\n";
	std::cout << "(double) 7.4 - (double) 7.4 = " << x2 - y2 << "\n";
	std::cout << "(double) 7.4 / (double) 7.4 = " << x2 / y2 << "\n";
}