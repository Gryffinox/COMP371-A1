//
// COMP 371 Assignment 1 team LastMinuteFormed
//
// Built on in-class Lab Framework
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


const float SCR_WIDTH = 1024.0f;
const float SCR_HEIGHT = 768.0f;

const float modelRotationSpeedMult = 10.0;

unsigned int renderMode = GL_FILL;

float deltaTime = 0.0f;    // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void drawGround(int worldLoc);
void drawCrosshairs(int worldLoc);
void drawModels(int worldLoc);

float scale = 1.0f;
glm::vec3 modelTranslation = glm::vec3{0.0f, 0.0f, 0.0f};
glm::vec3 modelRotations = glm::vec3{ 0.0f, 0.0f, 0.0f };
glm::vec3 rotationPoint = glm::vec3(0.0f, 0.0f, 0.0f);

int modelToDisplay = 0;
int modelMoveSpeedMult = 2;

Model Amanda;
Model Calvin;
Model Charles;
Model Dante;
Model Yeeho;
Camera camera;
Shader shader;

int createVertexArrayObject()
{
    
    float cubeVertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
    };
    
    glm::vec3 groundArray[] = {
        glm::vec3(0.0f,0.0f,0.0f),
        YELLOW,
        glm::vec3(0.0f,0.0f,1.0f),
        YELLOW,
        glm::vec3(1.0f,0.0f,1.0f),
        YELLOW,
        glm::vec3(1.0f,0.0f,0.0f),
        YELLOW,
    };
    
    glm::vec3 crosshairArray[] = {
        glm::vec3(0.0f,0.0f,0.0f),
        BLUE,
        glm::vec3(0.0f,0.0f,1.0f),
        BLUE,
        glm::vec3(0.0f,0.0f,0.0f),
        RED,
        glm::vec3(0.0f,1.0f,0.0f),
        RED,
        glm::vec3(0.0f,0.0f,0.0f),
        GREEN,
        glm::vec3(1.0f,0.0f,.0f),
        GREEN,
    };
    
    //sizeof(cubeVertices)/sizeof(float)/3 = 36
    int neededSize = 36 * 2 + 8 + 12;
    //glm::vec3 vertexArray[neededSize];
    glm::vec3 vertexArray[92];
    int j = 0;
    bool addV = false;
    for(int i = 0; i < 72; i++)
    {
        addV = !addV;
        
        if(addV)
        {
            vertexArray[i] = glm::vec3(cubeVertices[j],cubeVertices[j+1],cubeVertices[j+2]);
            j = j+3;
            
        } else {
            //set color white
            vertexArray[i] = ORANGE;
        }
    }
    
    j=0;
    for (int i = 72; i < 80; i++)
    {
        vertexArray[i] = groundArray[j];
        j++;
    }
    
    j=0;
    for (int i = 80; i < neededSize; i++)
    {
        vertexArray[i] = crosshairArray[j];
        j++;
    }
   
    
    /*glm::vec3 vertexArrays = {
        vertexArray,
        groundArray,
    };*/
    
    
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
                          2*sizeof(glm::vec3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          2*sizeof(glm::vec3),
                          (void*)sizeof(glm::vec3)      // color is offseted a vec3 (comes after position)
                          );
    glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

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
    //u -- move model left
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        modelTranslation.x += (deltaTime * modelMoveSpeedMult);
    }
    //l -- move model right
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        modelTranslation.x -= (deltaTime * modelMoveSpeedMult);
    }
    //u -- move model up (forward)
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        modelTranslation.z += (deltaTime * modelMoveSpeedMult);
    }  
    //k -- move model down (backward)
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        modelTranslation.z -= (deltaTime * modelMoveSpeedMult);
    }
    //y -- rotate model
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        modelRotations.y += deltaTime * modelRotationSpeedMult;
    }
    //i -- rotate model
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        modelRotations.y -= deltaTime * modelRotationSpeedMult;
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
}

//Draw everything calls other draw functions
void draw(Shader shader, int vao)
{
    shader.use();
    //bind main VAO containing all models and grid
    glBindVertexArray(vao);
    
    GLuint worldMatrixLocation = shader.getUniform("worldMatrix");
    
    drawGround(worldMatrixLocation);
    drawCrosshairs(worldMatrixLocation);
    drawModels(worldMatrixLocation);

}

//Draws the 100*100 grid
void drawGround(int worldLoc)
{
    for(int i = 0; i<100; i++)
    {
        for (int j = 0; j<100; j++)
        {
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),glm::vec3((1.0f*i-50.f), -10.0f, (1.0f*j -50.f)));
            glm::mat4 worldMatrix =  translationMatrix;
            glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
            //TODO: replace magic numbers with constants
            glDrawArrays(GL_LINE_LOOP, 36, 4);
        }
    }
}

//Draw axes crosshairs (RGB axes)
void drawCrosshairs(int worldLoc)
{
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(5.f, 5.f, 5.f));
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, 0.0f));
    glm::mat4 worldMatrix =  translationMatrix * scalingMatrix ;
    glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
    //TODO: replace magic numbers with constants
    glDrawArrays(GL_LINES, 40, 6);
}

//Draws the models
void drawModels(int worldLoc)
{
    switch (modelToDisplay) {
    case 1:
        //parameters: world location as int, vertex array offset, s t r transformations
        Amanda.draw(worldLoc, 0, scale, modelTranslation, modelRotations);
        /*Amanda.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Amanda.xAxis);
        Amanda.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Amanda.yAxis);*/
        Amanda.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Amanda.zAxis);
        break;
    case 2:
        Calvin.draw(worldLoc, 0, scale, modelTranslation, modelRotations);
        /*Calvin.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Calvin.xAxis);
        Calvin.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Calvin.yAxis);*/
        Calvin.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Calvin.zAxis);
        break;
    case 3:
        Charles.draw(worldLoc, 0, scale, modelTranslation, modelRotations);
        /*Charles.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Charles.xAxis);
        Charles.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Charles.yAxis);*/
        Charles.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Charles.zAxis);
        break;
    case 4:
        Dante.draw(worldLoc, 0, scale, modelTranslation, modelRotations);
        /*Dante.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Dante.xAxis);
        Dante.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Dante.yAxis);*/
        Dante.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Dante.zAxis);
        break;
    case 5:
        Yeeho.draw(worldLoc, 0, scale, modelTranslation, modelRotations);
        /*Yeeho.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Yeeho.xAxis);
        Yeeho.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Yeeho.yAxis);*/
        Yeeho.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Yeeho.zAxis);
        break;
    default:
        Amanda.draw(worldLoc, 0, scale, modelTranslation, modelRotations);
        Amanda.drawWall(worldLoc, 0, scale, modelTranslation, modelRotations, Amanda.zAxis);
        Calvin.draw(worldLoc, 0, scale, modelTranslation + glm::vec3(0.0f, 0.0f, -40.0f), modelRotations);
        Calvin.drawWall(worldLoc, 0, scale, modelTranslation + glm::vec3(0.0f, 0.0f, -40.0f), modelRotations, Calvin.zAxis);
        Charles.draw(worldLoc, 0, scale, modelTranslation + glm::vec3(40.0f, 0.0f, 0.0f), modelRotations);
        Charles.drawWall(worldLoc, 0, scale, modelTranslation + glm::vec3(40.0f, 0.0f, 0.0f), modelRotations, Charles.zAxis);
        Dante.draw(worldLoc, 0, scale, modelTranslation + glm::vec3(0.0f, 0.0f, 40.0f), modelRotations);
        Dante.drawWall(worldLoc, 0, scale, modelTranslation + glm::vec3(0.0f, 0.0f, 40.0f), modelRotations, Dante.zAxis);
        Yeeho.draw(worldLoc, 0, scale, modelTranslation + glm::vec3(-40.0f, 0.0f, 0.0f), modelRotations);
        Yeeho.drawWall(worldLoc, 0, scale, modelTranslation + glm::vec3(-40.0f, 0.0f, 0.0f), modelRotations, Yeeho.zAxis);
        break;
    }
}


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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Comp371 - Assignment", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    //initialize shaders
    shader = Shader("vertexShader.glsl", "fragmentShader.glsl");
    //set camera position
    camera = Camera(&shader);


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
    /*glm::vec3 charlesPts[] = {
        glm::vec3(3.0f,0.0f,0.0f),
        glm::vec3(3.0f,0.0f,1.0f),
        glm::vec3(2.0f,0.0f,2.0f),
        glm::vec3(1.0f,0.0f,3.0f),
        glm::vec3(0.0f,0.0f,3.0f),
        glm::vec3(0.0f,1.0f,3.0f),
        glm::vec3(0.0f,2.0f,2.0f),
        glm::vec3(0.0f,3.0f,1.0f),
        glm::vec3(0.0f,3.0f,0.0f)
    };*/
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
        
        // Draw geometry
        draw(shader, vao);
        
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
