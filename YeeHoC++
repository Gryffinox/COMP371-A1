////
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//

#include <iostream>
#include <list>

//#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>


using namespace glm;
using namespace std;


class Projectile
{
public:
    Projectile(vec3 position, vec3 velocity, int shaderProgram) : mPosition(position), mVelocity(velocity)
    {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    }

    void Update(float dt)
    {
        mPosition += mVelocity * dt;
    }

    void Draw() {
        // this is a bit of a shortcut, since we have a single vbo, it is already bound
        // let's just set the world matrix in the vertex shader

        mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

public:
    GLuint mWorldMatrixLocation;
    vec3 mPosition;
    vec3 mVelocity;
};

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        "uniform mat4 orientationMatrix = mat4(1.0);"
        "out vec3 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix *orientationMatrix *  worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}

const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
        "}";
}

int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int createVertexArrayObject()
{
    // Cube model
    vec3 vertexArray[] = {  // position,                            color
        vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f),	 //Grid - Yellow
        vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f),
        vec3(1.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.0f, 1.0f), vec3(1.0f, 1.0f, 0.0f),

        vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), // green +Y
        vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), // red +X
        vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), // blue +Z
        vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, 1.0f),

        vec3(-0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f), //left - red
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.65f,0.65f,0.65f),

        vec3(-0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),// far - blue
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.65f,0.65f,0.65f), // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f,0.0f,1.0f), // near - green
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f,0.0f,1.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f,0.0f,1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f,0.0f,1.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f,0.0f,1.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f,0.0f,1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.65f,0.65f,0.65f), // right - purple
        vec3(0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),

        vec3(0.5f,-0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.65f,0.65f,0.65f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.65f,0.65f,0.65f), // top - yellow
        vec3(0.5f, 0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f, 0.5f,-0.5f),vec3(0.65f,0.65f,0.65f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.65f,0.65f,0.65f),
        vec3(-0.5f, 0.5f, 0.5f),vec3(0.65f,0.65f,0.65f),
    };


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
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
}

void drawAxis(GLint worldMatrixLocation)
{

    mat4 lineMatrix;
    lineMatrix = translate(mat4(1.0f), vec3(0.0f, 0.1f, 0.0f)) * scale(mat4(5.0f), vec3(1.3f, 5.0f, 1.3f));
    lineMatrix = translate(mat4(1.0f), vec3(0.0f, 0.1f, 0.0f)) * scale(mat4(5.0f), vec3(1.3f, 7.0f, 1.3f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineMatrix[0][0]);
    glDrawArrays(GL_LINES, 4, 2);
    lineMatrix = translate(mat4(1.0f), vec3(0.0f, 0.1f, 0.0f)) * scale(mat4(5.0f), vec3(5.0f, 1.3f, 1.3f));
    lineMatrix = translate(mat4(1.0f), vec3(0.0f, 0.1f, 0.0f)) * scale(mat4(5.0f), vec3(7.0f, 1.3f, 1.3f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineMatrix[0][0]);
    glDrawArrays(GL_LINES, 6, 2);
    lineMatrix = translate(mat4(1.0f), vec3(0.0f, 0.1f, 0.0f)) * scale(mat4(5.0f), vec3(1.3f, 1.3f, 5.0f));
    lineMatrix = translate(mat4(1.0f), vec3(0.0f, 0.1f, 0.0f)) * scale(mat4(5.0f), vec3(1.3f, 1.3f, 7.0f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineMatrix[0][0]);
    glDrawArrays(GL_LINES, 8, 2);
}

void drawGrid(GLint worldMatrixLocation)
{
    mat4 groundWorldMatrix;
    for (int i = -64; i < 64; i++) {
        for (int j = -64; j < 64; j++) {
            groundWorldMatrix = translate(mat4(1.0f), vec3(1.0f * j, 0.0f, 1.0f * i));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        }
    }
}

const int Width = 1024;                 //Set window width and heights
const int Height = 768;
const int CubeVertexLocation = 10;

//Tool variables
float worldX = 0.0f;                   //Rotation angle toward X axis.
float worldY = 0.0f;                   //Rotation angle toward Y axis.

float zoom_size0 = 1.0;                //Scale coefficients of different models, because they are distinct, one scale won't influence others.
float zoom_size1 = 1.0;
float zoom_size2 = 1.0;
float zoom_size3 = 1.0;
float zoom_size4 = 1.0;

float Angle0 = 0.0f;                   //Rotation coefficients of different models, because they are distinct, one rotation won't influence others.
float Angle1 = 0.0f;
float Angle2 = 0.0f;
float Angle3 = 0.0f;
float Angle4 = 0.0f;

bool flag0 = false;                    //Flags are used to record if button are pressed and if other button it won't changed state until pressing home button.
bool flag1 = false;
bool flag2 = false;
bool flag3 = false;
bool flag4 = false;

//Represent world X-Z coordinates of each letter and digits
float coor0[4][2] = { {0.0f,0.0f}};
float coor1[4][2] = { {11.269f,63.0f},{ 5.056f, 63.8f},{-5.056f, 63.8f},{-11.269f, 63.0f} };
float coor2[4][2] = { {-63.0f,11.269f}, {-63.8f,5.056f}, {-63.8f,-5.056f},{-63.0f,-11.269f} };
float coor3[4][2] = { {-11.269f,-63.0f},{-5.056f,-63.8f},{5.056f,-63.8f},{11.269f,-63.0f} };
float coor4[4][2] = { {63.0f,-11.269f},{63.8f,-5.056f},{63.8f,5.056f},{63.0f,11.269f} };

//Status recorded which model is activated, and only one model can be activated at the same time.
bool status[5] = { false,false, false, false, false };


//Initialize render type to triangle, user can change it by pressing button.
int rendertype0 = GL_TRIANGLES;
int rendertype1 = GL_TRIANGLES;
int rendertype2 = GL_TRIANGLES;
int rendertype3 = GL_TRIANGLES;
int rendertype4 = GL_TRIANGLES;

void WalldrawYh(int worldMatrixLocation, GLFWwindow* window, bool status[], float angle) {

    if (status[0]) { flag0 = true; }

    mat4 wallMat;
    mat4 wallSetMatrix = translate(mat4(1.0f), vec3(0, 0, 0)) * rotate(mat4(1.0f), radians(angle), vec3(0, 1.0f, 0));
    mat4 groupMat = translate(mat4(1.0f), vec3(0, 0,0)) * rotate(mat4(1.0f), radians(angle), vec3(0, 1.0f, 0));;
    groupMat = scale(groupMat, vec3(zoom_size0, zoom_size0, zoom_size0));
    mat4 world_mat;

    wallMat = translate(mat4(1.0f), vec3(0.0f, 8.0f, -25.0f)) * scale(mat4(1.0f), vec3(18.0f, 2.0f, 0.0f));
    
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(0.0f, 16.0f, -25.0f)) * scale(mat4(1.0f), vec3(18.0f, 2.0f, 0.0f));

    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(-8.0f, 12.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 10.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(8.0f, 12.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 10.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(6.0f, 14.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(6.0f, 10.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(-6.0f, 14.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(-6.0f, 10.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(2.0f, 14.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(2.0f, 10.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(-2.0f, 14.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(-2.0f, 10.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(0.0f, 12.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(4.0f, 12.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    wallMat = translate(mat4(1.0f), vec3(-4.0f, 12.0f, -25.0f)) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 0.0f));
    world_mat = groupMat * wallSetMatrix * wallMat;

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &world_mat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);
}

    

void drawYeeho(int worldMatrixLocation, GLFWwindow* window, float coor[4][2], bool status[], float angle) {
    //
    if (status[0]) { flag0 = true; }

    mat4 cubeMat;
    mat4 CubeWorldMatrix = translate(mat4(1.0f), vec3(coor[0][0], 0, 0)) * rotate(mat4(1.0f), radians(angle), vec3(0, 1.0f, 0));;
    mat4 groupMat = translate(mat4(1.0f), vec3(0, 0, coor[0][1])) * rotate(mat4(1.0f), radians(angle), vec3(0, 1.0f, 0));;
    groupMat = scale(groupMat, vec3(zoom_size0, zoom_size0, zoom_size0));
    mat4 worldMat;

    mat4 rotations = rotate(mat4(1.0f), radians(0.0f), vec3(0, 1.0f, 0));
    mat4 scales = scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
    mat4 trans1 = translate(mat4(1.0f), vec3(0.0f, 10.0f, 0.0f));
    cubeMat = rotations * trans1 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    mat4 trans2 = translate(mat4(1.0f), vec3(4.0f, 10.0f, 0.0f));
    cubeMat = rotations * trans2 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    mat4 trans3 = translate(mat4(1.0f), vec3(-4.0f, 10.0f, 0.0f));
    cubeMat = rotations * trans3 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    mat4 trans4 = translate(mat4(1.0f), vec3(-2.0f, 12.0f, 0.0f));
    cubeMat = rotations * trans4 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    mat4 trans5 = translate(mat4(1.0f), vec3(2.0f, 12.0f, 0.0f));
    cubeMat = rotations * trans5 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    mat4 trans6 = translate(mat4(1.0f), vec3(-6.0f, 12.0f, 0.0f));
    cubeMat = rotations * trans6 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    mat4 trans7 = translate(mat4(1.0f), vec3(6.0f, 12.0f, 0.0f));
    cubeMat = rotations * trans7 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    mat4 trans8 = translate(mat4(1.0f), vec3(-4.0f, 14.0f, 0.0f));
    cubeMat = rotations * trans8 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    mat4 trans9 = translate(mat4(1.0f), vec3(4.0f, 14.0f, 0.0f));
    cubeMat = rotations * trans9 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

    mat4 trans10 = translate(mat4(1.0f), vec3(0.0f, 14.0f, 0.0f));
    cubeMat = rotations * trans10 * scales;

    worldMat = groupMat * CubeWorldMatrix * cubeMat;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMat[0][0]);
    glDrawArrays(rendertype0, CubeVertexLocation, 36);

}

GLFWwindow* window = NULL;

int main(int argc, char* argv[])
{
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

    // Create Window and rendering context using GLFW, resolution is 800x600
    window = glfwCreateWindow(800, 600, "Comp371 - Lab 03", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // @TODO 3 - Disable mouse cursor
    // ...
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();

    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);

    // Camera parameters for view transform
    vec3 cameraPosition(0.0f, 2.0f, 15.0f);
    vec3 cameraLookAt(vec3(cosf(0) * cosf(radians(90.0f)), sinf(0), -cosf(0) * sinf(radians(150.0f))));
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Other camera parameters
    float cameraSpeed = 1.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable

    // Spinning cube at camera position
    float spinningCubeAngle = 0.0f;

    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(45.0f,            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
        cameraPosition + cameraLookAt,  // center
        cameraUp); // up

    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    mat4 orientationMatrix = rotate(mat4(1.0f), radians(worldX), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(worldY), vec3(0.0f, 1.0f, 0.0f));
    GLuint worldorientationLocation = glGetUniformLocation(shaderProgram, "orientationMatrix");
    glUniformMatrix4fv(worldorientationLocation, 1, GL_FALSE, &orientationMatrix[0][0]);

    // Define and upload geometry to the GPU here ...
    int vao = createVertexArrayObject();

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
   
    // Other OpenGL states to set once before the Game Loop
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Container for projectiles to be implemented in tutorial
    list<Projectile> projectileList;

    int vbo = createVertexArrayObject();

    // Entering Game Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor

        // @TODO 1 - Clear Depth Buffer Bit as well
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Draw geometry
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        drawAxis(worldMatrixLocation);

        drawGrid(worldMatrixLocation);

        WalldrawYh(worldMatrixLocation,window, status, Angle0);

        drawYeeho(worldMatrixLocation, window, coor0, status, Angle0);

        glBindVertexArray(0);

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
        {
            cameraPosition = { 0.0f, 10.0f, 15.0f };
            cameraLookAt = (vec3(cosf(0) * cosf(radians(90.0f)), sinf(0), -cosf(0) * sinf(radians(150.0f))));
            cameraUp = { 0.0f, 1.0f, 0.0f };

            for (int i = 0; i < 5; i++)
            {
                status[i] = false;
            }

            status[0] = true;
        }



        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
        {
            cameraPosition = { 0.0f, 2.0f, 32.0f };
            cameraLookAt = vec3(cosf(0) * cosf(radians(-90.0f)), sinf(0), -cosf(0) * sinf(radians(-90.0f)));
            cameraUp = { 0.0f, 1.0f, 0.0f };

            for (int i = 0; i < 5; i++)
            {
                status[i] = false;
            }
            status[1] = true;

        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) // move camera down
        {
            cameraPosition = { -32.0f, 2.0f, 0.0f };
            cameraLookAt = vec3(cosf(0) * cosf(radians(180.0f)), sinf(0), -cosf(0) * sinf(radians(180.0f)));
            cameraUp = { 0.0f, 1.0f, 0.0f };

            for (int i = 0; i < 5; i++)
            {
                status[i] = false;
            }
            status[2] = true;
        }

        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) // move camera down
        {
            cameraPosition = { 0.0f, 2.0f, -32.0f };
            cameraLookAt = vec3(cosf(0) * cosf(radians(90.0f)), sinf(0), -cosf(0) * sinf(radians(90.0f)));
            cameraUp = { 0.0f, 1.0f, 0.0f };

            for (int i = 0; i < 5; i++)
            {
                status[i] = false;
            }

            status[3] = true;
        }

        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) // move camera down
        {
            cameraPosition = { 32.0f, 2.0f, 0.0f };
            cameraLookAt = vec3(cosf(0) * cosf(radians(0.0f)), sinf(0), -cosf(0) * sinf(radians(0.0f)));
            cameraUp = { 0.0f, 1.0f, 0.0f };

            for (int i = 0; i < 5; i++)
            {
                status[i] = false;
            }
            status[4] = true;
        }

        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            if (status[0])
            {
                zoom_size0 += 0.01;
            }
            if (status[1])
            {
                zoom_size1 += 0.01;
            }
            if (status[2])
            {
                zoom_size2 += 0.01;
            }
            if (status[3])
            {
                zoom_size3 += 0.01;
            }

            if (status[4])
            {
                zoom_size4 += 0.01;
            }

        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        {
            if (status[0])
            {
                zoom_size0 -= 0.01;
            }
            if (status[1])
            {
                zoom_size1 -= 0.01;
            }
            if (status[2])
            {
                zoom_size2 -= 0.01;
            }
            if (status[3])
            {
                zoom_size3 -= 0.01;
            }

            if (status[4])
            {
                zoom_size4 -= 0.01;
            }


        }
        
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            if (status[0])
            {
                rendertype0 = GL_POINTS;
            }
            if (status[1])
            {
                rendertype1 = GL_POINTS;
            }
            if (status[2])
            {
                rendertype2 = GL_POINTS;
            }
            if (status[3])
            {
                rendertype3 = GL_POINTS;
            }

            if (status[4])
            {
                rendertype4 = GL_POINTS;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            if (status[0])
            {
                rendertype0 = GL_LINE_LOOP;
            }
            if (status[1])
            {
                rendertype1 = GL_LINE_LOOP;
            }
            if (status[2])
            {
                rendertype2 = GL_LINE_LOOP;
            }
            if (status[3])
            {
                rendertype3 = GL_LINE_LOOP;
            }

            if (status[4])
            {
                rendertype4 = GL_LINE_LOOP;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            if (status[0])
            {
                rendertype0 = GL_TRIANGLES;
            }
            if (status[1])
            {
                rendertype1 = GL_TRIANGLES;
            }
            if (status[2])
            {
                rendertype2 = GL_TRIANGLES;
            }
            if (status[3])
            {
                rendertype3 = GL_TRIANGLES;
            }

            if (status[4])
            {
                rendertype4 = GL_TRIANGLES;
            }
        }

        // This was solution for Lab02 - Moving camera exercise
        // We'll change this to be a first or third person camera
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


        // @TODO 4 - Calculate mouse motion dx and dy
        //         - Update camera horizontal and vertical angle


        // Please understand the code when you un-comment it!
        
        double mousePosX, mousePosY;
        glfwGetCursorPos(window, &mousePosX, &mousePosY);

        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 60.0f;
        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;

        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f, std::min(85.0f, cameraVerticalAngle));
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }

        float theta = radians(cameraHorizontalAngle);
        float phi = radians(cameraVerticalAngle);

        cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
        vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

        glm::normalize(cameraSideVector);
        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
        {
            cameraPosition = { 0.0f, 2.0f, 30.0f };
            cameraLookAt = (vec3(cosf(0) * cosf(radians(90.0f)), sinf(0), -cosf(0) * sinf(radians(150.0f))));

            worldX = 0;
            worldY = 0;
            orientationMatrix = mat4(1.0f);
            rendertype0 = GL_TRIANGLES;
            rendertype1 = GL_TRIANGLES;
            rendertype2 = GL_TRIANGLES;
            rendertype3 = GL_TRIANGLES;
            rendertype4 = GL_TRIANGLES;

            zoom_size0 = 1.00;
            Angle0 = 0.0f;
            flag0 = false;
            coor0[0][0] = 0.0f;
            coor0[0][1] = 0.0f;
            coor0[1][0] = -3.5f;
            coor0[1][1] = 0.0f;
            coor0[2][0] = 3.813f;
            coor0[2][1] = 0.0f;
            coor0[3][0] = 8.426f;
            coor0[3][1] = 0.0f;


            zoom_size1 = 1.00;
            Angle1 = 0.0f;
            flag1 = false;
            coor1[0][0] = 11.269f;
            coor1[0][1] = 63.0f;
            coor1[1][0] = 5.056f;
            coor1[1][1] = 63.8f;
            coor1[2][0] = -5.056f;
            coor1[2][1] = 63.8f;
            coor1[3][0] = -11.269f;
            coor1[3][1] = 63.0f;

            zoom_size2 = 1.00;
            Angle2 = 0.0f;
            flag2 = false;
            coor2[0][0] = -63.0f;
            coor2[0][1] = 11.269f;
            coor2[1][0] = -63.8f;
            coor2[1][1] = 5.056f;
            coor2[2][0] = -63.8f;
            coor2[2][1] = -5.056f;
            coor2[3][0] = -63.0f;
            coor2[3][1] = -11.269f;


            zoom_size3 = 1.00;
            Angle3 = 0.0f;
            flag3 = false;
            coor3[0][0] = -11.269f;
            coor3[0][1] = -63.0f;
            coor3[1][0] = -5.056f;
            coor3[1][1] = -63.8f;
            coor3[2][0] = 5.056f;
            coor3[2][1] = -63.8f;
            coor3[3][0] = 11.269f;
            coor3[3][1] = -63.0f;


            zoom_size4 = 1.00;
            Angle4 = 0.0f;
            flag4 = false;
            coor4[0][0] = 63.0f;
            coor4[0][1] = -11.269f;
            coor4[1][0] = 63.8f;
            coor4[1][1] = -5.056f;
            coor4[2][0] = 63.8f;
            coor4[2][1] = 5.056f;
            coor4[3][0] = 63.0f;
            coor4[3][1] = 11.269f;


        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            if (status[0])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle0 += 5 * dt;
                }
                else {

                    coor0[0][1] -= 0.1f;
                    coor0[1][1] -= 0.1f;
                    coor0[2][1] -= 0.1f;
                    coor0[3][1] -= 0.1f;
                }

            }
            if (status[1])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle1 += 5 * dt;
                }
                else {

                    coor1[0][0] += 0.1f;
                    coor1[1][0] += 0.1f;
                    coor1[2][0] += 0.1f;
                    coor1[3][0] += 0.1f;
                }

            }
            if (status[2])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle2 += 5 * dt;
                }
                else {

                    coor2[0][1] += 0.1f;
                    coor2[1][1] += 0.1f;
                    coor2[2][1] += 0.1f;
                    coor2[3][1] += 0.1f;
                }

            }
            if (status[3])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle3 += 5 * dt;
                }
                else {

                    coor3[0][0] -= 0.1f;
                    coor3[1][0] -= 0.1f;
                    coor3[2][0] -= 0.1f;
                    coor3[3][0] -= 0.1f;
                }

            }

            if (status[4])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle4 += 5 * dt;
                }
                else {

                    coor4[0][1] -= 0.1f;
                    coor4[1][1] -= 0.1f;
                    coor4[2][1] -= 0.1f;
                    coor4[3][1] -= 0.1f;
                }

            }
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            if (status[0])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle0 -= 5 * dt;
                }
                else {

                    coor0[0][1] += 0.1f;
                    coor0[1][1] += 0.1f;
                    coor0[2][1] += 0.1f;
                    coor0[3][1] += 0.1f;
                }

            }
            if (status[1])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle1 -= 5 * dt;
                }
                else {

                    coor1[0][0] -= 0.1f;
                    coor1[1][0] -= 0.1f;
                    coor1[2][0] -= 0.1f;
                    coor1[3][0] -= 0.1f;
                }

            }
            if (status[2])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle2 -= 5 * dt;
                }
                else {

                    coor2[0][1] -= 0.1f;
                    coor2[1][1] -= 0.1f;
                    coor2[2][1] -= 0.1f;
                    coor2[3][1] -= 0.1f;
                }

            }
            if (status[3])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle3 -= 5 * dt;
                }
                else {

                    coor3[0][0] += 0.1f;
                    coor3[1][0] += 0.1f;
                    coor3[2][0] += 0.1f;
                    coor3[3][0] += 0.1f;
                }

            }

            if (status[4])
            {
                if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
                {
                    Angle4 -= 5 * dt;
                }
                else {

                    coor4[0][1] += 0.1f;
                    coor4[1][1] += 0.1f;
                    coor4[2][1] += 0.1f;
                    coor4[3][1] += 0.1f;
                }

            }
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            if (status[0])
            {
                coor0[0][0] += 0.1f;
                coor0[1][0] += 0.1f;
                coor0[2][0] += 0.1f;
                coor0[3][0] += 0.1f;
            }
            if (status[1])
            {
                coor1[0][1] += 0.1f;
                coor1[1][1] += 0.1f;
                coor1[2][1] += 0.1f;
                coor1[3][1] += 0.1f;
            }
            if (status[2])
            {
                coor2[0][0] -= 0.1f;
                coor2[1][0] -= 0.1f;
                coor2[2][0] -= 0.1f;
                coor2[3][0] -= 0.1f;
            }
            if (status[3])
            {
                coor3[0][1] -= 0.1f;
                coor3[1][1] -= 0.1f;
                coor3[2][1] -= 0.1f;
                coor3[3][1] -= 0.1f;
            }

            if (status[4])
            {
                coor4[0][0] += 0.1f;
                coor4[1][0] += 0.1f;
                coor4[2][0] += 0.1f;
                coor4[3][0] += 0.1f;

            }
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            if (status[0])
            {
                coor0[0][0] -= 0.1f;
                coor0[1][0] -= 0.1f;
                coor0[2][0] -= 0.1f;
                coor0[3][0] -= 0.1f;
            }
            if (status[1])
            {
                coor1[0][1] -= 0.1f;
                coor1[1][1] -= 0.1f;
                coor1[2][1] -= 0.1f;
                coor1[3][1] -= 0.1f;
            }
            if (status[2])
            {
                coor2[0][0] += 0.1f;
                coor2[1][0] += 0.1f;
                coor2[2][0] += 0.1f;
                coor2[3][0] += 0.1f;
            }
            if (status[3])
            {
                coor3[0][1] += 0.1f;
                coor3[1][1] += 0.1f;
                coor3[2][1] += 0.1f;
                coor3[3][1] += 0.1f;
            }

            if (status[4])
            {
                coor4[0][0] -= 0.1f;
                coor4[1][0] -= 0.1f;
                coor4[2][0] -= 0.1f;
                coor4[3][0] -= 0.1f;

            }
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            double posX, posY;
            glfwGetCursorPos(window, &posX, &posY);

            float deltaY = posY - lastMousePosY;

            lastMousePosY = posY;

            cameraPosition.x += deltaY * 0.01;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double posX, posY;
            glfwGetCursorPos(window, &posX, &posY);

            float deltaX = posX - lastMousePosX;

            lastMousePosX = posX;

            cameraPosition.z += deltaX * 0.01;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
            double posX, posY;
            glfwGetCursorPos(window, &posX, &posY);

            float deltaY = posY - lastMousePosY;

            lastMousePosY = posY;

            cameraPosition.y += deltaY * 0.01;
        }
        
        if ((glfwGetKey(window, GLFW_KEY_LEFT)) == GLFW_PRESS)
        {
            worldX -= 0.5f;
            orientationMatrix = rotate(mat4(1.0f), radians(worldX), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(worldY), vec3(0.0f, 1.0f, 0.0f));
        }
        if ((glfwGetKey(window, GLFW_KEY_RIGHT)) == GLFW_PRESS)
        {
            worldX += 0.5f;
            orientationMatrix = rotate(mat4(1.0f), radians(worldX), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(worldY), vec3(0.0f, 1.0f, 0.0f));
        }
        if ((glfwGetKey(window, GLFW_KEY_UP)) == GLFW_PRESS)
        {
            worldY += 0.5f;
            orientationMatrix = rotate(mat4(1.0f), radians(worldX), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(worldY), vec3(0.0f, 1.0f, 0.0f));
        }
        if ((glfwGetKey(window, GLFW_KEY_DOWN)) == GLFW_PRESS)
        {
            worldY -= 0.5f;
            orientationMatrix = rotate(mat4(1.0f), radians(worldX), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), radians(worldY), vec3(0.0f, 1.0f, 0.0f));
        }

        worldorientationLocation = glGetUniformLocation(shaderProgram, "orientationMatrix");
        glUniformMatrix4fv(worldorientationLocation, 1, GL_FALSE, &orientationMatrix[0][0]);


        mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);


    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}


