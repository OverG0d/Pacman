#include "BallObject.h"
#include "GameLevel.h"

BallObject::BallObject()
	: GameObject() { }

BallObject::BallObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite)
	: GameObject(pos, size, sprite) { }


void BallObject::MoveToward(glm::vec2 current, glm::vec2 target, Tile currentTile, std::vector<std::vector<Tile>> worldTiles)
{
	glm::vec2 fromTileCoord = current;
	glm::vec2 toTileCoord = target;
	GameLevel level;

	
	if (target != end.position)
	{
		pathFound = false;
		shortestPath.clear();
	}

	if (pathFound)
	{
		return;
	}



	InsertInOpenSteps(currentTile);
	
	do
	{
		
			Tile currentTile = openList[0];

			Tile* currentStep = &currentTile;

			shortestPath.push_back(currentTile);
			closedList.push_back(currentTile);

			openList.erase(openList.begin() + 0);

		//cout << currentTile.position.x << " " << currentTile.position.y << endl;
		if (currentTile.position == target)
		{
			for (int i = 0; i < shortestPath.size(); i++)
			{
				//cout << shortestPath[i].position.x << " " << shortestPath[i].position.y << endl;
			}
			    end = shortestPath.back();
			    pathFound = true; 
				openList.clear();
				closedList.clear();
				return;
		}

        vector<Tile> adjSteps = level.walkableAdjacentTilesCoordForTileCoord(currentTile, worldTiles);
		for (Tile s : adjSteps)
		{
			Tile step = s;


			if (std::find(closedList.begin(), closedList.end(), step) != closedList.end())
			{
				continue;
			}



			int moveCost = CostToMoveFromStep(currentTile,step);


			if (std::find(openList.begin(), openList.end(), step) == openList.end()) {

				step.parent = currentStep;
				//cout << step.parent->position.x << " " << step.parent->position.y << endl;
				step.gScore = currentStep->gScore + moveCost;

				step.hScore = ComputeHScoreFromCoord(step.position, toTileCoord);

				InsertInOpenSteps(step);
			}
			else
			{
				
				auto it = std::find(openList.begin(), openList.end(), step);
				auto index = std::distance(openList.begin(), it);

				step = openList[index];

				if ((step.gScore + moveCost) < step.gScore)
				{
					step.gScore = currentStep->gScore + moveCost;

					openList.erase(openList.begin() + index);

					InsertInOpenSteps(step);
                }
			}
		}

	} while (openList.size() > 0);

	if (!pathFound)
	{
		//cout << "Uh Oh...." << endl;
	}

}

glm::vec2 BallObject::MoveTowards(glm::vec2 current, glm::vec2 target, float maxDistanceDelta)
{
	glm::vec2 a = target - current;
	float magnitude = a.length();
	if (magnitude <= maxDistanceDelta || magnitude == 0)
	{
		return target;
	}
	return current + a / magnitude * maxDistanceDelta;

}

void BallObject::Dealloc()
{
	openList.clear();
	closedList.clear();
}

void BallObject::InsertInOpenSteps(Tile step)
{
	int stepFScore = step.fScore();
	int count = openList.size();
	int i = 0;
	for (; i < count; i++)
	{
		if (stepFScore <= openList[i].fScore())
		{
			break;
		}
	}
	openList.insert(openList.begin() + i, step);
}

int BallObject::ComputeHScoreFromCoord(glm::vec2 fromCoord, glm::vec2 toCoord)
{
	return abs(toCoord.x - fromCoord.x) + abs(toCoord.y - fromCoord.y);
}

int BallObject::CostToMoveFromStep(Tile from, Tile to)
{
	return 1;
}
































/*glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width)
{
	// If not stuck to player board
	if (!this->Stuck)
	{
		// Move the ball
		this->Position += this->Velocity * dt;
		// Then check if outside window bounds and if so, reverse velocity and restore at correct position
		if (this->Position.x <= 0.0f)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = 0.0f;
		}
		else if (this->Position.x + this->Size.x >= window_width)
		{
			this->Velocity.x = -this->Velocity.x;
			this->Position.x = window_width - this->Size.x;
		}
		if (this->Position.y <= 0.0f)
		{
			this->Velocity.y = -this->Velocity.y;
			this->Position.y = 0.0f;
		}
	}
	return this->Position;
}*/

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
/*void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
	this->Position = position;
	this->Velocity = velocity;
	this->Stuck = true;
}*/