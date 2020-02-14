#include "Tile.h"

Tile::Tile()
{

}

Tile::Tile(glm::vec2 pos, GLboolean isSolid, GLboolean atBase) {
	this->position = pos;
	this->gScore = 0;
	this->hScore = 0;
	this->IsSolid = isSolid;
	this->atBase = atBase;
	this->parent = NULL;
}

void Tile::Description()
{
	cout << "PositionX:" << this->position.x << "PositionY:" << this->position.y << "GScore:" << this->gScore << "HScore:" << this->hScore << "FScore" << this->fScore() << endl;
}

bool Tile::isEqual(Tile* other)
{
	return this->position == other->position;
}

int Tile::fScore()
{
	return this->gScore + this->hScore;
}

bool Tile::operator==(Tile other)
{
	return this->position == other.position;
}