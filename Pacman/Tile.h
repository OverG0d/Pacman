#pragma once
#ifndef TILE_H
#define TILE_H
#include "GameObject.h"
#include <string>
#include <iostream>

using namespace std;

class Tile : public GameObject
{

public:
	Tile();
	Tile(glm::vec2, GLboolean, GLboolean);
	void Description();
	bool isEqual(Tile*);
	int fScore();	
	bool operator==(Tile);
	GLboolean   IsSolid;
	glm::vec2 position;
	Tile* parent;
	int gScore;
	int hScore;
	GLboolean atBase;
	
private:
};
#endif