#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>
#include "LetterTile.h"

class Board {
public:

	int xDim;
	int yDim;

	std::vector<int> px;
	std::map<int, std::vector<int>> charAssociate;
	std::vector<std::vector<LetterTile>> B;


	Board();
	Board(int xDim, int yDim);

	std::string toString();

	void generate();
	LetterTile getTile(int i, int j);

};
#endif