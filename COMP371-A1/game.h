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
void moveScene(float distance);
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
const float TOTAL_DISTANCE = 30.0f;
const float CAMERA_LOCK_DISTANCE = 10.0f;
const glm::vec3 MODEL_INITIAL_POS = glm::vec3(0.0f, 0.0f, 00.0f);
const glm::vec3 WALL_POS = glm::vec3(0.0f, 0.0f, -TOTAL_DISTANCE);
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
const int MAX_LEVEL = 20;
const int BASE_SCORE_PER_LEVEL = 100;
const int PTS_LEVEL_DIFFICULTY = 10;
const int STREAK_BONUS = 50;
int level = 1;
int score = 0;
int currentStreak = 0;

//Timer
const float INITIAL_TIME = 12.f;
const float DIFFICULTY_MULTIPLIER = 1.1f;
const float WAIT_TIME_MAX = 1.2f;	//in ???? idek what unit i think seconds?
//const float TIME_DEFICIT_PER_LEVEL = 0.5f;
float totalTime = 0.0f;
float timeLeft = 0.0f;
float glideAcceleration = 1.0f;		//when the user whishes to accelerate the animation
float waitTime = 0.0f;				//time between levels

//Game State
enum GameState { NewLevel = 0, InLevel = 1, PostLevel = 2 };
float gameState = GameState::NewLevel;

//other game values
float travelDistance = 0.0f;		//the distance the model will travel for the current game interval
float postLvlAcceleration = 1.0f;	//accelerates the models movement once its past the wall

//Pause
bool paused = true;	//start paused

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
int updateGameState(float deltaTime) {
    bool scored = false;
    bool failed = false;
    
	//if paused dont do anything
	if (paused || level > MAX_LEVEL) {
		return 0;
	}
	//New level setup
	//================================
	if (gameState == GameState::NewLevel) {
		gameState = GameState::InLevel;
		//reset camera position
		camera.reset();
		int direction, up, rng;
		//Select the model to use, and reset it
		//----------------
		currentModel = numModelDist(rndGenerator);
		models[currentModel].resetModel();
		models[currentModel].modelTranslation = MODEL_INITIAL_POS;
		//reset the orientation variables
		modelForward = Axes::negz;	//default forward is Negative Z towards the wall
		modelUp = Axes::posy;		//default up is pointed to sky (pos y)
		modelRight = Axes::posx;	//remaining direction, from cameras pov is right which is pos x
		//Spin the model in random directions
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
		//----------------
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
		//----------------
		//if no streak, no time is removed. other wise, we start removing time exponentially
 		float timeRemoved = (currentStreak == 0) ? 0 : std::pow(DIFFICULTY_MULTIPLIER, currentStreak - 1);
		//a little time is also lost per level. relative to initial time and total number of levels
		//at a scale of 3, excluding the streak, the time will lose at most a third of initial time
		timeRemoved += (level - 1) * (INITIAL_TIME / (MAX_LEVEL * 3));
		totalTime = INITIAL_TIME - timeRemoved;
		//cap the smallest amount of time deficit to half of initial time
		if (totalTime < INITIAL_TIME / 2) { totalTime = INITIAL_TIME / 2; }
		timeLeft = totalTime;
	}
	//Once arrived at the end
	//================================
	if (timeLeft <= 0.0f && gameState == GameState::InLevel) {
		//set timeleft to 0 to that user never sees negative time
		timeLeft = 0.0f;
		//Check for right orientation
		if (modelUp == wallUp && modelForward == wallDirection) {
			//increment score by base, level and streak
			score += 
				BASE_SCORE_PER_LEVEL + 
				PTS_LEVEL_DIFFICULTY * (level - 1) + 
				STREAK_BONUS * currentStreak;
			currentStreak++;
            scored = true;
		}
		else {
			currentStreak = 0;
            failed = true;
		}
		level++;
		//put the game in the post level pause
		gameState = GameState::PostLevel;
	}
	//Give the player some time before next level to see the model through the wall
	//================================
	if (gameState == GameState::PostLevel) {
		postLvlAcceleration = 2.5f;
		waitTime = waitTime + (deltaTime * glideAcceleration);
		if (waitTime >= WAIT_TIME_MAX) {
			postLvlAcceleration = 1.0f;
			waitTime = 0.0f;
			gameState = GameState::NewLevel;
		}
	}
	//Update Timer and scene
	//===========================
	timeLeft -= deltaTime * glideAcceleration;
	travelDistance = (deltaTime * glideAcceleration * postLvlAcceleration) / totalTime;
	moveScene(travelDistance * TOTAL_DISTANCE);
    if (failed)
    {
        return 2;
    }
    return scored;
}

void moveScene(float distance) {
	models[currentModel].modelTranslation.z -= distance;
	//since z positiion are in negative range
	if (camera.getPosition().z > -(TOTAL_DISTANCE - CAMERA_LOCK_DISTANCE)) {
		camera.changePosition(camera.getPosition() - glm::vec3(0.0f, 0.0f, distance));
	}
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
