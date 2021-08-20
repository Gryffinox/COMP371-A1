//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//

#include <iostream>
#include <map>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

#include <irrKlang.h> // sound

#include <ft2build.h> //text rendering
#include FT_FREETYPE_H

//Classes
#include "shader.h"
#include "camera.h"
#include "model.h"

#include "texture.h"
#include "colors.h"
#include "vao.h"

/*================================================================
	Forward Declarations
================================================================*/
void drawModel(Shader theShader);
void getInput(GLFWwindow* window, float deltaTime);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void createShadowDepthMap(GLuint& depthMapFBO, GLuint& depthMap);
void playSound(char* filename, bool repeat);
void renderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color);

/*================================================================
	Globals
================================================================*/

/*--------------------------------
	Constants
--------------------------------*/
const float DEFAULT_SCR_WIDTH = 1024.0f;
const float DEFAULT_SCR_HEIGHT = 768.0f;
const float SHADOW_WIDTH = 4096.f;
const float SHADOW_HEIGHT = 4096.f;

const float ANIMATION_TIME = 0.4f;	//how much time should one animation take at most

/*--------------------------------
	Variables
--------------------------------*/
float screenWidth = DEFAULT_SCR_WIDTH;
float screenHeight = DEFAULT_SCR_HEIGHT;

//Camera
Camera camera;

//Shaders
Shader shader;
Shader depthShader;
Shader textShader;

//Uniform variables
bool drawShadows;

//Struct for first click controls so that each key/click can have its own instance instead of having 3000 variables
struct KeyControl {
	KeyControl() : firstClick(true) {};
	bool firstClick = true;
	static float lastMousePosX;
	static float lastMousePosY;
};
float KeyControl::lastMousePosX = 0;
float KeyControl::lastMousePosY = 0;

KeyControl RightMouseBtn;
KeyControl LeftMouseBtn;
KeyControl SpaceBar;
KeyControl LetterKeys[26];

//Variables necessary for animating certain movements
float rotationAnimationTime[3];

/*================================================================
	MORE INCLUDES because global variables required idk I'm in crank mode
================================================================*/
#include "game.h"

/*================================================================
	Sound Engine
================================================================*/
irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

/*================================================================
	Text Rendering
================================================================*/
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};
std::map<char, Character> Characters;
unsigned int VAOT, VBOT;

/*==================================================
	Shadow debug
==================================================*/

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad() {
	if (quadVAO == 0) {
		float quadVertices[] = {
			// positions        // texture Coords
			0.f,  1.f, 0.0f, 0.0f, 1.0f,
			0.f,  0.f, 0.0f, 0.0f, 0.0f,
			1.f,  1.f, 0.0f, 1.0f, 1.0f,
			1.f,  0.f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

/*================================================================
	Main
================================================================*/
int main(int argc, char* argv[]) {
	/*--------------------------------
		Open GL initilizations
	--------------------------------*/
	// Initialize GLFW and OpenGL version
	glfwInit();
#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 2.1, to support more hardware
	//sike assignment specs ask us to set it to 3.1 fuck your hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW
	//Resolution 1024 x 768
	GLFWwindow* window = glfwCreateWindow(DEFAULT_SCR_WIDTH, DEFAULT_SCR_HEIGHT, "Comp371 Project", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//Set window resize callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	//hide mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set Background Color
	glClearColor(DARK_BLUE.x, DARK_BLUE.y, DARK_BLUE.z, 1.0f);
	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	// Enable Backface culling
	glEnable(GL_CULL_FACE);

	/*--------------------------------
		Font Loading
	--------------------------------*/
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	FT_Face face;
	if (FT_New_Face(ft, "fonts/PressStart2P-Regular.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < 128; c++) {
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// generate texture
		unsigned int textTexture;
		glGenTextures(1, &textTexture);
		glBindTexture(GL_TEXTURE_2D, textTexture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			textTexture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &VAOT);
	glGenBuffers(1, &VBOT);
	glBindVertexArray(VAOT);
	glBindBuffer(GL_ARRAY_BUFFER, VBOT);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/*--------------------------------
		Variable initialization
	--------------------------------*/
	//initialize global shaders
	shader = Shader("VertexShader.glsl", "FragmentShader.glsl");
	depthShader = Shader("VertexShaderDepth.glsl", "FragmentShaderDepth.glsl");
	textShader = Shader("VertexShaderText.glsl", "FragmentShaderText.glsl");

	//Light position
	glm::vec3 lightPos = glm::vec3(-10.f, 30.f, 10.f);
	shader.use();
	glUniform3fv(shader.getUniform("lightPos"), 1, &lightPos[0]);

	//camera position
	camera = Camera(DEFAULT_SCR_WIDTH, DEFAULT_SCR_HEIGHT, glm::vec3(6.0f, 1.0f, 10.0f), glm::vec3(-1.0f, -.0f, -4.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Frame difference for time calculations of animations
	float deltaTime = 0.0f; // Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	/*--------------------------------
		VAO
	--------------------------------*/
	int fuschiaCubeVAO = createCubeVAO(FUSCHIA);
	int whiteCubeVAO = createCubeVAO();

	/*--------------------------------
		Shadow Setup
	--------------------------------*/
	//Shadow stuff
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;		//Light space matrix transforms coordinates from the light sources point of view
	float near_plane = 10.f, far_plane = 60.0f;
	//set orthogonal view to be about as large as the ground
	lightProjection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	//put it in a uniform for the depth shader to use to generate shadow map
	depthShader.use();
	depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	GLuint depthMapFBO, depthMap;
	createShadowDepthMap(depthMapFBO, depthMap);

	//debug
	Shader debugDepthQuad("VertexShaderDebug.glsl", "FragmentShaderDebug.glsl");
	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 13);

	/*--------------------------------
	Model Setup
	--------------------------------*/
	numModels = 5;
	models = new Model[numModels];
	models[0] = Model("models/amanda.model");
	models[1] = Model("models/calvin.model");
	models[2] = Model("models/charles.model");
	models[3] = Model("models/dante.model");
	models[4] = Model("models/yeeho.model");
	numModelDist = std::uniform_int_distribution<int>(0, numModels - 1);

	/*--------------------------------
	Texture load
	--------------------------------*/
	drawTextures = true;
	loadTexture("textures/glossy.jpg", &glossyTexture);
	loadTexture("textures/concrete.jpg", &concreteTexture);

	/*--------------------------------
	start bck music
	--------------------------------*/
	//playSound((char*)"sounds/lofi.mp3", true);

	/*--------------------------------
		Main Loop / Render Loop
	--------------------------------*/
	while (!glfwWindowShouldClose(window)) {

		//Update frames
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Update the camera matrices.
		camera.updateProjectionViewMatrices();
		//pass them to the shader
		shader.use();
		shader.setMat4("viewMatrix", camera.getViewMatrix());
		shader.setMat4("projectionMatrix", camera.getProjectionMatrix());
		shader.setVec3("cameraPos", camera.getPosition());

		//Update Game State
		updateGameState(deltaTime);

		/*--------------------------------
		Draw scene
		--------------------------------*/
		//Shadow Pass
		//================================
		//Render from the light source's position to generate depth map which will be used to draw shadows
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//Draw the world with the shadow depth shader
		depthShader.use();
		glBindVertexArray(whiteCubeVAO);
		drawModel(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Regular draw pass
		//================================
		//draw as normal but using the depth map to add shadow
		glViewport(0, 0, screenWidth, screenHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		// Draw geometry
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		shader.use();
		glBindVertexArray(whiteCubeVAO);
		drawModel(shader);

		//Text Render
		textShader.use();
		textShader.setMat4("projectionMatrix", glm::ortho(0.0f, screenWidth, screenHeight, 0.0f));
		renderText(textShader, "TIME | 00:00", 100.0f, 100.0f, 0.7f, WHITE);
		renderText(textShader, "SCORE 000", 1800.0f, 100.0f, .7f, TEAL);

		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		/*debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE13);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderQuad();*/

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handle inputs
		getInput(window, deltaTime);
	}
	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

/*================================================================
	Drawers
================================================================*/
void drawModel(Shader theShader) {
	setTexture(Texture::Glossy, theShader);
	models[currentModel].draw(theShader, 0);
	setTexture(Texture::Concrete, theShader);
	//Draw one of the 3 walls
	float yRotation;
	float zRotation;
	yRotation = (wallDirection > 0) ? 0.0f : NEG_ROTATION;
	switch (std::abs(wallDirection)) {
	case Axes::posx:
		//set rotation vector
		switch (wallUp) {
		case Axes::posy: zRotation = 0.0f; break;
		case Axes::posz: zRotation = 90.0f; break;
		case Axes::negy: zRotation = 180.0f; break;
		case Axes::negz: zRotation = 270.0f; break;
		default:
			std::cout << "Something went wrong in switch(wallUp) in case Axes::posx in switch(wallDirection) in drawModels() in main.cpp";
			exit(EXIT_FAILURE);
		}
		wallRotation = glm::vec3(0.0f, yRotation, zRotation);
		//draw
		models[currentModel].drawWall(theShader, 0,
			Model::xAxis,
			WALL_POS, wallRotation
		);
		break;
	case Axes::posy:
		//set rotation vector
		switch (wallUp) {
		case Axes::posz: zRotation = 0.0f; break;
		case Axes::posx: zRotation = 90.0f; break;
		case Axes::negz: zRotation = 180.0f; break;
		case Axes::negx: zRotation = 270.0f; break;
		default:
			std::cout << "Something went wrong in switch(wallUp) in case Axes::posy in switch(wallDirection) in drawModels() in main.cpp";
			exit(EXIT_FAILURE);
		}
		wallRotation = glm::vec3(0.0f, yRotation, zRotation);
		models[currentModel].drawWall(theShader, 0,
			Model::yAxis,
			WALL_POS, wallRotation
		);
		break;
	case Axes::posz:
		//set rotation vector
		switch (wallUp) {
		case Axes::posy: zRotation = 0.0f; break;
		case Axes::negx: zRotation = 90.0f; break;
		case Axes::negy: zRotation = 180.0f; break;
		case Axes::posx: zRotation = 270.0f; break;
		default:
			std::cout << "Something went wrong in switch(wallUp) in case Axes::posy in switch(wallDirection) in drawModels() in main.cpp";
			exit(EXIT_FAILURE);
		}
		wallRotation = glm::vec3(0.0f, yRotation, zRotation);
		models[currentModel].drawWall(theShader, 0,
			Model::zAxis,
			WALL_POS, wallRotation
		);
		break;
		//uh end it and let the user know
	default:
		std::cout << "Something went wrong switch(wallDirection) in drawModels() in main.cpp";
		exit(EXIT_FAILURE);
		break;
	}
}

/*================================================================
	Inputs
================================================================*/
void getInput(GLFWwindow* window, float deltaTime) {

	//close
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//Rotate Object
	//=====================================================================
	//press W -- rotate forward (x axis)
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		int w = (int)'w' - (int)'a';
		if (LetterKeys[w].firstClick) {
			rotateForward();
			LetterKeys[w].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
		LetterKeys[(int)'w' - (int)'a'].firstClick = true;
	}
	//press S -- rotate backwards (x axis)
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		int s = (int)'s' - (int)'a';
		if (LetterKeys[s].firstClick) {
			rotateBackward();
			LetterKeys[s].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
		LetterKeys[(int)'s' - (int)'a'].firstClick = true;
	}
	//press A -- rotate left (y axis)
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		int a = (int)'a' - (int)'a';
		if (LetterKeys[a].firstClick) {
			turnLeft();
			LetterKeys[a].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
		LetterKeys[(int)'a' - (int)'a'].firstClick = true;
	}
	//press D -- rotate right (y axis)
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		int d = (int)'d' - (int)'a';
		if (LetterKeys[d].firstClick) {
			turnRight();
			LetterKeys[d].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
		LetterKeys[(int)'d' - (int)'a'].firstClick = true;
	}
	//press Q -- roll left (z axis)
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		int q = (int)'q' - (int)'a';
		if (LetterKeys[q].firstClick) {
			rollLeft();
			LetterKeys[q].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) {
		LetterKeys[(int)'q' - (int)'a'].firstClick = true;
	}
	//press E -- roll right (z axis)
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		int e = (int)'e' - (int)'a';
		if (LetterKeys[e].firstClick) {
			rollRight();
			LetterKeys[e].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
		LetterKeys[(int)'e' - (int)'a'].firstClick = true;
	}
	//P to pause game updates
	//=====================================================================
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		int p = (int)'p' - (int)'a';
		if (LetterKeys[p].firstClick) {
			paused = !paused;
			LetterKeys[p].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
		LetterKeys[(int)'p' - (int)'a'].firstClick = true;
	}
	//Accelerates the movement of the model towards the wall
	//=====================================================================
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (SpaceBar.firstClick) {
			glideAcceleration = 3.0f;
			SpaceBar.firstClick = false;
		}
		else {
			if (glideAcceleration <= 4.0f) {
				glideAcceleration += 0.05;
			}
		}
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		glideAcceleration = 1.0f;
		SpaceBar.firstClick = true;
	}


	//=====================================================================
	//Debug Controls
	//=====================================================================
	//press 1 to move camera forward
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		models[currentModel].modelTranslation.z += deltaTime * 10;
		camera.changePosition(camera.getPosition() + glm::vec3(0.0f, 0.0f, deltaTime * 10));
	}
	//press 2 to move camera backwards
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		models[currentModel].modelTranslation.z -= deltaTime * 10;
		camera.changePosition(camera.getPosition() - glm::vec3(0.0f, 0.0f, deltaTime * 10));
	}
	//C to cycle axes
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		if (LetterKeys[(int)'c' - (int)'a'].firstClick) {
			totalTime = 0.0f;
			LetterKeys[(int)'c' - (int)'a'].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
		LetterKeys[(int)'c' - (int)'a'].firstClick = true;
	}
	//V to print variable states
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		if (LetterKeys[(int)'v' - (int)'a'].firstClick) {
			std::cout
				<< "Wall Direction: " << "\t" << wallDirection << "\t\t" << "Wall Up: " << "\t" << wallUp << "\n"
				<< "Model Direction: " << "\t" << modelForward << "\t\t" << "Model Up: " << "\t" << modelUp << "\n"
				<< "====================================================" << "\n";
			LetterKeys[(int)'v' - (int)'a'].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) {
		LetterKeys[(int)'v' - (int)'a'].firstClick = true;
	}
	//right mouse + f for free cam -- pan camera in any direction
	//=====================================================================
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		//Current mouse pos
		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);
		//reset lastMousePos when we first click since the mouse may have moved while not clicked
		if (RightMouseBtn.firstClick) {
			RightMouseBtn.lastMousePosX = mousePosX;
			RightMouseBtn.lastMousePosY = mousePosY;
			RightMouseBtn.firstClick = false;
		}

		//Find difference from last pos
		double dx = mousePosX - RightMouseBtn.lastMousePosX;
		double dy = mousePosY - RightMouseBtn.lastMousePosY;

		//Set last to current
		RightMouseBtn.lastMousePosX = mousePosX;
		RightMouseBtn.lastMousePosY = mousePosY;
		camera.panCamera(dx * deltaTime, dy * deltaTime);
	}
	//On release, reset right mouse click variable for inital click
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
		RightMouseBtn.firstClick = true;
	}

	//left-mouse + F for free cam-- zoom in and out.
	//=====================================================================
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		//get y position only, we don't care about x for zooming in
		double mousePosY;
		glfwGetCursorPos(window, &mousePosY, &mousePosY);
		//Reset on first click
		if (LeftMouseBtn.firstClick) {
			LeftMouseBtn.lastMousePosY = mousePosY;
			LeftMouseBtn.firstClick = false;
		}
		double dy = mousePosY - LeftMouseBtn.lastMousePosY;
		LeftMouseBtn.lastMousePosY = mousePosY;
		camera.zoomCamera(dy * deltaTime);
	}
	//On release, reset left mouse click variable for inital click
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
		LeftMouseBtn.firstClick = true;
	}
	//Move camera
	//UHJK
	//=====================================================================
	//press U -- move forward
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		camera.moveForward(deltaTime);
	}
	//press H -- move left
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		camera.moveLeft(deltaTime);
	}
	//press J -- move backward
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		camera.moveBack(deltaTime);
	}
	//press K -- move right
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		camera.moveRight(deltaTime);
	}

}

/*================================================================
	Other Functions
================================================================*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	camera.setViewportDimensions(width, height);
	screenWidth = width;
	screenHeight = height;
}

void createShadowDepthMap(GLuint& depthMapFBO, GLuint& depthMap) {
	//Taken from https://learnopengl.com
	// configure depth map FBO
	// -----------------------
	//GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);                             //generate new framebuffer for our shadow depth map
	// create depth texture
	//GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	//Texture type, mipmap level, format to store texture as, size, size, deprecated, original texture format, original texture data type, the texture itself
	//for a shadow texture, because we'll be generating it, we pass NULL.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	//Define texture behaviours - repeat over x-y axis, texel filter by nearest instead of linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//framebuffer type, what we're attaching (could be color, we want depth), which texture (2D), the texture, mipmap
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	//because our framebuffer keeps track of depth as float values, we don't need to draw or read colors so explicitly set none
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Set uniforms to shaders to use
	shader.use();
	//is the only and first sampler2d so 0
	shader.setInt("shadowMap", 15);      //will have to change accordingly when merging with main and textures
	drawShadows = true;
	shader.setBool("drawShadows", drawShadows);

}

/*================================================================
	Sound
================================================================*/
void playSound(char* filename, bool repeat) {
	SoundEngine->play2D(filename, repeat);
}

/*================================================================
	Text draw
================================================================*/
void renderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color) {
	// activate corresponding render state
	s.use();
	glUniform3f(glGetUniformLocation(s.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAOT);

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[*c];

		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 0.0f },

			{ xpos,     ypos + h,   0.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBOT);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
