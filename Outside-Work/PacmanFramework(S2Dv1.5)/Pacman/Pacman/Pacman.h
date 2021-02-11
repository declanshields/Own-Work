#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#define MUNCHIECOUNT 50
#define GHOSTCOUNT 4
#define CHERRYCOUNT 4
#define MAXWALLCOUNT 15
// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

// Structure Definition

struct MovingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* sourceRect;
	int direction;
	float speed;
};

struct Enemy
{
	int currentFrameTime;
	int frameTime;
	int frameCount;
	int frame;
	Rect* rect;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};

struct Player
{
	bool dead;
	bool power;
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int lives;
	int powerElapsedTime;
	int frame;
	Rect* sourceRect;
	Texture2D* texture;
	Vector2* position;
};

struct Scene
{
	Texture2D* texture;
	Vector2* position;
	Rect* sourceRect;
};

class Pacman : public Game
{
private:
	
	
	//Input methods
	void Input(int elapsedTime, Input::KeyboardState* state, Input::MouseState* mouseState);

	//check methods
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	//Checks if the player has collided with anything
	bool checkCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2);
	void checkGhostCollisions();
	void UpdateGhost(MovingEnemy*, int elapsedTime, int a);

	
	//update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(Enemy*, int elapsedTime);
	void UpdateCherry(int elapsedTime);
	
	// Data for menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	Texture2D* _startMenuBackground;
	Rect* _startRectangle;
	Vector2* _startStringPosition;
	bool _paused;
	bool _pKeyDown;
	bool _gameStarted;
	int _score;
	int munchieCount;
	SoundEffect* _pop;
	SoundEffect* _gameOver;
	SoundEffect* _cherryPickup;
	int _levelWallCount;
	int _level;
	
	// Data to represent Pacman
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;

	// Data to represent Munchie
	const int _cMunchieFrameTime;

	// Data to represent Cherry
	const int _cCherryFrameTime;
	int _cherryFrameCount;
	int _cherryCurrentFrameTime;
	// Position for String
	Vector2* _stringPosition;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	Player* _pacman;
	Enemy* _munchies[MUNCHIECOUNT];
	Enemy* _cherry[CHERRYCOUNT];
	MovingEnemy* _ghosts[GHOSTCOUNT];
	Scene* _walls[MAXWALLCOUNT];
};