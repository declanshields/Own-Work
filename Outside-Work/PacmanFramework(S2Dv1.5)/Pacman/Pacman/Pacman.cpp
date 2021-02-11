#include "Pacman.h"
#include <time.h>
#include <cmath>
#include <sstream>
#include <iostream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.15f), _cPacmanFrameTime(250), _cMunchieFrameTime(500 * 50), _cCherryFrameTime(500)
{
    //Declare munchies
	srand(time(NULL));
	for(int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Enemy();
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameCount = rand() % 1;
		_munchies[i]->frameTime = rand() % 500 + 50;
	}

	//Declare ghosts
	for(int i = 0; i < GHOSTCOUNT; i++)
	{
		//initialise ghost character
		_ghosts[i] = new MovingEnemy();
		_ghosts[i]->direction = (rand() % 2);
		_ghosts[i]->speed = 0.2f;
	}

	//Declare cherries
	for (int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i] = new Enemy();
		_cherry[i]->frame = 0;

	}

	//Declare Walls
	for (int i = 0; i < MAXWALLCOUNT; i++)
	{
		_walls[i] = new Scene();
	}

	//pacman and game declarations
	_pacman = new Player();
	_pacman->dead = false;
	_paused = false;
	_pKeyDown = false;
	_pacman->speedMultiplier = 1.0f;
	_pacman->currentFrameTime = 0;
	_pacman->powerElapsedTime = 0;
	_pacman->frame = 0;
	_pacman->lives = 3;
	_cherryCurrentFrameTime = 0;
	_gameStarted = false;
	_score = 0;
	_level = 1;
	_levelWallCount = 3;

	//Sound declarations
	_pop = new SoundEffect();
	_gameOver = new SoundEffect();
	_cherryPickup = new SoundEffect();
	
	//Initialise important Game aspects
	Audio::Initialise();
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();

	// Set Main Menu
	_startMenuBackground = new Texture2D();
	_startMenuBackground->Load("Textures/gray.png", false);
	_startRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_startStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}
Pacman::~Pacman()
{
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _munchies[0]->texture;

	int nCount = 0;
	for(nCount = 0; nCount < MUNCHIECOUNT; nCount++)
	{
		delete _munchies[nCount]->position;
		delete _munchies[nCount]->sourceRect;
		delete _munchies[nCount];
	}
	delete[] _munchies;

	nCount = 0;
	for(nCount = 0; nCount < CHERRYCOUNT; nCount++)
	{
		delete _cherry[nCount]->rect;
		delete _cherry[nCount]->sourceRect;
		delete _cherry[nCount]->texture;
	}
	delete[] _cherry;
	
	nCount = 0;
	for(nCount = 0; nCount < GHOSTCOUNT; nCount++)
	{
		delete _ghosts[nCount]->position;
		delete _ghosts[nCount]->sourceRect;
		delete _ghosts[nCount]->texture;
	}
	delete[] _ghosts;
	delete _pop;
	delete _gameOver;
	delete _cherryPickup;

	for(int i = 0; i < MAXWALLCOUNT; i++)
	{
		delete _walls[i]->position;
		delete _walls[i]->sourceRect;
		delete _walls[i]->texture;
	}
	delete[] _walls;
}

void Pacman::LoadContent()
{
	//initialise ghost character
	for(int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = new Texture2D();
		_ghosts[i]->texture->Load("Textures/GhostBlue.png", false);
		_ghosts[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_ghosts[i]->sourceRect = new Rect(0.0f, 0.0f, 20, 20);
	}

	// Set Menu Parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
	_pop->Load("Sounds/pop.wav");
	_gameOver->Load("Sounds/death.wav"); //found at https://freesound.org/s/382310/ under a CC0 license.
	_cherryPickup->Load("Sounds/cherryPickup.wav"); //found at https://freesound.org/s/133280/ under a CC0 license.

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacman->direction = 0;
	_pacman->power = false;

	//Walls
	for(int i = 0; i < _levelWallCount; i++)
	{
		_walls[i]->sourceRect = new Rect(0.0f, 0.0f, 16, 256);
		_walls[i]->texture = new Texture2D();
		_walls[i]->texture->Load("Textures/wall.png", false);
		_walls[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (Graphics::GetViewportHeight() - _walls[i]->sourceRect->Height));
		while (_walls[i]->position->X < (_pacman->sourceRect->Width) || _walls[i]->position->X >(Graphics::GetViewportWidth() - _pacman->sourceRect->Width))
		{
			_walls[i]->position->X = (rand() % Graphics::GetViewportWidth());
			_walls[i]->position->Y = (Graphics::GetViewportHeight() - _walls[i]->sourceRect->Height);
		}
		if (checkCollision(_ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height, _walls[i]->position->X, _walls[i]->position->Y, _walls[i]->sourceRect->Width, _walls[i]->sourceRect->Height))
		{
			_ghosts[i]->position->X -= 1;
		}
	}
	
	// Load Munchie
	Texture2D* munchieTex = new Texture2D();
	munchieTex->Load("Textures/Munchie.tga", false);
	
	for(int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = new Texture2D();
		_munchies[i]->texture = munchieTex;
		_munchies[i]->rect = new Rect(100.0f, 450.0f, 32, 32);
		_munchies[i]->sourceRect = new Rect(0.0f, 0.0f, 12, 12);
		_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));

		while((_munchies[i]->position->X >= (Graphics::GetViewportWidth() - _pacman->sourceRect->Width)) || (_munchies[i]->position->X <= (_pacman->sourceRect->Width)) || (_munchies[i]->position->Y >= (Graphics::GetViewportHeight() - _pacman->sourceRect->Height)) || (_munchies[i]->position->Y <= (_pacman->sourceRect->Height)))
		{
			_munchies[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		}
	}

	for(int i = 0; i < GHOSTCOUNT; i++)
	{
		while ((_ghosts[i]->position->X + _ghosts[i]->sourceRect->Width) >= Graphics::GetViewportWidth() || (_ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height) >= Graphics::GetViewportHeight() || (_ghosts[i]->position->Y <= _pacman->sourceRect->Height))
		{
			_ghosts[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		}
	}

	// Load Cherry
	for(int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i]->texture = new Texture2D();
		_cherry[i]->texture->Load("Textures/Cherry.png", true);
		_cherry[i]->rect = new Rect(200, 450, 32, 32);
		_cherry[i]->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
		_cherry[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));

		while ((_cherry[i]->position->X >= (Graphics::GetViewportWidth() - _pacman->sourceRect->Width)) || (_cherry[i]->position->X <= (_pacman->sourceRect->Width)) || (_cherry[i]->position->Y >= (Graphics::GetViewportHeight() - _pacman->sourceRect->Height)) || (_cherry[i]->position->Y <= (_pacman->sourceRect->Height)))
		{
			_cherry[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		}
	}
	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	PlaySound(TEXT("Sounds/backgroundMusic.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	//Gets the current state of the mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	//checks if audio is initialised
	if(!Audio::IsInitialised())
	{
		std::cout << "Audio is not initialised" << std::endl;
	}

	//checks if sound effect is loaded
	if(!_pop->IsLoaded())
	{
		std::cout << "_pop member sound effect has not loaded" << std::endl;
	}

	//checks if the game has started
	if (_gameStarted == true)
	{
		CheckPaused(keyboardState, Input::Keys::P);

		if(!_paused && !_pacman->dead)
		{
			Input(elapsedTime, keyboardState, mouseState);
			UpdatePacman(elapsedTime);
			for(int i = 0; i < GHOSTCOUNT; i++)
			{
				UpdateGhost(_ghosts[i], elapsedTime, i);
				checkGhostCollisions();
			}
			for(int i = 0; i < _levelWallCount; i++)
			{
				if(checkCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _walls[i]->position->X, _walls[i]->position->Y, _walls[i]->sourceRect->Width, _walls[i]->sourceRect->Height))
				{
					_pacman->position->X -= 1;
				}
			}
			for(int i = 0; i < MUNCHIECOUNT; i++)
			{
				if (checkCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _munchies[i]->position->X, _munchies[i]->position->Y, _munchies[i]->sourceRect->Width, _munchies[i]->sourceRect->Height))
				{
					_munchies[i]->position->X = -100;
					_munchies[i]->position->Y = -100;

					_score += 5;
					Audio::Play(_pop);
				}
			}

			for(int i = 0; i < CHERRYCOUNT; i++)
			{
				if (checkCollision(_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height, _cherry[i]->position->X, _cherry[i]->position->Y, _cherry[i]->sourceRect->Width, _cherry[i]->sourceRect->Height))
				{
					_cherry[i]->position->X = -100;
					_cherry[i]->position->Y = -100;

					_score += 10;
					Audio::Play(_cherryPickup);
				}
			}
			
			for (int i = 0; i < MUNCHIECOUNT; i++)
			{
				UpdateMunchie(_munchies[i], elapsedTime);
			}
			UpdateCherry(elapsedTime);
			CheckViewportCollision();
		}
	}
}

void Pacman::CheckViewportCollision()
{
	//Checks if pacman will go off the right edge
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth()) { //1024 is game width
		//Pacman hit right wall
		_pacman->position->X = Graphics::GetViewportWidth() - _pacman->sourceRect->Width;
	}

	//Checks if pacman will go off the left edge
	if (_pacman->position->X - _pacman->sourceRect->Width < 0) { //0 is the x coord of the left edge
	//Pacman hit left wall
		_pacman->position->X = 0 + _pacman->sourceRect->Width;
	}

	//checks if pacman will go off top
	if (_pacman->position->Y - _pacman->sourceRect->Height < 0) { //0 is the y coord of the top edge
	 //Pacman hit top wall
		_pacman->position->Y = 0 + _pacman->sourceRect->Height;
	}

	//checks if pacman will go off bottom
	if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight()) { //0 is the y coord of the top edge
	//Pacman hit bottom wall
		_pacman->position->Y = Graphics::GetViewportHeight() - _pacman->sourceRect->Height;
	}
}

void Pacman::checkGhostCollisions()
{
	//local variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		//Populate variables with ghost data
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->sourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->sourceRect->Width;
		top2 = _ghosts[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			_pacman->lives -= 1;
			Audio::Play(_gameOver);
			if(_pacman->lives <= 0)
			{
				_pacman->dead = true;
				i = GHOSTCOUNT;
			}
			_pacman->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
		}
	}
}

//1 is pacman, 2 is other sprites
/*x1 is pacmans x coord, y1 is pacmans y coord, width1 is pacmans width and height1 is pacmans height.*/
bool Pacman::checkCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1, left2 = x2;
	int right1 = x1 + width1, right2 = x2 + width2;
	int top1 = y1, top2 = y2;
	int bottom1 = y1 + height1, bottom2 = y2 + height2;

	if (bottom1 < top2)
		return false;
	if (top1 > bottom2)
		return false;
	if (right1 < left2)
		return false;
	if (left1 > right2)
		return false;
	
	return true;
}


void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	if (state->IsKeyDown(pauseKey) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (state->IsKeyUp(pauseKey))
		_pKeyDown = false;
}
void Pacman::UpdatePacman(int elapsedTime)
{
	
	_pacman->currentFrameTime += elapsedTime;
	if (_pacman->currentFrameTime > _cPacmanFrameTime)
	{
		_pacman->frame++;
		if (_pacman->frame >= 2)
			_pacman->frame = 0;
		_pacman->currentFrameTime = 0;
	}
	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;
}

void Pacman::UpdateGhost(MovingEnemy* ghost, int elapsedTime, int a)
{
	int distanceX = _pacman->position->X - ghost->position->X; //Sets horizontal distance to difference between x values
	int distanceY = _pacman->position->Y - ghost->position->Y; //Sets vertical distance to difference between y values.

	if (distanceX > 0)
	{
		ghost->position->X += 1; //if pacman is to the right of the ghost, moves ghost right
		ghost->sourceRect->Y = 0;
	}
	else if (distanceX < 0)
	{
		ghost->position->X -= 1; //else moves ghost left
		ghost->sourceRect->Y = 20;
	}

	if (distanceY > 0)
		ghost->position->Y += 1; //if pacman is above ghost, moves ghost up
	else if (distanceY < 0)
		ghost->position->Y -= 1; //else moves ghost down

	for(int i = 0; i < GHOSTCOUNT; i++)
	{
		if (i == a)
			continue;
		while(checkCollision(ghost->position->X, ghost->position->Y, ghost->sourceRect->Width, ghost->sourceRect->Height, _ghosts[i]->position->X, _ghosts[i]->position->Y, _ghosts[i]->sourceRect->Width, _ghosts[i]->sourceRect->Height))
		{
			_ghosts[i]->position->X = (rand() % Graphics::GetViewportWidth());
			_ghosts[i]->position->Y = (rand() % Graphics::GetViewportHeight());
		}
	}
}

void Pacman::UpdateMunchie(Enemy*, int elapsedTime)
{
	for(int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->currentFrameTime += elapsedTime;
		if (_munchies[i]->currentFrameTime > _cMunchieFrameTime)
		{
			_munchies[i]->frame++;
			if (_munchies[i]->frame >= 2)
				_munchies[i]->frame = 0;
			_munchies[i]->currentFrameTime = 0;
		}
		_munchies[i]->sourceRect->X = _munchies[i]->sourceRect->Width * _munchies[i]->frame;
	}
}

void Pacman::UpdateCherry(int elapsedTime)
{
	_cherryCurrentFrameTime += elapsedTime;
	if (_cherryCurrentFrameTime > _cCherryFrameTime)
	{
	    for(int i = 0; i < CHERRYCOUNT; i++)
	    {
			_cherry[i]->frame++;
			if (_cherry[i]->frame >= 2)
				_cherry[i]->frame = 0;
	    }
		_cherryCurrentFrameTime = 0;
	}

	for(int i = 0; i < CHERRYCOUNT; i++)
	{
		_cherry[i]->sourceRect->X = _cherry[i]->sourceRect->Width * _cherry[i]->frame;
	}
}
void Pacman::Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState)
{
	bool rKeyDown = false;
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;
	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::D))
	{
		_pacman->position->X += pacmanSpeed; //Moves Pacman across X axis
		_pacman->direction = 0;
	}

	// Checks if S key is pressed
	else if (state->IsKeyDown(Input::Keys::S))
	{
		_pacman->position->Y += pacmanSpeed; //Moves Pacman down y axis
		_pacman->direction = 1;
	}

	// Checks if A key is pressed
	else if (state->IsKeyDown(Input::Keys::A))
	{
		_pacman->position->X -= pacmanSpeed; //Moves Pacman left
		_pacman->direction = 2;
	}

	// Checks if W key is pressed
	else if (state->IsKeyDown(Input::Keys::W))
	{
		_pacman->position->Y -= pacmanSpeed; //Moves pacman up
		_pacman->direction = 3;
	}

	//Changes orientation of the Pacman sprite
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
}
void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Score: " << _score << " Lives: " << _pacman->lives;

	SpriteBatch::BeginDraw(); // Starts Drawing
	if(!_pacman->dead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect); // Draws Pacman
	}

	if (_gameStarted == false)
	{
		Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
		std::stringstream mainStream;
		mainStream << "Press Space to Start";

		SpriteBatch::Draw(_startMenuBackground, _startRectangle, nullptr);
		SpriteBatch::DrawString(mainStream.str().c_str(), _startStringPosition, Color::Black);
		
		if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		{
			_gameStarted = true;
		}
	}
	if (_gameStarted)
	{   
	    // Draws Munchie
		for(int i = 0; i < MUNCHIECOUNT; i++)
		{
			SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->sourceRect);
		}

		//Draws cherry
		for(int i = 0; i < CHERRYCOUNT; i++)
		{
			SpriteBatch::Draw(_cherry[i]->texture, _cherry[i]->position, _cherry[i]->sourceRect);
		}

		for(int i = 0; i < GHOSTCOUNT; i++)
		{
			//Draws Ghosts
			SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->sourceRect);
		}

		//Draws wall
		for(int i = 0; i < _levelWallCount; i++)
		{
			SpriteBatch::Draw(_walls[i]->texture, _walls[i]->position, _walls[i]->sourceRect);
		}
	}

	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}