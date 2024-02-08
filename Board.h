#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "LetterTile.h"

class Board {
public:

	int xDim;
	int yDim;

	std::vector<std::vector<LetterTile>> B;
	
	Board();
	Board(int xDim, int yDim);

	std::string toString();

	void generate();


};
#endif