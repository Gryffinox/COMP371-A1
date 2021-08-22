//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//

#include <iostream>
#include <map>
#include <algorithm>
#include <vector>



#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include <irrKlang.h> // sound

#include <ft2build.h> //text rendering
#include FT_FREETYPE_H

//Classes
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "OBJloader.h"  //For loading .obj files
#include "OBJloaderV2.h"  //For loading .obj files using a polygon list format

#include "texture.h"
#include "colors.h"
#include "vao.h"
#include "text.h"

using namespace std;

/*================================================================
	Forward Declarations
================================================================*/
void drawModel(Shader theShader);
int getInput(GLFWwindow* window, float deltaTime, int menu);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void createShadowDepthMap(GLuint& depthMapFBO, GLuint& depthMap);
void playSound(char* filename, bool repeat);
void drawObject(Shader aShader);
void drawSky(GLuint VAO);
void drawPlatform(GLuint VAO, Shader aShader);
void drawMenu(GLuint VAO, int menu);

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
Shader dragonShader;
Shader skyboxShader;

//Text rendering
Text* text;

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
    Object variables;
================================================================*/
int activeVAOVertices;
GLuint activeVAO;

/*================================================================
    Menu variables;
================================================================*/
enum Menu {
    Gamestart = 0,
    Controls = 1,
    Paused = 2,
    Gameover = 3,
    None = 4
};

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

GLuint setupModelVBO(string path, int& vertexCount) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;

	//read the vertex data from the model's OBJ file
	loadOBJ(path.c_str(), vertices, normals, UVs);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //Becomes active VAO
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

	//Vertex VBO setup
	GLuint vertices_VBO;
	glGenBuffers(1, &vertices_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Normals VBO setup
	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//UVs VBO setup
	GLuint uvs_VBO;
	glGenBuffers(1, &uvs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs, as we are using multiple VAOs)
	vertexCount = vertices.size();
	return VAO;
}

//Sets up a model using an Element Buffer Object to refer to vertex data
GLuint setupModelEBO(string path, int& vertexCount)
{
	vector<int> vertexIndices; //The contiguous sets of three indices of vertices, normals and UVs, used to make a triangle
	vector<glm::vec3> vertices;
	vector<glm::vec3> normals;
	vector<glm::vec2> UVs;

	//read the vertices from the cube.obj file
	//We won't be needing the normals or UVs for this program
	loadOBJ2(path.c_str(), vertexIndices, vertices, normals, UVs);

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //Becomes active VAO
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

	//Vertex VBO setup
	GLuint vertices_VBO;
	glGenBuffers(1, &vertices_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Normals VBO setup
	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	//UVs VBO setup
	GLuint uvs_VBO;
	glGenBuffers(1, &uvs_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(2);

	//EBO setup
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(int), &vertexIndices.front(), GL_STATIC_DRAW);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
	vertexCount = vertexIndices.size();
	return VAO;
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set Background Color
	glClearColor(DARK_BLUE.x, DARK_BLUE.y, DARK_BLUE.z, 1.0f);
	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	// Enable Backface culling
	glEnable(GL_CULL_FACE);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*--------------------------------
		Variable initialization
	--------------------------------*/
	//initialize global shaders
	shader = Shader("VertexShader.glsl", "FragmentShader.glsl");
	depthShader = Shader("VertexShaderDepth.glsl", "FragmentShaderDepth.glsl");
	textShader = Shader("VertexShaderText.glsl", "FragmentShaderText.glsl");
    dragonShader = Shader("VertexShaderDragon.glsl", "FragmentShaderDragon.glsl");

    /*--------------------------------
        Font Loading
    --------------------------------*/
    text = new Text(screenWidth, screenHeight, &textShader);
    text->Load("fonts/PressStart2P-Regular.ttf", 16);
    
    int menu = Menu::Gamestart;
    
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
    int skyboxVAO = createSkyboxVAO(WHITE);

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
	debugDepthQuad.setInt("depthMap", 10);

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
    loadTexture("textures/metal.jpeg", &metalTexture);
    loadTexture("textures/road.jpg", &roadTexture);
    loadTexture("textures/border.jpg", &borderTexture);
    //skybox textures
    skyboxShader.use();
    loadTexture("textures/skyboxTop.png",&skyboxTextureT);
    loadTexture("textures/skyboxLeft.png",&skyboxTextureL);
    loadTexture("textures/skyboxFront.png",&skyboxTextureFr);
    loadTexture("textures/skyboxRight.png",&skyboxTextureR);
    loadTexture("textures/skyboxBack.png",&skyboxTextureB);
    loadTexture("textures/skyboxFloor.png",&skyboxTextureFl);
    
    


	/*--------------------------------
		Object Loader
	--------------------------------*/

	//Setup models
	string heraclesPath = "models/dragon.obj";

	int heraclesVertices;
	GLuint heraclesVAO = setupModelVBO(heraclesPath, heraclesVertices);
	//TODO 3 load the models as EBOs instead of only VBOs

    activeVAOVertices = heraclesVertices;
    activeVAO = heraclesVAO;

    /*--------------------------------
    Music playback begin
    Downloaded from: https://soundcloud.com/mdkofficial
    --------------------------------*/
    //playSound((char*)"sounds/MDK-Fb.mp3", true);
   

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

        dragonShader.use();
        dragonShader.setMat4("viewMatrix", camera.getViewMatrix());
        dragonShader.setMat4("projectionMatrix", camera.getProjectionMatrix());
        dragonShader.setVec3("cameraPos", camera.getPosition());
        
        if(!paused){
                
		//Update Game State
        int status = updateGameState(deltaTime);
        if(status == 1 )
        {
            playSound((char*)"sounds/correct.wav", false);
        } else if(status == 2 ) {
            playSound((char*)"sounds/wrong.mp3", false);
        } else if(status == 3){
            menu = Menu::Gameover;
        }

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
        drawObject(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Regular draw pass
		//================================
		//draw as normal but using the depth map to add shadow
		glViewport(0, 0, screenWidth, screenHeight);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw geometry
        shader.use();
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glBindVertexArray(whiteCubeVAO);
		drawModel(shader);
        drawSky(skyboxVAO);
        drawObject(dragonShader);
        drawPlatform(whiteCubeVAO, shader);

		//Text Render
        std::string timeDisplay = timeLeft > 0 ? "TIME " + std::to_string(timeLeft) : "TIME 0";
        std::string levelDisplay = "LEVEL " + std::to_string(level);
        std::string scoreDisplay = "SCORE " + std::to_string(score);
        std::string livesDisplay = "LIVES " + std::to_string(lives);
        text->RenderText(timeDisplay, 20.0f, 20.0f, 2.f, WHITE);
        text->RenderText(levelDisplay, 20.0f, 60.0f, 2.f, TEAL);
        text->RenderText(scoreDisplay, 20.0f, 100.0f, 2.f, LIGHT_GREEN);
        text->RenderText(livesDisplay, 20.0f, 140.0f, 2.f, MANATEE);
        text->RenderText("press <esc> to exit", 50.0f, screenHeight - 80, 1.f, WHITE);
        text->RenderText("press <p> to pause the game", 50.0f, screenHeight - 50, 1.f, WHITE);
        } else {
            drawSky(skyboxVAO);
            drawMenu(whiteCubeVAO, menu);
        }

		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		/*debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, glossyTexture);
		renderQuad();*/

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handle inputs
		menu = getInput(window, deltaTime, menu);
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

void drawSky(GLuint VAO){
    shader.use();
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(90, 90, 90));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -30));
    shader.setMat4("worldMatrix", translationMatrix * scalingMatrix);
    shader.setBool("lightsOff", true);
    glBindVertexArray(VAO);
    setTexture(Texture::SkyboxFl, shader);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    setTexture(Texture::SkyboxT, shader);
    glDrawArrays(GL_TRIANGLES, 6, 6);
    setTexture(Texture::SkyboxL, shader);
    glDrawArrays(GL_TRIANGLES, 12, 6);
    setTexture(Texture::SkyboxR, shader);
    glDrawArrays(GL_TRIANGLES, 18, 6);
    setTexture(Texture::SkyboxFr, shader);
    glDrawArrays(GL_TRIANGLES, 24, 6);
    setTexture(Texture::SkyboxB, shader);
    glDrawArrays(GL_TRIANGLES, 30, 6);
    shader.setBool("lightsOff", false);
    
}

void drawPlatform(GLuint VAO, Shader aShader){
    aShader.use();
    float width = 25;
    float thickness = 0.5;
    float length = 100;
    float y = -10;
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(width, thickness, length));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, y, 0));
    aShader.setMat4("worldMatrix", translationMatrix * scalingMatrix);
    glBindVertexArray(VAO);
    //main platform
    setTexture(Texture::Metal, aShader);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    setTexture(Texture::Border, aShader);
    for (int i = -2; i<3; i++)
    {
        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3, thickness, length/5));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-width/2-1.5, y, -length/5*i));
        aShader.setMat4("worldMatrix", translationMatrix * scalingMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3, thickness, length/5));
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(width/2+1.5, y, -length/5*i));
        aShader.setMat4("worldMatrix", translationMatrix * scalingMatrix);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void drawMenu(GLuint VAO, int menu){
    switch (menu){
    case Menu::Gamestart:
            text->RenderText("DRAGON SOMETHING GAME LOL", 50, 400, 4.5, TEAL);
            text->RenderText("Press <p> to start game", 50, 500, 3, WHITE);
            text->RenderText("press <esc> to exit", 50.0f, screenHeight - 80, 1.f, WHITE);
            text->RenderText("press <c> to see the game controls", 50.0f, screenHeight - 50, 1.f, WHITE);
        break;
    case Menu::Gameover:
            text->RenderText("GAMEOVER", 50, 200, 5, RED);
            text->RenderText("Press <p> to start game", 50, 300, 3, WHITE);
            text->RenderText("press <esc> to exit", 50.0f, screenHeight - 80, 1.f, WHITE);
            text->RenderText("press <c> to see the game controls", 50.0f, screenHeight - 50, 1.f, WHITE);
        break;
    case Menu::Paused:
            text->RenderText("PAUSED", 50, 200, 5, PURPLE_NAVY);
            text->RenderText("Press <p> to resume game", 50, 280, 3, WHITE);
            text->RenderText("press <c> to see the game controls", 50.0f, screenHeight - 50, 1.f, WHITE);
            text->RenderText("press <esc> to exit", 50.0f, screenHeight - 80, 1.f, WHITE);
        break;
    case Menu::Controls:
            float y = 50;
            float offset = 60;
            text->RenderText("CONTROL MENU", 50, y, 5, PURPLE_NAVY );
            text->RenderText("Rotate <a> <d> <w> <s> <q> <e>", 50, y+=2*offset, 2, WHITE );
            text->RenderText("Move camera <u> <j> <k> <h>", 50, y+=offset, 2, WHITE );
            text->RenderText("Pan camera <Right click> + hold <f>", 50, y+=offset, 2, WHITE );
            text->RenderText("Toggle music on/off <m>", 50, y+=offset, 2, WHITE );
            text->RenderText("Music volume <+> <->", 50, y+=offset, 2, WHITE );
            text->RenderText("Press <p> to resume game", 50, y+=offset, 2, WHITE);
            text->RenderText("Press <esc> to exit game", 50, y+=offset, 2, WHITE);
        break;
    }
    
}

/*================================================================
	Inputs
================================================================*/
int getInput(GLFWwindow* window, float deltaTime, int menu) {
    
    
	//close
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
    //toggle music
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    //increase volume
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    //decrease volume
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (menu == Menu::None)
    {
        //Rotate Object
        //=====================================================================
        //press W -- rotate forward (x axis)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !paused) {
            int w = (int)'w' - (int)'a';
            if (LetterKeys[w].firstClick) {
                rotateForward();
                LetterKeys[w].firstClick = false;
                playSound((char*)"sounds/click.wav", false);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
            LetterKeys[(int)'w' - (int)'a'].firstClick = true;
        }
        //press S -- rotate backwards (x axis)
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && !paused) {
            int s = (int)'s' - (int)'a';
            if (LetterKeys[s].firstClick) {
                rotateBackward();
                LetterKeys[s].firstClick = false;
                playSound((char*)"sounds/click.wav", false);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
            LetterKeys[(int)'s' - (int)'a'].firstClick = true;
        }
        //press A -- rotate left (y axis)
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && !paused) {
            int a = (int)'a' - (int)'a';
            if (LetterKeys[a].firstClick) {
                turnLeft();
                LetterKeys[a].firstClick = false;
                playSound((char*)"sounds/click.wav", false);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
            LetterKeys[(int)'a' - (int)'a'].firstClick = true;
        }
        //press D -- rotate right (y axis)
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && !paused) {
            int d = (int)'d' - (int)'a';
            if (LetterKeys[d].firstClick) {
                turnRight();
                LetterKeys[d].firstClick = false;
                playSound((char*)"sounds/click.wav", false);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
            LetterKeys[(int)'d' - (int)'a'].firstClick = true;
        }
        //press Q -- roll left (z axis)
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && !paused) {
            int q = (int)'q' - (int)'a';
            if (LetterKeys[q].firstClick) {
                rollLeft();
                LetterKeys[q].firstClick = false;
                playSound((char*)"sounds/click.wav", false);
            }
        }
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) {
            LetterKeys[(int)'q' - (int)'a'].firstClick = true;
        }
        //press E -- roll right (z axis)
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && !paused) {
            int e = (int)'e' - (int)'a';
            if (LetterKeys[e].firstClick) {
                rollRight();
                LetterKeys[e].firstClick = false;
                playSound((char*)"sounds/click.wav", false);
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
                return Menu::Paused;
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
    } else {
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            return Menu::Gamestart;
        }
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
            return Menu::Controls;
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
            int p = (int)'p' - (int)'a';
            if (LetterKeys[p].firstClick) {
                paused = !paused;
                LetterKeys[p].firstClick = false;
                return Menu::None;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
            LetterKeys[(int)'p' - (int)'a'].firstClick = true;
        }
        return menu;
    }
    return Menu::None;
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
    text->width = width;
    text->height = height;
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

void drawObject(Shader aShader)
{
    aShader.use();

    // Set world matrix
    glm::mat4 modelMatrix =
        glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, .5f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 10, -35));
    glm::mat4 transformationMatrix = translationMatrix * modelMatrix;
    aShader.setMat4("worldMatrix", transformationMatrix);

    //Draw the stored vertex objects
    glBindVertexArray(activeVAO);
    glDrawArrays(GL_TRIANGLES, 0, activeVAOVertices);

    glBindVertexArray(0);
}
