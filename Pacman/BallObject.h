#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glew.h>
#include <glm/glm.hpp>

#include "Texture.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Tile.h"
#include <vector>
#include <iostream>

using namespace std;
// BallObject holds the state of the Ball object inheriting
// relevant state data from GameObject. Contains some extra
// functionality specific to Breakout's ball object that
// were too specific for within GameObject alone.
class BallObject : public GameObject
{
public:
	// Ball state	
	// Constructor(s)
	BallObject();
	BallObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite);
	void MoveToward(glm::vec2, glm::vec2, Tile, std::vector<std::vector<Tile>>);
	glm::vec2 MoveTowards(glm::vec2, glm::vec2, float);
	void Dealloc();
    //A* Algorithm Methods
	void InsertInOpenSteps(Tile);
	int  ComputeHScoreFromCoord(glm::vec2, glm::vec2);
	int  CostToMoveFromStep(Tile, Tile);
	bool pathFound;
	std::vector<Tile> shortestPath;
	Tile end;
	
private:
	std::vector<Tile> openList;
	std::vector<Tile> closedList;
	float speed;
};
	


#endif


// Moves the ball, keeping it constrained within the window bounds (except bottom edge); returns new position
	//glm::vec2 Move(GLfloat dt, GLuint window_width);
	// Resets the ball to original state with given position and velocity
	//void      Reset(glm::vec2 position, glm::vec2 velocity);
