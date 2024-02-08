#include <string>
#include <iostream>

#include "Board.h"

int main(){

	Board B(7, 7);
	std::cout << B.toString();

	return 0;

};