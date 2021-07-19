//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

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

unsigned int renderMode = GL_FILL;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;    // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void drawGround(int worldLoc);
void drawCrosshairs(int worldLoc);
void drawModels(int worldLoc);

float scale = 1.0f;

Model model;
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

void getInput(GLFWwindow *window, float deltaTime)
{
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    /*
    //press 1 -- focus on 1
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
    }
     
     //press 2 -- focus on 2
     if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
     {
     }
     
     //press 3 -- focus on 3
     if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
     {
     }
     
     //press 4 -- focus on 4
     if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
     {
     }
     
     //press 5 -- focus on 5
     if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
     {
     }
     */
    
    //press u -- scale-up
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        scale += 0.1*2.5*deltaTime;
    }
    
    //press j -- scale-down
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        scale -= 0.1*2.5*deltaTime;
    }
    
    //press W -- move up
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.moveForward(deltaTime);
    }
    
    //press A -- move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.moveLeft(deltaTime);
    }
    
    //press S -- move down
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.moveBack(deltaTime);
    }
    
    //press D -- move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.moveRight(deltaTime);
    }
     
    //press left arrow -- rotate anti-clockwise around x axis
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        camera.turnRX(deltaTime);
    }
    
    //press right arrow -- rotate clockwise around x axis
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        camera.turnRnX(deltaTime);
    }
    
    //press up arrow -- rotate upward around y axis
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        camera.turnRY(deltaTime);
    }
    
    //press down arrow -- rotate downward around y axis
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        camera.turnRnY(deltaTime);
    }
    
    //press P -- display as points
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        renderMode = GL_POINT;
    }
    
    //press L -- display as wireframe
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        renderMode = GL_LINE;
    }
    
    //press T (And F) -- display as filled triangles
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        renderMode = GL_FILL;
    }
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        model.shuffle(deltaTime);
    }
    
    //right mouse -- pan x direction
    
    
    //middle-mouse -- tilt y direction
    
    //left-mouse -- zoom in and out.
   
}

void draw(Shader shader, int vao)
{
    shader.use();
    glBindVertexArray(vao);
    
    GLuint worldMatrixLocation = shader.getUniform("worldMatrix");
    
    drawGround(worldMatrixLocation);
    drawCrosshairs(worldMatrixLocation);
    drawModels(worldMatrixLocation);

}

void drawGround(int worldLoc)
{
    for(int i = 0; i<100; i++)
    {
        for (int j = 0; j<100; j++)
        {
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),glm::vec3((1.0f*i-50.f), -10.0f, (1.0f*j -50.f)));
            glm::mat4 worldMatrix =  translationMatrix;
            glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(GL_LINE_LOOP, 36, 4);
        }
    }
}

void drawCrosshairs(int worldLoc)
{
    glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(5.f, 5.f, 5.f));
    
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, -9.99f, 0.0f));
    glm::mat4 worldMatrix =  translationMatrix * scalingMatrix ;
    glUniformMatrix4fv(worldLoc, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_LINES, 40, 6);

}

void drawModels(int worldLoc)
{
    glPolygonMode(GL_FRONT_AND_BACK, renderMode);
    glm::vec3 center = glm::vec3(0.f,0.f,0.f);
    model.draw(center, worldLoc, 0, scale);
    
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
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
    shader = Shader("vertexShader.vs", "fragmentShader.fs");
    //set camera position
    camera = Camera(&shader);
    model = Model();
    
    

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
