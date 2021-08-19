//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//

#ifndef game_h
#define game_h

#include <random>

//Forward Declarations
void rotateForward();
void rotateBackward();
void turnLeft();
void turnRight();
void rollLeft();
void rollRight();

/*================================================================
	Update Game States
================================================================*/
/*--------------------------------
	Game Variables
--------------------------------*/

//consts
enum Axes { posx = 1, negx = -1, posy = 2, negy = -2, posz = 3, negz = -3 };	//yes, there is a separate enum also named axes in the model class
const glm::vec3 MODEL_INITIAL_POS = glm::vec3(0.0f, 0.0f, 00.0f);
const glm::vec3 WALL_POS = glm::vec3(0.0f, 0.0f, -30.0f);
const float NEG_ROTATION = 180.0f;
const int RND_SPINS = 6;		//Number of times to randomly spin the model so that it's not always in the same default orientation

//Wall
int wallDirection = Axes::posx;
int wallUp = Axes::posy;
glm::vec3 wallRotation = glm::vec3(0.0f, 0.0f, 0.0f);

//All models
int currentModel = 1;
int numModels = 0;
Model* models;

//Game Current Model
int modelForward = Axes::negz;	//default forward is Negative Z towards the wall
int modelUp = Axes::posy;		//default up is pointed to sky (pos y)
int modelRight = Axes::posx;	//remaining direction, from cameras pov is right which is pos x

//Scoreboard
const int MAX_ATTEMPS = 20;
int totalAttempts = 0;
int score = 0;
int currentStreak = 0;

//Timer
const float INITIAL_TIME = 12.f;
const float DIFFICULTY_MULTIPLIER = 1.1f;
float totalTime = 0.0f;
float timeLeft = 0.0f;

//Pause
bool paused = false;

//Random number generation
std::random_device rndDevice;
std::mt19937 rndGenerator(rndDevice());
std::uniform_int_distribution<int> numModelDist(0, 1);		//IMPORTANT: set properly after all models loaded
std::uniform_int_distribution<int> spinsDist(0, 5);			//can spin in 6 directions
std::uniform_int_distribution<int> directionsDist(1, 3);	//3 axes
std::uniform_int_distribution<int> posNegDist(0, 1);		//pos negative

/*--------------------------------
	updateGameState function
--------------------------------*/
void updateGameState() {
	//if paused dont do anything
	if (paused) {
		return;
	}
	//new level, pick model and direction
	if (totalTime <= 0.0f) {
		//reset camera
		camera.reset();
		int direction, up, rng;
		//Select the model to use, and reset it
		//=======================
		currentModel = numModelDist(rndGenerator);
		models[currentModel].resetModel();
		models[currentModel].modelTranslation = MODEL_INITIAL_POS;
		//reset the orientation variables
		modelForward = Axes::negz;	//default forward is Negative Z towards the wall
		modelUp = Axes::posy;		//default up is pointed to sky (pos y)
		modelRight = Axes::posx;	//remaining direction, from cameras pov is right which is pos x
		//Spin the model
		for (int i = 0; i < RND_SPINS; i++) {
			rng = spinsDist(rndGenerator);	//spin in a random direction
			switch (rng) {
			case 0: rotateForward(); break;
			case 1: rotateBackward(); break;
			case 2: turnLeft(); break;
			case 3: turnRight(); break;
			case 4: rollLeft(); break;
			case 5: rollRight(); break;
			default:
				std::cout << "Something went wrong in updateGameState in game.h";
				exit(EXIT_FAILURE);
			}
		}
		//Select a wall
		//=============
		direction = directionsDist(rndGenerator); //range 1 to 3
		direction = (posNegDist(rndGenerator) == 1) ? direction : -direction;	//can either be positive or negative. pseudorandoms a 0 or 1
		//do the same thing for the up direction, but it cant be the same axis
		do {
			up = directionsDist(rndGenerator);
			up = (posNegDist(rndGenerator) == 1) ? up : -up;
		} while (std::abs(up) == std::abs(direction));
		wallDirection = direction;
		wallUp = up;
		//Set time variables
		//==================
		//if no streak, no time is removed. other wise, we start removing time exponentially
		int timeRemoved = (currentStreak == 0) ? 0 : std::pow(DIFFICULTY_MULTIPLIER, currentStreak - 1);
		totalTime = INITIAL_TIME - timeRemoved;
		timeLeft = totalTime;
	}
	//update positions and time values

}

void rotateForward() {
	//rotate the model forward
	models[currentModel].rotateModel(-90.0f, Model::xAxis);
	//update the game variables(swaps the values)
	int modelForwardSwap = modelForward;
	modelForward = modelUp;
	modelUp = -modelForwardSwap;
}

void rotateBackward() {
	//rotate model backwards
	models[currentModel].rotateModel(90.0f, Model::xAxis);
	//game variable update
	int modelForwardSwap = modelForward;
	modelForward = -modelUp;
	modelUp = modelForwardSwap;
}

void turnLeft() {
	//turn front of model to the left
	models[currentModel].rotateModel(90.0f, Model::yAxis);
	//game variable update
	int modelForwardSwap = modelForward;
	modelForward = modelRight;
	modelRight = -modelForwardSwap;
}

void turnRight() {
	//rotate front of model to the right
	models[currentModel].rotateModel(-90.0f, Model::yAxis);
	//game variable update
	int modelForwardSwap = modelForward;
	modelForward = -modelRight;
	modelRight = modelForwardSwap;
}

void rollLeft() {
	//roll model on to its left side
	models[currentModel].rotateModel(90.0f, Model::zAxis);
	//game variable update
	int modelRightSwap = modelRight;
	modelRight = -modelUp;
	modelUp = modelRightSwap;
}

void rollRight() {
	//roll model on to its right side
	models[currentModel].rotateModel(-90.0f, Model::zAxis);
	//game variable update
	int modelRightSwap = modelRight;
	modelRight = modelUp;
	modelUp = -modelRightSwap;
}

#endif