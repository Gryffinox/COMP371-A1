//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
// COMP 371 Assignment 2 team LastMinuteFormed
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

#include "shader.h"
#include "camera.h"
#include "colors.h"
#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const float DEFAULT_SCR_WIDTH = 1024.0f;
const float DEFAULT_SCR_HEIGHT = 768.0f;

float screenWidth = DEFAULT_SCR_WIDTH;
float screenHeight = DEFAULT_SCR_HEIGHT;

const float modelRotationSpeedMult = 10.0;

unsigned int renderMode = GL_FILL;

float deltaTime = 0.0f;    // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void drawGround(int worldLoc, Shader aShader);
void drawCrosshairs(int worldLoc, Shader aShader);
void drawModels(int worldLoc, Shader aShader);
void drawLight(int worldLoc);

float scale = 1.0f;
glm::vec3 modelTranslation = glm::vec3{0.0f, 0.0f, 0.0f};
glm::vec3 modelRotations = glm::vec3{ 0.0f, 0.0f, 0.0f };
glm::vec3 modelRotationsWoutWall = glm::vec3{ 0.0f, 0.0f, 0.0f };
glm::vec3 rotationPoint = glm::vec3(0.0f, 0.0f, 0.0f);

//vertex start index and count variables
//start indices
int crosshairsIndex;
int groundIndex;

glm::vec3 color[]=
{
    LIGHT_BLUE,
    YELLOW,
    LIGHT_GREEN,
    DARK_ORANGE,
    FUSCHIA,
    WHITE
};

//coloredCube index dynamically set based on number of colors in color[]
int numberOfColors = (sizeof(color)/sizeof(glm::vec3));
int* coloredCubeIndex = new int[numberOfColors];

//counts
int gvCount;
int chvCount;
int cubevCount;

//model variables
int modelToDisplay = 0;
int modelMoveSpeedMult = 2;

Model Amanda;
Model Calvin;
Model Charles;
Model Dante;
Model Yeeho;
Camera camera;
Shader shader;

int* amandaColor = &coloredCubeIndex[0];
int* calvinColor = &coloredCubeIndex[1];
int* yeehoColor = &coloredCubeIndex[2];
int* danteColor = &coloredCubeIndex[3];
int* charlesColor = &coloredCubeIndex[4];

//light variables
int lightCubeIndex;
glm::vec3 lightPos = glm::vec3(0,30,0);
float lightScale = 5;
Shader lightShader;

//shadow variables
Shader depthShader;
bool drawShadows;
bool firstB = true;

bool firstM = true;
bool cursorEnabled = false;

// texture variables
unsigned int brickTexture, metalTexture, emissionMap, tileTexture;
bool textured = true;
bool glowing = true;

// input variable
bool firstX;
bool firstG;

//continuous
bool isMovingForward = false;
bool isMovingBackward = false;
bool firstZero = true;
bool firstNine = true;

//texture variables type
enum Texture
{
    BRICK,
    METAL,
    TILE,
    NONE
};

float glowIntensity = 1;
bool glowIncreasing = false;

int createVertexArrayObject()
{
    //vertices (with normal)
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
    };
    
    //note third coordinate not used, implemented as vec3 to simplify holding all info in 1 vertexArray of vec3
    glm::vec3 textureCoords[] =
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),

        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3( 1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
    };
    
    //TODO: figure out ground texture coordinate
    glm::vec3 groundArray[] = {
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        YELLOW,
        glm::vec3(0.0f, 0.0f, 0.0f),
        
        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        YELLOW,
        glm::vec3(0.0f, 1.0f, 0.0f),
        
        glm::vec3(1.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        YELLOW,
        glm::vec3(1.0f, 0.0f, 0.0f),

        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        YELLOW,
        glm::vec3(0.0f, 1.0f, 0.0f),

        glm::vec3(1.0f,0.0f,1.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        YELLOW,
        glm::vec3(1.0f, 1.0f, 0.0f),

        glm::vec3(1.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        YELLOW,
        glm::vec3(1.0f, 0.0f, 0.0f),
    };
    
    //note, texture coordinates are placeholders and are not used.
    glm::vec3 crosshairArray[] = {
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        BLUE,
        glm::vec3(0.0f, 0.0f, 0.0f),
        
        glm::vec3(0.0f,0.0f,1.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        BLUE,
        glm::vec3(0.0f, 0.0f, 0.0f),
        
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        RED,
        glm::vec3(0.0f, 0.0f, 0.0f),
        
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        RED,
        glm::vec3(0.0f, 0.0f, 0.0f),
        
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        GREEN,
        glm::vec3(0.0f, 0.0f, 0.0f),
        
        glm::vec3(1.0f,0.0f,.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        GREEN,
        glm::vec3(0.0f, 0.0f, 0.0f),
    };
    
    // array size is (cubes: colors * 36 vertices + crosshairs: 6 vertices + ground: 4 vertices) * 4 -- vertice coords, norm vector, color, texture coords
    int numParameters = 4;
    int arraySize = (numberOfColors * 36 + 6 + 6) * numParameters;
    //glm::vec3* vertexArray = new glm::vec3[arraySize];
    glm::vec3 vertexArray[912];
    
    groundIndex = 0;
    gvCount = 6;
    int istop = gvCount * numParameters;
    int ioffset = 0;
    int j=0;
    for (int i = 0; i < istop; i++)
    {
        vertexArray[i + groundIndex ] = groundArray[j];
        j++;
    }
    
    crosshairsIndex = 6;
    chvCount = 6;
    istop = chvCount * numParameters;
    ioffset = crosshairsIndex * numParameters;
    j=0;
    for (int i = 0; i < istop; i++)
    {
        vertexArray[i + ioffset] = crosshairArray[j];
        j++;
    }
    //add colored cube for each color in color[]
    cubevCount = 36;
    istop = cubevCount * numParameters;
    
    for (int kolor = 0; kolor < numberOfColors; kolor++)
    {
        coloredCubeIndex[kolor] = 12 + 36 * kolor;
        ioffset =  coloredCubeIndex[kolor] * numParameters;
        
        if(color[kolor] == WHITE)
        {
            lightCubeIndex = coloredCubeIndex[kolor];
        }
        
        j = 0;
        int k = 0;
        for(int i = 0; i < istop; i++)
        {
            //if location 0 or 1, turn coordinates from cubeVertices into vec3
            if(i%numParameters == 0 || i%numParameters == 1)
            {
                vertexArray[i + ioffset ] = glm::vec3(cubeVertices[j],cubeVertices[j+1],cubeVertices[j+2]);
                j = j+3;
                
            } else {
                //if location 2, set color
                if(i%numParameters == 2)
                {
                    vertexArray[i + ioffset] = color[kolor];
                } else
                {
                    //if location 3, set texture coords
                    vertexArray[i + ioffset] = textureCoords[k++];
                }
                
            }
        }
    }
    
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
                          3,                   // size
                          GL_FLOAT,            // type
                          GL_FALSE,            // normalized?
                          numParameters*sizeof(glm::vec3), // stride - each vertex contain 3 vec3 (position, norm, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1,                   // attribute 1 matches aNorm in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          numParameters*sizeof(glm::vec3),
                          (void*)sizeof(glm::vec3)
                          );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,                   // attribute 2 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          numParameters*sizeof(glm::vec3),
                          (void*)(2*sizeof(glm::vec3))
                          );
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3,                   // attribute 3 matches textCoords in Vertex Shader
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          numParameters*sizeof(glm::vec3),
                          (void*)(3*sizeof(glm::vec3))
                          );
    glEnableVertexAttribArray(3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // load and create a texture
    // texture 1 wall texture
    glGenTextures(1, &brickTexture);
    glBindTexture(GL_TEXTURE_2D, brickTexture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("brick.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2 model texture
    glGenTextures(1, &metalTexture);
    glBindTexture(GL_TEXTURE_2D, metalTexture);
    // load image, create texture and generate mipmaps
    data = stbi_load("silver.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 3 glowing effect
    glGenTextures(1, &emissionMap);
    glBindTexture(GL_TEXTURE_2D, emissionMap);
    // load image, create texture and generate mipmaps
    data = stbi_load("glowMap-01.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    
    // texture 4 ground texture
    glGenTextures(1, &tileTexture);
    glBindTexture(GL_TEXTURE_2D, tileTexture);
    // load image, create texture and generate mipmaps
    data = stbi_load("tile.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    
    
    return vertexArrayObject;
}

//Settings to reset whenever switching model (through controls 1-5 and Home)
void sceneReset() {
    scale = 1;
    modelRotations = glm::vec3{ .0f, .0f, .0f };
    modelTranslation = glm::vec3{ .0f, .0f, .0f };
}

/*=====================================================================
 Input controls
 =====================================================================*/
void getInput(GLFWwindow *window, float deltaTime)
{
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    //Select Model
    //=====================================================================
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        modelToDisplay = 1;
        sceneReset();
    }
    
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        modelToDisplay = 2;
        sceneReset();
    }
    
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        modelToDisplay = 3;
        sceneReset();
    }
    
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        modelToDisplay = 4;
        sceneReset();
    }
    
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        modelToDisplay = 5;
        sceneReset();
    }
    //home key to reset everything
    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) {
        //reset camera position to position 1
        //reset model variables to draw all of them
        modelToDisplay = 0;     //default case anything not 1-5
        camera.reset();
        sceneReset();
    }
    //Move camera and rotate about object
    //=====================================================================
    //press W -- move forward
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.moveForward(deltaTime);
    }
    //press A -- move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.moveLeft(deltaTime);
    }
    //press S -- move backward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.moveBack(deltaTime);
    }
    //press D -- move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.moveRight(deltaTime);
    }
    //press Q -- rotate about the object
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.rotateAboutCenter(deltaTime);
    }
    //press E -- rotate about the object
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.rotateAboutCenter(-deltaTime);
    }
    
    //Move Model and rotate model
    //=====================================================================
    //h -- move model left
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        modelTranslation.x += (deltaTime * modelMoveSpeedMult);
    }
    //k -- move model right
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        modelTranslation.x -= (deltaTime * modelMoveSpeedMult);
    }
    //u -- move model up (forward)
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        modelTranslation.z += (deltaTime * modelMoveSpeedMult);
    }
    //j -- move model down (backward)
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        modelTranslation.z -= (deltaTime * modelMoveSpeedMult);
    }
    //y -- rotate model counter clockwise
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        modelRotations.y += deltaTime * modelRotationSpeedMult;
        modelRotationsWoutWall.y += deltaTime * modelRotationSpeedMult;
    }
    //i -- rotate model clockwise
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        modelRotations.y -= deltaTime * modelRotationSpeedMult;
        modelRotationsWoutWall.y -= deltaTime * modelRotationSpeedMult;
    }
    //shift+u -- rotate model counter clockwise                                                                     //NEW CODE FOR ROTATING MODELS
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        modelRotations.x -= deltaTime * modelRotationSpeedMult;
        modelRotationsWoutWall.x -= deltaTime * modelRotationSpeedMult;
        modelTranslation.z -= (deltaTime * modelMoveSpeedMult);
    }
    //shift+j -- rotate model clockwise
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        modelRotations.x += deltaTime * modelRotationSpeedMult;
        modelRotationsWoutWall.x += deltaTime * modelRotationSpeedMult;
        modelTranslation.z += (deltaTime * modelMoveSpeedMult);
    }
    //shift+h -- rotate model counter clockwise
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        modelRotations.z += deltaTime * modelRotationSpeedMult;
        modelRotationsWoutWall.z += deltaTime * modelRotationSpeedMult;
        modelTranslation.x -= (deltaTime * modelMoveSpeedMult);
    }
    //shift+k -- rotate model clockwise
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        modelRotations.z -= deltaTime * modelRotationSpeedMult;
        modelRotationsWoutWall.z -= deltaTime * modelRotationSpeedMult;
        modelTranslation.x += (deltaTime * modelMoveSpeedMult);
    }
    //up -- rotate model counter clockwise
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        modelRotationsWoutWall.x -= deltaTime * modelRotationSpeedMult;
    }
    //down -- rotate model clockwise
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        modelRotationsWoutWall.x += deltaTime * modelRotationSpeedMult;
    }
    //left -- rotate model counter clockwise
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        modelRotationsWoutWall.y += deltaTime * modelRotationSpeedMult;
    }
    //right -- rotate model clockwise
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        modelRotationsWoutWall.y -= deltaTime * modelRotationSpeedMult;
    }
    //shift+left -- rotate model counter clockwise
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        modelRotationsWoutWall.z += deltaTime * modelRotationSpeedMult;
        modelRotationsWoutWall.y -= deltaTime * modelRotationSpeedMult;
    }
    //shift+right -- rotate model clockwise
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        modelRotationsWoutWall.z -= deltaTime * modelRotationSpeedMult;
        modelRotationsWoutWall.y += deltaTime * modelRotationSpeedMult;
    }
    //TFPL for selecting render mode
    //=====================================================================
    //press P -- display as points
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
    
    //press L -- display as wireframe
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    //press T (And F) -- display as filled triangles
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    
    //Scale up and down
    //=====================================================================
    //press + -- scale-up
    if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) {
        scale += deltaTime;
    }
    
    //press - -- scale-down
    if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) {
        scale -= deltaTime;
        if (scale < 0) {
            scale = 0;
        }
    }
    
    //shuffle
    //=====================================================================
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Amanda.shuffle(deltaTime);
        Calvin.shuffle(deltaTime);
        Charles.shuffle(deltaTime);
        Dante.shuffle(deltaTime);
        Yeeho.shuffle(deltaTime);
    }

    //shadow toggle
    //=====================================================================
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        if (firstB) {
            drawShadows = !drawShadows;
            firstB = false;
        }
        shader.use();
        shader.setBool("drawShadows", drawShadows);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
        firstB = true;
    }
    
    //right mouse -- pan camera in any direction
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        //Current mouse pos
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);
        camera.panCamera(deltaTime, mousePosX, mousePosY);
    }
    //On release, reset right mouse click variable for inital click
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        camera.firstRightMouse = true;
    }
    
    //left-mouse -- zoom in and out.
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        //get y position only, we don't care about x for zooming in
        double mousePosY;
        glfwGetCursorPos(window, &mousePosY, &mousePosY);
        camera.zoomCamera(mousePosY);
    }
    //On release, reset left mouse click variable for inital click
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        camera.firstLeftMouse = true;
    }
    
    //cursor
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        if(firstM)
        {
            if(cursorEnabled)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } 
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            cursorEnabled = !cursorEnabled;
            firstM = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
    {
        firstM = true;
    }

    // Turns the texture off by changing variable textured from the set texture
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        if (firstX)
        {
            textured = !textured;
            firstX = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE)
    {
        firstX = true;
    }

    // Turns the glowing off by changing variable textured from the set texture
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        if (firstG)
        {
            glowing = !glowing;
            firstG = false;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE)
    {
        firstG = true;
    }

    //continuous movement
    //9
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        isMovingBackward = false;
        if (firstNine) {
            firstNine = false;
            isMovingForward = !isMovingForward;
        }
        //modelTranslation.z += (deltaTime * modelMoveSpeedMult);
    }
    if (isMovingForward) {
        modelTranslation.z += (deltaTime * modelMoveSpeedMult);
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        firstNine = true;;
    }
    //0
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        isMovingForward = false;
        if (firstZero) {
            firstZero = false;
            isMovingBackward = !isMovingBackward;
        }
        //modelTranslation.z -= (deltaTime * modelMoveSpeedMult);
    }
    if (isMovingBackward) {
        modelTranslation.z -= (deltaTime * modelMoveSpeedMult);
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_RELEASE) {
        firstZero = true;
    }
}

// set Texture and changing it depending on input
void setTexture(Texture text, Shader aShader)
{
    aShader.use();
    if(glowIncreasing)
    {
        glowIntensity += 0.2 * deltaTime;
    } else {
        glowIntensity -= 0.2 * deltaTime;
    }
    if (glowIntensity >= 2.0)
        glowIncreasing = false;
    if (glowIntensity <= 0.4)
        glowIncreasing = true;
    switch(text)
    {
        case BRICK:
            aShader.setBool("colorOn", !textured);
            aShader.setBool("textureOn", textured);
            aShader.setBool("glowOn", false);
            aShader.setInt("tex", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, brickTexture);
            break;
        case METAL:
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, metalTexture);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, emissionMap);
            aShader.setBool("colorOn", true);
            aShader.setBool("textureOn", textured);
            aShader.setBool("glowOn", glowing);
            aShader.setFloat("intensity", glowIntensity);
            aShader.setInt("tex", 1);
            aShader.setInt("emissionMap", 2);
            break;
        case TILE: 
            aShader.setBool("colorOn", !textured);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, tileTexture);
            aShader.setBool("textureOn", textured);
            aShader.setBool("glowOn", false);
            aShader.setInt("tex", 3);
            
            break;
        case NONE:
            aShader.setBool("textureOn", false);
            aShader.setBool("glowOn", false);
            aShader.setBool("colorOn", true);
            break;
    }
}

//Draw everything calls other draw functions
void draw(Shader aShader, int vao)
{
    aShader.use();
    //bind main VAO containing all models and grid
    glBindVertexArray(vao);
    
    GLuint worldMatrixLocation = aShader.getUniform("worldMatrix");
    
    drawGround(worldMatrixLocation, aShader);
    drawCrosshairs(worldMatrixLocation, aShader);
    drawModels(worldMatrixLocation, aShader);
    
    drawLight(lightShader.getUniform("worldMatrix"));
    //drawLight(worldMatrixLocation);
}

//Draws the 100*100 grid
void drawGround(int worldLoc, Shader aShader)
{
    setTexture(TILE, aShader);
    for(int i = 0; i<100; i++)
    {
        for (int j = 0; j<100; j++)
        {
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),glm::vec3((1.0f*i-50.f), -10.0f, (1.0f*j -50.f)));
            glm::mat4 worldMatrix =  translationMatrix;
            glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
            //TODO: replace magic numbers with constants
            glDrawArrays(GL_TRIANGLES, groundIndex, 6);
        }
    }
}

//Draw axes crosshairs (RGB axes)
void drawCrosshairs(int worldLoc, Shader aShader)
{
    setTexture(NONE, aShader);
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(5.f, 5.f, 5.f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 worldMatrix =  translationMatrix * scalingMatrix ;
    glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
    //TODO: replace magic numbers with constants
    glDrawArrays(GL_LINES, crosshairsIndex, 6);
}

//Draws the models
void drawModels(int worldLoc, Shader aShader)
{
    glm::mat4 rotationMatrix =
        glm::rotate(glm::mat4(1.0f), glm::radians(modelRotations.x), glm::vec3(1.f, .0f, .0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(modelRotations.y), glm::vec3(.0f, 1.f, .0f)) *
        glm::rotate(glm::mat4(1.0f), glm::radians(modelRotations.z), glm::vec3(.0f, .0f, 1.f)
        );
    glUniformMatrix4fv(shader.getUniform("rotationMatrix"), 1, GL_FALSE, &rotationMatrix[0][0]);
    switch (modelToDisplay) {
        case 1:
            setTexture(METAL, aShader);
            //parameters: world location as int, vertex array offset, s t r transformations
            Amanda.draw(worldLoc, *amandaColor, scale, modelTranslation, modelRotations);
            /*Amanda.drawWall(worldLoc, *amandaColor, scale, modelTranslation, modelRotations, Amanda.xAxis);
             Amanda.drawWall(worldLoc, *amandaColor, scale, modelTranslation, modelRotations, Amanda.yAxis);*/
            setTexture(BRICK, aShader);
            Amanda.drawWall(worldLoc, *amandaColor, scale, modelTranslation, modelRotations, Amanda.zAxis);
            break;
        case 2:
            setTexture(METAL, aShader);
            Calvin.draw(worldLoc, *calvinColor, scale, modelTranslation, modelRotations);
            /*Calvin.drawWall(worldLoc, *calvinColor, scale, modelTranslation, modelRotations, Calvin.xAxis);
             Calvin.drawWall(worldLoc, *calvinColor, scale, modelTranslation, modelRotations, Calvin.yAxis);*/
            setTexture(BRICK, aShader);
            Calvin.drawWall(worldLoc, *calvinColor, scale, modelTranslation, modelRotations, Calvin.zAxis);
            break;
        case 3:
            setTexture(METAL, aShader);
            Charles.draw(worldLoc, *charlesColor, scale, modelTranslation, modelRotations);
            /*Charles.drawWall(worldLoc, *charlesColor, scale, modelTranslation, modelRotations, Charles.xAxis);
             Charles.drawWall(worldLoc, *charlesColor, scale, modelTranslation, modelRotations, Charles.yAxis);*/
            setTexture(BRICK, aShader);
            Charles.drawWall(worldLoc, *charlesColor, scale, modelTranslation, modelRotations, Charles.zAxis);
            break;
        case 4:
            setTexture(METAL, aShader);
            Dante.draw(worldLoc, *danteColor, scale, modelTranslation, modelRotations);
            /*Dante.drawWall(worldLoc, *danteColor, scale, modelTranslation, modelRotations, Dante.xAxis);
             Dante.drawWall(worldLoc, *danteColor, scale, modelTranslation, modelRotations, Dante.yAxis);*/
            setTexture(BRICK, aShader);
            Dante.drawWall(worldLoc, *danteColor, scale, modelTranslation, modelRotations, Dante.zAxis);
            break;
        case 5:
            setTexture(METAL, aShader);
            Yeeho.draw(worldLoc, *yeehoColor, scale, modelTranslation, modelRotations);
            /*Yeeho.drawWall(worldLoc, *yeehoColor, scale, modelTranslation, modelRotations, Yeeho.xAxis);
             Yeeho.drawWall(worldLoc, *yeehoColor, scale, modelTranslation, modelRotations, Yeeho.yAxis);*/
            setTexture(BRICK, aShader);
            Yeeho.drawWall(worldLoc, *yeehoColor, scale, modelTranslation, modelRotations, Yeeho.zAxis);
            break;
        default:
            setTexture(METAL, aShader);
            Amanda.draw(worldLoc, *amandaColor, scale, modelTranslation, modelRotations);
            
            Calvin.draw(worldLoc, *calvinColor, scale, modelTranslation + glm::vec3(0.0f, 0.0f, -40.0f), modelRotations);
            
            Charles.draw(worldLoc, *charlesColor, scale, modelTranslation + glm::vec3(40.0f, 0.0f, 0.0f), modelRotations);
            
            Dante.draw(worldLoc, *danteColor, scale, modelTranslation + glm::vec3(0.0f, 0.0f, 40.0f), modelRotations);
            Yeeho.draw(worldLoc, *yeehoColor, scale, modelTranslation + glm::vec3(-40.0f, 0.0f, 0.0f), modelRotations);
            setTexture(BRICK, aShader);
            Amanda.drawWall(worldLoc, *amandaColor, scale, modelTranslation, modelRotations, Amanda.zAxis);
            Calvin.drawWall(worldLoc, *calvinColor, scale, modelTranslation + glm::vec3(0.0f, 0.0f, -40.0f), modelRotations, Calvin.zAxis);
            Charles.drawWall(worldLoc, *charlesColor, scale, modelTranslation + glm::vec3(40.0f, 0.0f, 0.0f), modelRotations, Charles.zAxis);
            Dante.drawWall(worldLoc, *danteColor, scale, modelTranslation + glm::vec3(0.0f, 0.0f, 40.0f), modelRotations, Dante.zAxis);
            Yeeho.drawWall(worldLoc, *yeehoColor, scale, modelTranslation + glm::vec3(-40.0f, 0.0f, 0.0f), modelRotations, Yeeho.zAxis);
            break;
    }
}

//draw light cube
void drawLight(int worldLoc)
{
    lightShader.use();
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(lightScale, lightScale, lightScale));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),lightPos);
    glm::mat4 worldMatrix =  translationMatrix * scalingMatrix ;
    glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, lightCubeIndex, 36);
    
}

//Handle window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    camera.setWH(width, height);
    screenWidth = width;
    screenHeight = height;
}

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
/*==================================================
Shadow debug
==================================================*/


int main(int argc, char*argv[])
{
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
    GLFWwindow* window = glfwCreateWindow(DEFAULT_SCR_WIDTH, DEFAULT_SCR_HEIGHT, "Comp371 - Assignment", NULL, NULL);
    if (window == NULL)
    {
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
    
    //initialize shaders
    shader = Shader("VertexShader.glsl", "FragmentShader.glsl");
    lightShader = Shader("VertexShaderLight.glsl", "FragmentShaderLight.glsl");
    depthShader = Shader("VertexShaderDepth.glsl", "FragmentShaderDepth.glsl");
    //set light position
    shader.use();
    glUniform3fv(shader.getUniform("lightPos"), 1, &lightPos[0]);
    
    //set camera position
    camera = Camera(&shader, &lightShader, DEFAULT_SCR_WIDTH, DEFAULT_SCR_HEIGHT);

    //TODO: get models from inputs
    //Amanda
    glm::vec3 amandaPts[] = {
        glm::vec3(-1.f,  -1.f, -5.0f),
        glm::vec3(-0.f,  -0.f, -4.0f),
        glm::vec3(1.f,  1.f, -3.0f),
        glm::vec3(2.f,  2.f, -2.f),
        glm::vec3(2.f,  1.f, -1.f),
        glm::vec3(2.f,  -0.f, 0.f),
        glm::vec3(2.f,  -1.f, 1.f),
        glm::vec3(1.f,  -0.f, 2.f),
        glm::vec3(0.f,  1.f, 3.f),
        glm::vec3(-1.f,  2.f, 4.f),
    };
    float size = sizeof(amandaPts) / sizeof(glm::vec3);
    Amanda = Model(amandaPts, size);
    //Calvin
    glm::vec3 calvinPts[] = {
        glm::vec3(0.0f, 0.0f, 3.0f),
        glm::vec3(1.0f, 0.0f, 2.0f),
        glm::vec3(-1.0f, 0.0f, 2.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 1.0f),
        glm::vec3(2.0f, 0.0f, 0.0f),
        glm::vec3(-2.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 1.0f),
        glm::vec3(0.0f, -1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, -1.0f)
    };
    size = sizeof(calvinPts) / sizeof(glm::vec3);
    Calvin = Model(calvinPts, size);
    
    //Charles
    glm::vec3 charlesPts[] = {
        glm::vec3(-3.0f,0.0f,0.0f),
        glm::vec3(-2.0f,0.0f,0.0f),
        glm::vec3(-1.0f,0.0f,0.0f),
        glm::vec3(0.0f,0.0f,0.0f),
        glm::vec3(1.0f,0.0f,0.0f),
        glm::vec3(2.0f,0.0f,0.0f),
        glm::vec3(3.0f,0.0f,0.0f),
        glm::vec3(-3.0f,1.0f,0.0f),
        glm::vec3(-2.0f,1.0f,0.0f),
        glm::vec3(-1.0f,1.0f,0.0f),
        glm::vec3(0.0f,1.0f,0.0f),
        glm::vec3(1.0f,1.0f,0.0f),
        glm::vec3(2.0f,1.0f,0.0f),
        glm::vec3(3.0f,1.0f,0.0f),
        glm::vec3(-2.0f,2.0f,0.0f),
        glm::vec3(-1.0f,2.0f,0.0f),
        glm::vec3(0.0f,2.0f,0.0f),
        glm::vec3(1.0f,2.0f,0.0f),
        glm::vec3(2.0f,2.0f,0.0f),
        glm::vec3(-2.0f,-1.0f,0.0f),
        glm::vec3(-1.0f,-1.0f,0.0f),
        glm::vec3(0.0f,-1.0f,0.0f),
        glm::vec3(1.0f,-1.0f,0.0f),
        glm::vec3(2.0f,-1.0f,0.0f),
    };
    size = sizeof(charlesPts) / sizeof(glm::vec3);
    Charles = Model(charlesPts, size);
    //Dante
    glm::vec3 dantePts[] = {
        glm::vec3(0.0f,  0.0f, 0.0f),
        glm::vec3(-1.0f,  1.0f, 1.0f),
        glm::vec3(-2.0f,  1.0f, 0.0f),
        glm::vec3(1.0f,  1.0f, -1.0f),
        glm::vec3(2.0f,  1.0f, 0.0f),
        glm::vec3(-1.0f,  -1.0f, 1.0f),
        glm::vec3(-2.0f,  -1.0f, 0.0f),
        glm::vec3(1.0f,  -1.0f, -1.0f),
        glm::vec3(2.0f,  -1.0f, 0.0f)
    };
    size = sizeof(dantePts) / sizeof(glm::vec3);
    Dante = Model(dantePts, size);
    //Yeeho
    glm::vec3 yeehoPts[] = {
        glm::vec3(0.0f, 5.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, 0.0f),
        glm::vec3(-2.0f, 5.0f, 0.0f),
        glm::vec3(-1.0f, 6.0f, -1.0f),
        glm::vec3(1.0f, 6.0f, -1.0f),
        glm::vec3(-3.0f, 6.0f, -1.0f),
        glm::vec3(3.0f, 6.0f, -1.0f),
        glm::vec3(-2.0f, 7.0f, -2.0f),
        glm::vec3(2.0f, 7.0f, -2.0f),
        glm::vec3(2.0f, 7.0f, -2.0f),
        glm::vec3(0.0f, 7.0f, -2.0f)
    };
    size = sizeof(yeehoPts) / sizeof(glm::vec3);
    Yeeho = Model(yeehoPts, size);
    
    /*==================================================
        Shadow Setup
    ==================================================*/
    //Taken from https://learnopengl.com
    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;   //shadow texture resolution
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);                             //generate new framebuffer for our shadow depth map
    // create depth texture
    unsigned int depthMap;
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
    shader.setInt("shadowMap", 4);      //will have to change accordingly when merging with main and textures

    drawShadows = true;
    shader.setBool("drawShadows", drawShadows);

    //debug
    Shader debugDepthQuad("VertexShaderDebug.glsl", "FragmentShaderDebug.glsl");
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);

    /*==================================================
        Shadow setup done
    ==================================================*/

    //hide mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set Background Color
    glClearColor(DARK_BLUE.x, DARK_BLUE.y, DARK_BLUE.z, 1.0f);
    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    
    // Define and upload geometry to the GPU here ...
    int vao = createVertexArrayObject();
    
    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        camera.updateCam();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Each frame, reset color of each pixel to glClearColor
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Render from the light source's position to generate depth map which will be used to draw shadows
        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        //the light space matrix transforms coordinates into their position from the light sources point of view
        glm::mat4 lightSpaceMatrix;
        //set orthogonal view to be about as large as the ground
        float near_plane = 0.1f, far_plane = 50.0f;
        lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
        // we are at where the light is, looking down at the origin of the world
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        //lightSpaceMatrix = glm::mat4(1.0f);
        // render scene from light's point of view
        depthShader.use();
        //put it in a uniform for the depth shader to use to generate shadow map
        //only 1 matrix not an array so pass 1, and GL_FALSE because we are not transposing the matrix
        glUniformMatrix4fv(glGetUniformLocation(depthShader.ID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

        //prepare to render from the light's view
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //Draw the world with the shadow depth shader
        draw(depthShader, vao);
        //draw(shader, vao);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, screenWidth, screenHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();

        //draw as normal but using the depth map to add shadow
        // Draw geometry
        glUniformMatrix4fv(glGetUniformLocation(shader.ID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        draw(shader, vao);

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
