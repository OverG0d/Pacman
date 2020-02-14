#include "game.h"
#include "resource_manager.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "BallObject.h"
#include "Tile.h"
#include <iostream>

using namespace std;


// Game-related State data
SpriteRenderer  *Renderer;
GameObject      *Player;
BallObject      *Blinky;
Tile            *tile;



Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Blinky;
}

void Game::Init()
{
	// Load shaders
	ResourceManager::LoadShader("Sprite.vs", "Sprite.frag", nullptr, "sprite");
	// Configure shaders
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
	// Load textures
	//ResourceManager::LoadTexture("D:/Pacman/Textures/background.jpg", GL_FALSE, "background");
	//ResourceManager::LoadTexture("D:/Pacman/Textures/Pacman.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("D:/Pacman/Textures/Purple-Box.jpg", GL_FALSE, "Purple-Box");
	ResourceManager::LoadTexture("D:/Pacman/Textures/block_solid.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("D:/Pacman/Textures/Pacman.png", true, "paddle");
	ResourceManager::LoadTexture("D:/Pacman/Textures/Blinky.png", true, "Blinky");
	ResourceManager::LoadTexture("D:/Pacman/Textures/Pellet.png", true, "Pellet");
	// Set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	// Load levels
	GameLevel one; one.Load("D:/Pacman/Levels/One.lvl.txt", this->Width, this->Height * 0.5);
	GameLevel two; two.Load("D:/Pacman/Levels/Two.lvl.txt", this->Width, this->Height * 0.5);
	GameLevel three; three.Load("D:/Pacman/Levels/Three.lvl.txt", this->Width, this->Height * 0.5);
	GameLevel four; four.Load("D:/Pacman/Levels/Four.lvl.txt", this->Width, this->Height * 0.5);
	level.Load("D:/Pacman/Levels/One.lvl.txt", this->Width, this->Height * 0.5);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
	Left = true;
	Right = true;
	Down = true;
	Up = true;
	timer = 0;
	timer1 = 4;
	timer2 = 6;
	//cout << tempTiles.size() << endl;
	// Configure game objects
	//glm::vec2 playerPos = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	glm::vec2 playerPos = glm::vec2(380, 100);
	glm::vec2 tempPos = glm::vec2(380, 310);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	//glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	Blinky = new BallObject(tempPos, glm::vec2(30,30), ResourceManager::GetTexture("Blinky"));
	//Winky = new GameObject(playerPos, glm::vec2(30, 30), ResourceManager::GetTexture("Blinky"));
	//Blinky->Position = tempPos;
	//Blinky->MoveTowards(currentTile.position, targetTile.position, currentTile, level.worldTiles);
	startBase = true;
	left = true;
}

void Game::Update(GLfloat dt)
{
	// Update objects
	//Ball->Move(dt, this->Width);
	// Check for collisions
	timer -= dt;
	timer1 -= dt;
	timer2 -= dt;
	this->DoCollisions();
	if (timer1 >= 0)
	{
		if (Blinky->Position.x <= 330)
			left = false;

		if (Blinky->Position.x >= 420)
			left = true;

		if (left)
		{
			Blinky->Position.x -= 30 * dt;
		}
		else
		{
			Blinky->Position.x += 30 * dt;
		}


	}

	if (timer2 >= 0 && timer1 <= 0)
	{
		Blinky->Position = Blinky->MoveTowards(Blinky->Position, glm::vec2(380, 310), 3 * dt);
	}

	

	if (startBase && timer2 <= 0)
	{
		Blinky->Position.y -= 30 * dt;
	}
	//cout << Blinky->shortestPath.size() << endl;
	//cout << Blinky->Position.x << " " << Blinky->Position.y << endl;
	if(timer <= 0 && !startBase) 
	{
	   //Blinky->Position = Blinky->MoveTowards(Blinky->Position, targetTile.position, 3 * dt);
		if (Blinky->shortestPath.size() > 1)
		{
			//cout << Blinky->shortestPath.size() << endl;
			Blinky->shortestPath.erase(Blinky->shortestPath.begin() + 0);
			path = Blinky->shortestPath[0];

			path1 = &path;

			
			//cout << Blinky->shortestPath[1].position.x << " " << Blinky->shortestPath[1].position.y << endl;
			timer = 1;
		}
		//cout << targetTile.position.x << " " << targetTile.position.y << endl;
	   //Blinky->Position = Blinky->MoveTowards(Blinky->Position, Player->Position, 3 * dt);  
	   //cout << Blinky->Position.x << " " << Blinky->Position.y << endl;
	}


	if (!startBase)
	{
		Blinky->Position = Blinky->MoveTowards(Blinky->Position, path1->position, 5 * dt);
	}
   
	if (pelletCounter == 38)
	{
		exit(0);
	}

	

	
	
	
	
	// Check loss condition
	/*if (Ball->Position.y >= this->Height) // Did ball reach bottom edge?
	{
		this->ResetLevel();
		this->ResetPlayer();
	}*/
}


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		GLfloat velocity = PLAYER_VELOCITY * dt;
		// Move playerboard
		if (this->Keys[GLFW_KEY_A] && Left)
		{
			if (Player->Position.x <= 0)
			{
				Player->Position.x = 800;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);

			}
			    //cout << Player->Position.x << endl;
				Player->Position.x -= velocity;
				Player->Rotation = 110;
				/*if (Ball->Stuck)
					Ball->Position.x -= velocity;*/
			
		}
		if (this->Keys[GLFW_KEY_D] && Right)
		{
			if (Player->Position.x >= 770)
			{
				Player->Position.x = 0;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);

			}
			    //cout << Player->Position.x << endl;
				Player->Position.x += velocity;
				Player->Rotation = 0;
				/*if (Ball->Stuck)
					Ball->Position.x += velocity;*/
			
		}

		if (this->Keys[GLFW_KEY_W] && Up)
		{
		
			//cout << Player->Position.y << endl;
				Player->Position.y -= velocity;
				Player->Rotation = 300;
				//if (Ball->Stuck)
					//Ball->Position.x += velocity;
			
		}

		if (this->Keys[GLFW_KEY_S] && Down)
		{
	
				//cout << Player->Position.y << endl;
				Player->Position.y += velocity;
				Player->Rotation = 190;
				/*if (Ball->Stuck)
					Ball->Position.x += velocity;*/
			
		}
		//if (this->Keys[GLFW_KEY_SPACE])
			//Ball->Stuck = false;
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		// Draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		// Draw level
		this->Levels[this->Level].Draw(*Renderer);
		// Draw player
		Player->Draw(*Renderer);
		// Draw ball
		Blinky->Draw(*Renderer);
		//Winky->Draw(*Renderer);
	}
}


void Game::ResetLevel()
{
	if (this->Level == 0)this->Levels[0].Load("levels/one.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 1)
		this->Levels[1].Load("levels/two.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 2)
		this->Levels[2].Load("levels/three.lvl", this->Width, this->Height * 0.5f);
	else if (this->Level == 3)
		this->Levels[3].Load("levels/four.lvl", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
	// Reset player/ball stats
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y);
	//Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}



// Collision detection
GLboolean CheckCollision(GameObject &one, GameObject &two);
//Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 closest);

void Game::DoCollisions()
{

	GLboolean collision = CheckCollision(*Player, *Blinky);
	if (collision)
	{
		//glfwSetWindowShouldClose(window, GL_TRUE);
		exit(0);
	}

	for (GameObject &box : this->Levels[this->Level].emptyBricks)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Blinky, box);
			if (collision)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
			}

		}
	}


	for (GameObject &box : this->Levels[this->Level].horBricks)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				targetTile = FindTargetTile(temp);
				//cout << targetTile.position.x << " " << targetTile.position.y << endl;
				Right = true;
				Left = true;
				Player->Position.y = box.Position.y;
				// Destroy block if not solid
				/*if (!box.IsSolid)
					box.Destroyed = GL_TRUE;*/
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
				startBase = false;
			}

		}
	}

	for (GameObject &box : this->Levels[this->Level].verBricks)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				targetTile = FindTargetTile(temp);
				Right = true;
				Up = true;
				Down = true;
				Player->Position.x = box.Position.x;
				// Destroy block if not solid
				//if (!box.IsSolid)
					//box.Destroyed = GL_TRUE;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
			}
		}
	}

	for (GameObject &box : this->Levels[this->Level].TopRightBricks)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				targetTile = FindTargetTile(temp);
				if (Player->Position.x >= 691)
				{
					Right = false;
				}
				if (Player->Position.y <= 62)
				{
					Up = false;
				}
				if (Player->Position.x <= 60)
				{
					Left = false;
				}
				//if(Player->Position.y )
				// Destroy block if not solid
				//if (!box.IsSolid)
					//box.Destroyed = GL_TRUE;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
			}
		}
	}

	for (GameObject &box : this->Levels[this->Level].TopLeftBricks)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				targetTile = FindTargetTile(temp);
				Down = true;
				if (Player->Position.y <= 62)
				{
					Up = false;
				}

				//if(Player->Position.y )
				// Destroy block if not solid
				//if (!box.IsSolid)
					//box.Destroyed = GL_TRUE;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
			}
		}
	}
	
	for (GameObject &box : this->Levels[this->Level].BottomLeftBricks)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				targetTile = FindTargetTile(temp);
				Down = true;
				if (Player->Position.x <= 163)
				{
					Left = false;
				}
				else
				{
					Left = true;
				}
				if (Player->Position.x >= 583)
				{
			         Right = false;
				}
				if (Player->Position.y <= 186)
				{
					Up = false;
				}
				//if(Player->Position.y )
				// Destroy block if not solid
				//if (!box.IsSolid)
					//box.Destroyed = GL_TRUE;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
			}
		}
	}

	for (GameObject &box : this->Levels[this->Level].BottomRightBricks)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				targetTile = FindTargetTile(temp);
				Left = true;
				Right = true;
				if (Player->Position.y >= 310)
				{
					Down = false;
				}
				//if(Player->Position.y )
				// Destroy block if not solid
				//if (!box.IsSolid)
					//box.Destroyed = GL_TRUE;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
			}
		}
	}

	for (GameObject &box : this->Levels[this->Level].Bricks_1)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				targetTile = FindTargetTile(temp);
				Up = true;
				if (Player->Position.y >= 185)
				{
					Down = false;
				}
				//if(Player->Position.y )
				// Destroy block if not solid
				//if (!box.IsSolid)
					//box.Destroyed = GL_TRUE;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
			}
		}
	}

	for (GameObject &box : this->Levels[this->Level].Bricks_2)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				targetTile = FindTargetTile(temp);
				Up = true;
				Left = true;
				if (Player->Position.x >= 176)
				{
					Right = false;
				}
				if (Player->Position.y >= 318)
				{
					Down = false;
				}
				//if(Player->Position.y )
				// Destroy block if not solid
				//if (!box.IsSolid)
					//box.Destroyed = GL_TRUE;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
			}
		}
	}

	for (GameObject &box : this->Levels[this->Level].Bricks_3)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				targetTile = FindTargetTile(temp);
				Up = true;
				Right = true;
				if (Player->Position.x <= 585)
				{
					Left = false;
				}
				if (Player->Position.y >= 318)
				{
					Down = false;
				}
				//if(Player->Position.y )
				// Destroy block if not solid
				//if (!box.IsSolid)
					//box.Destroyed = GL_TRUE;
				Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				currentTile = FindCurrentTile(temp);
			}
		}
	}

	for (GameObject &box : this->Levels[this->Level].bannedTiles)
	{
		if (!box.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, box);
			GLboolean collision1 = CheckCollision(*Blinky, box);
			if (collision) // If collision is true
			{
				glm::vec2 temp = box.Position;
				//targetTile = FindTargetTile(temp);
				//cout << targetTile.position.x << " " << targetTile.position.y << endl;
				Right = true;
				Left = true;
				Player->Position.y = box.Position.y;
				// Destroy block if not solid
				/*if (!box.IsSolid)
					box.Destroyed = GL_TRUE;*/
				//Blinky->MoveToward(currentTile.position, targetTile.position, currentTile, level.worldTiles);
			}
			if (collision1)
			{
				glm::vec2 temp = box.Position;
				//currentTile = FindCurrentTile(temp);
				startBase = true;
				/*(Blinky->Position.x <= 0)
				Blinky->Position.x = 770;
				if (Blinky->Position.x >= 770)
					Blinky->Position.x = 0;*/
			}

		}
	}

	for (GameObject &pellet : this->Levels[this->Level].pellets)
	{
		if (!pellet.Destroyed)
		{
			GLboolean collision = CheckCollision(*Player, pellet);
			if (collision) // If collision is true
			{
				pellet.Destroyed = GL_TRUE;
				pelletCounter++;
				cout << pelletCounter << endl;
			}
		}
	}
}

GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
	// Collision x-axis?
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x;
	// Collision y-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y;
	// Collision only if on both axes
	return collisionX && collisionY;
}

/*Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
	// Get center point circle first 
	glm::vec2 center(one.Position + one.Radius);
	// Calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2, two.Size.y / 2);
	glm::vec2 aabb_center(two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y);
	// Get difference vector between both centers
	glm::vec2 difference = center - aabb_center;
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	// Now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
	glm::vec2 closest = aabb_center + clamped;
	// Now retrieve vector between center circle and closest point AABB and check if length < radius
	difference = closest - center;

	if (glm::length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
		return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, UP, glm::vec2(0, 0));
}*/

// Calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	GLfloat max = 0.0f;
	GLuint best_match = -1;
	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

Tile Game::FindTargetTile(glm::vec2 pos)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (pos == level.worldTiles[i][j].position)
			{
				return level.worldTiles[i][j];
			}
		}
	}
}

Tile Game::FindCurrentTile(glm::vec2 pos)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (pos == level.worldTiles[i][j].position)
			{
				return level.worldTiles[i][j];
			}
		}
	}
}



/* Collision resolution
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) // Horizontal collision
				{
					Ball->Velocity.x = -Ball->Velocity.x; // Reverse horizontal velocity
					// Relocate
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->Position.x += penetration; // Move ball to right
					else
						Ball->Position.x -= penetration; // Move ball to left;
				}
				else // Vertical collision
				{
					Ball->Velocity.y = -Ball->Velocity.y; // Reverse vertical velocity
					// Relocate
					GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->Position.y -= penetration; // Move ball bback up
					else
						Ball->Position.y += penetration; // Move ball back down
				}*/
				// Also check collisions for player pad (unless stuck)
					/*Collision result = CheckCollision(*Ball, *Player);
					if (!Ball->Stuck && std::get<0>(result))
					{
						// Check where it hit the board, and change velocity based on where it hit the board
						GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
						GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
						GLfloat percentage = distance / (Player->Size.x / 2);
						// Then move accordingly
						GLfloat strength = 2.0f;
						glm::vec2 oldVelocity = Ball->Velocity;
						Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
						//Ball->Velocity.y = -Ball->Velocity.y;
						Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
						// Fix sticky paddle
						Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
					}*/

					/*for (GameObject &box : this->Levels[this->Level].TopLeftBricks)
						{
							if (!box.Destroyed)
							{
								GLboolean collision = CheckCollision(*Player, box);
								if (collision) // If collision is true
								{
									if (Player->Position.x >= 691)
									{
										Right = false;
									}
									if (Player->Position.y <= 62)
									{
										Up = false;
									}
									//if(Player->Position.y )
									// Destroy block if not solid
									//if (!box.IsSolid)
										//box.Destroyed = GL_TRUE;
								}
							}
						}*/