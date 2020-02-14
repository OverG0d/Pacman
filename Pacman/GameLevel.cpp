#include "GameLevel.h"

#include <fstream>
#include <sstream>


void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
	// Clear old data
   	this->normBricks.clear();
	this->horBricks.clear();
	this->verBricks.clear();
	this->TopRightBricks.clear();
	this->TopLeftBricks.clear();
	this->BottomLeftBricks.clear();
	this->BottomRightBricks.clear();
	this->Bricks_1.clear();
	this->Bricks_2.clear();
	this->Bricks_3.clear();
	this->emptyBricks.clear();
	this->bannedTiles.clear();
	this->pellets.clear();
	
	// Load from file
	GLuint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<GLuint>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line)) // Read each line from level file
		{
			std::istringstream sstream(line);
			std::vector<GLuint> row;
			while (sstream >> tileCode) // Read each word seperated by spaces
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
	for (GameObject &tile : this->normBricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->horBricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->verBricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->TopRightBricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->TopLeftBricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->BottomLeftBricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->BottomRightBricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->Bricks_1)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->Bricks_2)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->Bricks_3)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->emptyBricks)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->bannedTiles)
		if (!tile.Destroyed)
			tile.Draw(renderer);
	for (GameObject &tile : this->pellets)
		if (!tile.Destroyed)
			tile.Draw(renderer);

}

/*GLboolean GameLevel::IsCompleted()
{
	for (GameObject &tile : this->verBricks)
		if (!tile.IsSolid && !tile.Destroyed)
			return GL_FALSE;
	return GL_TRUE;
}*/

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
	// Calculate dimensions
	GLuint height = tileData.size();
	GLuint width = tileData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
	GLfloat unit_width = levelWidth / static_cast<GLfloat>(width), unit_height = levelHeight / heightaa;
	// Initialize level tiles based on tileData		
	for (GLuint y = 0; y < height; ++y)
	{
		for (GLuint x = 0; x < width; ++x)
		{

			if (tileData[y][x] == 0)
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(50, 50);
				GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(0.37f, 0.18f, 0.47f));
				Tile tile(pos, GL_FALSE, GL_FALSE);
				tile.atBase = true;
				tempTiles.push_back(tile);
				obj.IsSolid = GL_FALSE;
				this->emptyBricks.push_back(obj);
			}

			// Check block type from level data (2D level array)
			if (tileData[y][x] == 1) // Solid
			{
				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(50, 50);
				GameObject obj(pos, size, ResourceManager::GetTexture("Purple-Box"), glm::vec3(0.37f, 0.18f, 0.47f));
				obj.IsSolid = GL_TRUE;
				Tile tile(pos, GL_TRUE, GL_FALSE);
				tempTiles.push_back(tile);
				this->normBricks.push_back(obj);
			}
			else if (tileData[y][x] == 2)	// Non-solid; now determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				/*if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);*/

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(40, 40);
				GameObject obj(pos, size, ResourceManager::GetTexture(""), color);
				GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
				obj.IsSolid = GL_FALSE;
				Tile tile(pos, GL_FALSE, GL_FALSE);
				tempTiles.push_back(tile);
				this->horBricks.push_back(obj);
				this->pellets.push_back(pellet);
			}
			else if (tileData[y][x] == 3)	// Non-solid; now determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				/*if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);*/

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(40, 40);
				GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(0.2f, 0.6f, 1.0f));
				GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
				obj.IsSolid = GL_FALSE;
				Tile tile(pos, GL_FALSE, GL_FALSE);
				tempTiles.push_back(tile);
				//tempTiles.push_back(tile);
				this->verBricks.push_back(obj);
				this->pellets.push_back(pellet);
			}
			else if (tileData[y][x] == 4)	// Non-solid; now determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				/*if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);*/

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(40, 40);
				GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(0.0f, 0.7f, 0.0f));
				GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
				obj.IsSolid = GL_FALSE;
				Tile tile(pos, GL_FALSE, GL_FALSE);
				tempTiles.push_back(tile);
				this->TopRightBricks.push_back(obj);
				this->pellets.push_back(pellet);
			}

			else if (tileData[y][x] == 5)	// Non-solid; now determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				/*if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);*/

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(40, 40);
				GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(0.1, 0.25, 0.25));
				GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
				obj.IsSolid = GL_FALSE;
				Tile tile(pos, GL_FALSE, GL_FALSE);
				tempTiles.push_back(tile);
				this->TopLeftBricks.push_back(obj);
				this->pellets.push_back(pellet);
			}

			else if (tileData[y][x] == 6)	// Non-solid; now determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				/*if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);*/

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(40, 40);
				GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(0.12, 0.12, 0.12));
				GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
				obj.IsSolid = GL_FALSE;
				Tile tile(pos, GL_FALSE, GL_FALSE);
				tempTiles.push_back(tile);
				this->BottomLeftBricks.push_back(obj);
				this->pellets.push_back(pellet);
			}

			else if (tileData[y][x] == 7)	// Non-solid; now determine its color based on level data
			{
				glm::vec3 color = glm::vec3(1.0f); // original: white
				/*if (tileData[y][x] == 2)
					color = glm::vec3(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(1.0f, 0.5f, 0.0f);*/

				glm::vec2 pos(unit_width * x, unit_height * y);
				glm::vec2 size(40, 40);
				GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(1.0f, 0.5f, 0.0f));
				GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
				obj.IsSolid = GL_FALSE;
				Tile tile(pos, GL_FALSE, GL_FALSE);
				tempTiles.push_back(tile);
				this->BottomRightBricks.push_back(obj);
				this->pellets.push_back(pellet);
			}
			else if (tileData[y][x] == 8)	// Non-solid; now determine its color based on level data
			{
			glm::vec3 color = glm::vec3(1.0f); // original: white
			/*if (tileData[y][x] == 2)
				color = glm::vec3(0.2f, 0.6f, 1.0f);
			else if (tileData[y][x] == 3)
				color = glm::vec3(0.0f, 0.7f, 0.0f);
			else if (tileData[y][x] == 4)
				color = glm::vec3(0.8f, 0.8f, 0.4f);
			else if (tileData[y][x] == 5)
				color = glm::vec3(1.0f, 0.5f, 0.0f);*/

			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(40, 40);
			GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(1.0f, 0.5f, 0.0f));
			GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
			obj.IsSolid = GL_FALSE;
			Tile tile(pos, GL_FALSE, GL_FALSE);
			tempTiles.push_back(tile);
			this->Bricks_1.push_back(obj);
			this->pellets.push_back(pellet);
			}

			else if (tileData[y][x] == 9)	// Non-solid; now determine its color based on level data
			{
			glm::vec3 color = glm::vec3(1.0f); // original: white
			/*if (tileData[y][x] == 2)
				color = glm::vec3(0.2f, 0.6f, 1.0f);
			else if (tileData[y][x] == 3)
				color = glm::vec3(0.0f, 0.7f, 0.0f);
			else if (tileData[y][x] == 4)
				color = glm::vec3(0.8f, 0.8f, 0.4f);
			else if (tileData[y][x] == 5)
				color = glm::vec3(1.0f, 0.5f, 0.0f);*/

			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(40, 40);
			GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(1.0f, 0.5f, 0.0f));
			GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
			obj.IsSolid = GL_FALSE;
			Tile tile(pos, GL_FALSE, GL_FALSE);
			tempTiles.push_back(tile);
			this->Bricks_2.push_back(obj);
			this->pellets.push_back(pellet);
			}

			else if (tileData[y][x] == 9)	// Non-solid; now determine its color based on level data
			{
			glm::vec3 color = glm::vec3(1.0f); // original: white
			/*if (tileData[y][x] == 2)
				color = glm::vec3(0.2f, 0.6f, 1.0f);
			else if (tileData[y][x] == 3)
				color = glm::vec3(0.0f, 0.7f, 0.0f);
			else if (tileData[y][x] == 4)
				color = glm::vec3(0.8f, 0.8f, 0.4f);
			else if (tileData[y][x] == 5)
				color = glm::vec3(1.0f, 0.5f, 0.0f);*/

			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(40, 40);
			GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(1.0f, 0.5f, 0.0f));
			GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
			obj.IsSolid = GL_FALSE;
			Tile tile(pos, GL_FALSE, GL_FALSE);
			tempTiles.push_back(tile);
			this->Bricks_2.push_back(obj);
			this->pellets.push_back(pellet);

			}
			
			else if (tileData[y][x] == 10)	// Non-solid; now determine its color based on level data
			{
			glm::vec3 color = glm::vec3(1.0f); // original: white
			/*if (tileData[y][x] == 2)
				color = glm::vec3(0.2f, 0.6f, 1.0f);
			else if (tileData[y][x] == 3)
				color = glm::vec3(0.0f, 0.7f, 0.0f);
			else if (tileData[y][x] == 4)
				color = glm::vec3(0.8f, 0.8f, 0.4f);
			else if (tileData[y][x] == 5)
				color = glm::vec3(1.0f, 0.5f, 0.0f);*/

			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(40, 40);
			GameObject obj(pos, size, ResourceManager::GetTexture(""), glm::vec3(1.0f, 0.5f, 0.0f));
			GameObject pellet(pos, glm::vec2(30, 30), ResourceManager::GetTexture("Pellet"), color);
			obj.IsSolid = GL_FALSE;
			Tile tile(pos, GL_FALSE, GL_FALSE);
			tempTiles.push_back(tile);
			this->Bricks_3.push_back(obj);
			this->pellets.push_back(pellet);
			}

			else if (tileData[y][x] == 11)	// Non-solid; now determine its color based on level data
			{
			glm::vec3 color = glm::vec3(1.0f); // original: white
			/*if (tileData[y][x] == 2)
				color = glm::vec3(0.2f, 0.6f, 1.0f);
			else if (tileData[y][x] == 3)
				color = glm::vec3(0.0f, 0.7f, 0.0f);
			else if (tileData[y][x] == 4)
				color = glm::vec3(0.8f, 0.8f, 0.4f);
			else if (tileData[y][x] == 5)
				color = glm::vec3(1.0f, 0.5f, 0.0f);*/

			glm::vec2 pos(unit_width * x, unit_height * y);
			glm::vec2 size(40, 40);
			GameObject obj(pos, size, ResourceManager::GetTexture(""), color);
			obj.IsSolid = GL_FALSE;
			Tile tile(pos, GL_TRUE, GL_FALSE);
			tempTiles.push_back(tile);
			this->bannedTiles.push_back(obj);
			}
		}
	}
	fillWorldTiles();
	worldTiles = fillWorldTiles();
}

std::vector<std::vector<Tile>> GameLevel::fillWorldTiles()
{
	int n = 8;
	int m = 15;
	int items = 0;
	std::vector<std::vector<Tile>> tiles(n, vector<Tile>(m));
	for (int i = 0; i < 8; i++)
	{
		tiles.push_back(std::vector<Tile>());
		for (int j = 0; j < 15; j++)
		{
			tiles[i][j] = tempTiles[items];
			items++;
		}
	}
	return tiles;
}

std::vector<Tile> GameLevel::walkableAdjacentTilesCoordForTileCoord(Tile current, std::vector<std::vector<Tile>> worldTiles)
{

	std::vector<Tile> temp;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			//cout << worldTiles[i][j].position.x << " " << worldTiles[i][j].position.y << endl;
			if (current.position == worldTiles[i][j].position)
			{
				//Top
				Tile t = worldTiles[i - 1][j];
				if (!t.IsSolid && !t.atBase)
				{
					temp.push_back(t);
				}

				//Bottom
				t = worldTiles[i + 1][j];
				if (!t.IsSolid && !t.atBase)
				{
					temp.push_back(t);
				}

				//Left
				t = worldTiles[i][j - 1];
				if (!t.IsSolid && !t.atBase)
				{
					temp.push_back(t);
				}

				//Right
				t = worldTiles[i][j + 1];
				if (!t.IsSolid && !t.atBase)
				{
					temp.push_back(t);
				}

				return temp;
			}
		}
	}
}


