#include <string>
#include <vector>
#include <cstdlib> // for srand, rand
#include <ctime>   // for time
#include <iostream>

#include "LetterTile.h"
#include "Board.h"

Board::Board() : xDim(0), yDim(0) {};
Board::Board(int xDim, int yDim) : xDim(xDim), yDim(yDim), B(xDim, std::vector<LetterTile>(yDim)), charAssociate{
	{0, {65, 69, 73, 79, 85}},
	{1, {76, 78, 82, 83, 84}},
	{2, {68, 71}},
	{3, {66, 67, 77, 80}},
	{4, {70, 72, 86, 87, 89}},
	{5, {75}},
	{6, {74, 88}},
	{7, {81, 90}}
	//65, 69, 73, 79, 85, 76,78, 82, 83, 84 -1
	// 68, 71 -2
	// 66,67,77,80	-3
	//70,72,86,87,89 -4
	// 75 - 5
	//74, 88 - 8
	//81, 90 - 10
} {
	px = { 400, 300, 100, 100, 25, 25, 25, 25 };

	generate();
};

void Board::generate() {
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = 0; i < xDim; i++) {
		for (int j = 0; j < yDim; j++) {
			int random_number = std::rand() % 1000 + 1;
			int x = 0;
			for (int k = 0; k < px.size(); k++) {
				x += px[k];
				if (random_number <= x) {
					x = k;
					break;
				}
			}
			int rand_key = std::rand() % (charAssociate[x].size());
			char selected_letter = charAssociate[x][rand_key];
			std::string letter(1, selected_letter);
			B[i][j] = LetterTile(letter);

		}
	}
};

std::string Board::toString() {
	std::string output = "\n";
	for (int i = 0; i < xDim; i++) {
		for (int j = 0; j < yDim; j++) {
			output += B[i][j].toString() + " ";
		}
		output += "\n";
	}
	return output;
}

LetterTile Board::getTile(int i, int j)
{
	return B[i][j];
}

bool Board::isTileChosen(int i, int j)
{
	return B[i][j].isLetterChosen;
}




