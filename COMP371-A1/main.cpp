//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//

#include <iostream>


#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices

//Classes
#include "shader.h"
#include "camera.h"
#include "model.h"

//Other
#include "colors.h"
#include "vao.h"
//#include "draw.h"

/*================================================================
	Forward Declarations
================================================================*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void createShadowDepthMap(GLuint& depthMapFBO, GLuint& depthMap);
void getInput(GLFWwindow* window, float deltaTime);
void drawModel(Shader theShader);

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
Shader lightShader;
Shader depthShader;

//Uniform variables
bool drawShadows;

//Models
Model tempModel;

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
KeyControl LetterKeys[26];

//Variables necessary for animating certain movements
float rotationAnimationTime[3];
enum Axes { x = 0, y = 1, z = 3 };	//yes, there is a separate enum also named axes in the model class. however, i do no think theyre necessarily the same so ive set a separate enum

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
		Variable initialization
	--------------------------------*/
	//initialize global shaders
	shader = Shader("VertexShader.glsl", "FragmentShader.glsl");
	lightShader = Shader("VertexShaderLight.glsl", "FragmentShaderLight.glsl");
	depthShader = Shader("VertexShaderDepth.glsl", "FragmentShaderDepth.glsl");

	//Light position
	glm::vec3 lightPos = glm::vec3(0.f, 30.f, 0.f);
	shader.use();
	glUniform3fv(shader.getUniform("lightPos"), 1, &lightPos[0]);

	//Camera position
	camera = Camera(DEFAULT_SCR_WIDTH, DEFAULT_SCR_HEIGHT);

	//Frame difference for time calculations of animations
	float deltaTime = 0.0f; // Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	//Shadow stuff
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;		//Light space matrix transforms coordinates from the light sources point of view
	float near_plane = 1.f, far_plane = 141.0f;
	//set orthogonal view to be about as large as the ground
	lightProjection = glm::ortho(-80.0f, 80.0f, -80.0f, 80.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	//put it in a uniform for the depth shader to use to generate shadow map
	depthShader.use();
	depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	// Define and upload geometry to the GPU here ...
	int fuschiaCubeVAO = createCubeVAO(FUSCHIA);
	int whiteCubeVAO = createCubeVAO();

	/*--------------------------------
		Shadow Setup
	--------------------------------*/
	GLuint depthMapFBO, depthMap;
	createShadowDepthMap(depthMapFBO, depthMap);

	//debug
	/*Shader debugDepthQuad("VertexShaderDebug.glsl", "FragmentShaderDebug.glsl");
	debugDepthQuad.use();
	debugDepthQuad.setInt("depthMap", 0);*/

	/*--------------------------------
	Model Setup
	--------------------------------*/
	tempModel = Model("calvin.model");
	Model center = Model("singleCube.model");

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
		//TODO? Update uniform matrices when we actually move the camera
		camera.updateProjectionViewMatrices();
		//pass them to the shader
		shader.setMat4("viewMatrix", camera.getViewMatrix());
		shader.setMat4("projectionMatrix", camera.getProjectionMatrix());
		shader.setVec3("cameraPos", camera.getPosition());
		lightShader.setMat4("viewMatrix", camera.getViewMatrix());
		lightShader.setMat4("projectionMatrix", camera.getProjectionMatrix());

		//TODO: Update game states

		//Render from the light source's position to generate depth map which will be used to draw shadows
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		//Draw the world with the shadow depth shader
		depthShader.use();
		glBindVertexArray(fuschiaCubeVAO);
		drawModel(depthShader);
		//tempModel.draw(depthShader, 0);
		//draw(depthShader, vao);
		//draw(shader, vao);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//draw as normal but using the depth map to add shadow
		// reset viewport
		glViewport(0, 0, screenWidth, screenHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		// Draw geometry
		glUniformMatrix4fv(glGetUniformLocation(shader.ID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//draw(shader, vao);
		shader.use();
		glBindVertexArray(fuschiaCubeVAO);
		//tempModel.draw(shader, 0);
		drawModel(shader);
		glBindVertexArray(whiteCubeVAO);
		center.draw(shader, 0);

		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		/*debugDepthQuad.use();
		debugDepthQuad.setFloat("near_plane", near_plane);
		debugDepthQuad.setFloat("far_plane", far_plane);
		glActiveTexture(GL_TEXTURE0);
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
	tempModel.draw(theShader, 0);
	tempModel.drawWall(theShader, 0, glm::vec3(.0f, .0f, -11.f), Model::xAxis);
	tempModel.drawWall(theShader, 0, glm::vec3(.0f, .0f, -12.f), Model::yAxis);
	tempModel.drawWall(theShader, 0, glm::vec3(.0f, .0f, -13.f), Model::zAxis);
}

/*================================================================
	Inputs
================================================================*/
void getInput(GLFWwindow* window, float deltaTime) {

	//close
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	//Move camera and rotate about object
	//=====================================================================
	//press W -- move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.moveForward(deltaTime);
	}
	//press A -- move left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.moveLeft(deltaTime);
	}
	//press S -- move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.moveBack(deltaTime);
	}
	//press D -- move right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.moveRight(deltaTime);
	}

	//right mouse -- pan camera in any direction
	//=====================================================================
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
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

	//left-mouse -- zoom in and out.
	//=====================================================================
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
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

	//Rotate Object
	//=====================================================================
	//press Q
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		int i = (int)'q' - (int)'a';
		if (LetterKeys[i].firstClick) {
			tempModel.modelRotation.x += 90.0f;
			LetterKeys[i].firstClick = false;
			//std::cout << tempModel.modelRotation.x << ", " << tempModel.modelRotation.y << ", " << tempModel.modelRotation.z << ", ";
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_RELEASE) {
		LetterKeys[(int)'q' - (int)'a'].firstClick = true;
	}
	//press E
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		int i = (int)'e' - (int)'a';
		if (LetterKeys[i].firstClick) {
			tempModel.modelRotation.x -= 90.0f;
			LetterKeys[i].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE) {
		LetterKeys[(int)'e' - (int)'a'].firstClick = true;
	}
	//press C
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
		int i = (int)'c' - (int)'a';
		if (LetterKeys[i].firstClick) {
			tempModel.modelRotation.z += 90.0f;
			LetterKeys[i].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE) {
		LetterKeys[(int)'c' - (int)'a'].firstClick = true;
	}
	//press V
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
		int i = (int)'v' - (int)'a';
		if (LetterKeys[i].firstClick) {
			tempModel.modelRotation.z -= 90.0f;
			LetterKeys[i].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_RELEASE) {
		LetterKeys[(int)'v' - (int)'a'].firstClick = true;
	}
	//press Z
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
		int i = (int)'z' - (int)'a';
		if (LetterKeys[i].firstClick) {
			tempModel.modelRotation.y += 90.0f;
			LetterKeys[i].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) {
		LetterKeys[(int)'z' - (int)'a'].firstClick = true;
	}
	//press X
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
		int i = (int)'x' - (int)'a';
		if (LetterKeys[i].firstClick) {
			tempModel.modelRotation.y -= 90.0f;
			LetterKeys[i].firstClick = false;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE) {
		LetterKeys[(int)'x' - (int)'a'].firstClick = true;
	}

	//Model movement TEMPORARY
	//press 
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
		tempModel.modelTranslation.x += deltaTime * 10;
	}
	//press 
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		tempModel.modelTranslation.x -= deltaTime * 10;
	}
	//press 
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		tempModel.modelTranslation.z += deltaTime * 10;
	}
	//press 
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		tempModel.modelTranslation.z -= deltaTime * 10;
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
	drawShadows = false;
	shader.setBool("drawShadows", drawShadows);

}