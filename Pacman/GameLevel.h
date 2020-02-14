#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glew.h>
#include <glm/glm.hpp>

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Resource_Manager.h"
#include "Tile.h"


/// GameLevel holds all Tiles as part of a Breakout level and 
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:



	// Level state
	std::vector<GameObject> normBricks;
	std::vector<GameObject> verBricks;
	std::vector<GameObject> horBricks;
	std::vector<GameObject> TopLeftBricks;
	std::vector<GameObject> TopRightBricks;
	std::vector<GameObject> BottomLeftBricks;
	std::vector<GameObject> BottomRightBricks;
	std::vector<GameObject> Bricks_1;
	std::vector<GameObject> Bricks_2;
	std::vector<GameObject> Bricks_3;
	std::vector<GameObject> emptyBricks;
	std::vector<GameObject> bannedTiles;
	std::vector<GameObject> pellets;


	//World Tiles
	std::vector<Tile> tempTiles;


	std::vector<std::vector<Tile>> tiles;
	std::vector<std::vector<Tile>> worldTiles;
	// Constructor/Destructor
	std::vector<std::vector<Tile>> fillWorldTiles();

	std::vector<Tile> walkableAdjacentTilesCoordForTileCoord(Tile, std::vector<std::vector<Tile>>);

	// Constructor
	GameLevel() { }
	// Loads level from file
	void      Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	// Render level
	void      Draw(SpriteRenderer &renderer);
	// Check if the level is completed (all non-solid tiles are destroyed)
	GLboolean IsCompleted();
private:
	// Initialize level from tile data
	void      init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);

};

#endif
