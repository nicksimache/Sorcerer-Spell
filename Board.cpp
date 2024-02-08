#include <string>
#include <vector>
#include <cstdlib> // for srand, rand
#include <ctime>   // for time
#include <iostream>

#include "LetterTile.h"
#include "Board.h"


Board::Board() : xDim(0), yDim(0) {};
Board::Board(int xDim, int yDim) : xDim(xDim), yDim(yDim), B(xDim, std::vector<LetterTile>(yDim)) {
	generate();
};


void Board::generate() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = 0; i < xDim; i++) {
		for (int j = 0; j < yDim; j++) {
			int random_number = std::rand() % (90 - 65 + 1) + 65;
			std::string X(1, char(random_number));

			B[i][j] = LetterTile(X);
		}
	}
};

std::string Board::toString(){
	std::string output = "";
	for (int i = 0; i < xDim; i++) {
		for (int j = 0; j < yDim; j++) {
			output += B[i][j].toString() + " ";
		}
		output += "\n";
	}
	return output;
}






