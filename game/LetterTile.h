#pragma once
#ifndef LETTERTILE_H
#define LETTERTILE_H

#include <string>
#include <map>


class LetterTile {
public:
	std::string letter;
	int point;


	static std::map<std::string, int> letterScores;


	LetterTile();
	LetterTile(std::string s);

	std::string toString();


};




#endif