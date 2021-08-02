//  COMP 371 Assignment 2 team Last Minute Formed

#ifndef camera_h
#define camera_h

#include <GL/glew.h> // include glew to get all the required OpenGL headers
#include "shader.h"

const float panConstant = 5.f;
const float rotationSpeedMult = 25.f;

class Camera
{
public:
    // Shaders
    Shader* shader;
    Shader* lightShader;
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    float yaw;      //left right
    float pitch;    //up down
    
    //movement and zoom controls
    float speed;
    float zoom;
    
    GLuint viewMatrixLocation;
    GLuint projectionMatrixLocation;
    GLuint viewLight;
    GLuint projectionLight;
    GLuint viewPosLight;

    //camera pan
    bool firstLeftMouse;
    bool firstRightMouse;
    double lastMousePosX;
    double lastMousePosY;
    
    //window size
    float width;
    float height;
    
    //Default constructor
    Camera(){}
    
    Camera(Shader* shader, Shader* lightShader, float width, float height)
    {
        this->shader = shader;
        this->lightShader = lightShader;
        position = glm::vec3(0.0f, 0.0f, -10.0f);
        front = glm::vec3(0.0f,0.0f,1.0f);
        up = glm::vec3(0.0f,1.0f,0.0f);
        worldUp = up;
        right = glm::normalize(glm::cross(front, worldUp));
        yaw = -90.f;
        pitch = 0.0f;
        speed=7.5f;
        zoom = 75.0f;
        viewMatrixLocation = shader->getUniform("viewMatrix");
        projectionMatrixLocation = shader->getUniform("projectionMatrix");
        viewPosLight = shader->getUniform("viewPos");
        viewLight = lightShader->getUniform("viewMatrix");
        projectionLight = lightShader->getUniform("projectionMatrix");
        firstLeftMouse = true;
        firstRightMouse = true;
        lastMousePosX = 0;
        lastMousePosY = 0;
        this->width = width;
        this->height = height;
    }
    
    void moveLeft(float deltaTime)
    {
        position -= right * speed * deltaTime;
    }
    
    void moveRight(float deltaTime)
    {
        position += right * speed * deltaTime;
    }
    
    void moveForward(float deltaTime)
    {
        position += front * speed * deltaTime;
    }
    
    void moveBack(float deltaTime)
    {
        position -= front * speed * deltaTime;
    }
    
    void rotateAboutCenter(float deltaTime) {
        //calculate angular positions relative to object to get polar coordinate
        float angle = glm::degrees(atan2(position.z, position.x));
        //change angle by deltaTime to move camera
        angle += (deltaTime * rotationSpeedMult);
        //distance from center
        float distance = sqrt(pow(position.z, 2) + pow(position.x, 2));
        //calculate new position
        position = glm::vec3(distance * cos(glm::radians(angle)), position.y, distance * sin(glm::radians(angle)));
        //point camera at object
        yaw = -angle + 180;
        //update vectors
        float radYaw = glm::radians(yaw);
        float radPitch = glm::radians(pitch);
        front = glm::normalize(glm::vec3(cos(radPitch) * cos(radYaw), sin(radPitch), -cos(radPitch) * sin(radYaw)));
        right = glm::normalize(glm::cross(front, worldUp));
    }
    
    void panCamera(float deltaTime, double mousePosX, double mousePosY) {

        //reset lastMousePos when we first click since the mouse may have moved while not clicked
        if (firstRightMouse) {
            lastMousePosX = mousePosX;
            lastMousePosY = mousePosY;
            firstRightMouse = false;
        }

        //Find difference from last pos
        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;

        //Set last to current
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;

        // Convert to spherical coordinates
        const float cameraAngularSpeed = 45.0f;
        yaw -= dx * cameraAngularSpeed * deltaTime;
        pitch -= dy * cameraAngularSpeed * deltaTime;

        // Clamp vertical angle to [-89, 89] degrees
        pitch = std::max(-89.0f, std::min(89.0f, pitch));
        //Reset horizontal angle values
        if (yaw > 360) {
            yaw -= 360;
        }
        else if (yaw < -360) {
            yaw += 360;
        }

        float radYaw = glm::radians(yaw);
        float radPitch = glm::radians(pitch);

        //Set front facing vector based on yaw and pitch angles
        front = glm::normalize(glm::vec3(cos(radPitch) * cos(radYaw), sin(radPitch), -cos(radPitch) * sin(radYaw)));
        right = glm::normalize(glm::cross(front, worldUp));
    }

    void zoomCamera(float mousePosY) {
        //Reset on first click
        if (firstLeftMouse) {
            lastMousePosY = mousePosY;
            firstLeftMouse = false;
        }
        double dy = mousePosY - lastMousePosY;
        lastMousePosY = mousePosY;

        //use difference in previous postion to change fov angle
        //clamp to 1 to 150. FOV angle affects zoom
        zoom += (dy / 3);
        if (zoom < 1) {
            zoom = 1;
        }
        if (zoom > 150) {
            zoom = 150;
        }
    }
    
    //Update camera view based on settings
    void updateCam()
    {
        shader->use();
        //set matrix in both shaders
        glm::mat4 viewMatrix = glm::lookAt(position, position + front, up);
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
       
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(zoom),  // field of view in degrees
                                                      width / height,      // aspect ratio
                                                      0.1f, 100.0f);       // near and far (near > 0)
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
        
        //make sure view position updated.
        glUniform3fv(viewPosLight, 1, &position[0]);
        
        lightShader->use();
        glUniformMatrix4fv(viewLight, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(projectionLight, 1, GL_FALSE, &projectionMatrix[0][0]);
        
        
    }
    
    //Move camera position relative to world space
    void changePosition(glm::vec3 newPosition) {
        this->position = newPosition;
    }

    //Resets the camera to the default position it starts in with default zoom
    void reset() {
        position = glm::vec3(0.0f, 0.0f, -10.0f);
        front = glm::vec3(0.0f, 0.0f, 1.0f);
        up = glm::vec3(0.0f, 1.0f, 0.0f);
        worldUp = up;
        right = glm::normalize(glm::cross(front, worldUp));
        yaw = -90.f;
        pitch = 0.0f;
        zoom = 75.0f;
    }
    
    void setWH(float width, float height)
    {
        this->width = width;
        this->height = height;
    }

};

#endif /* camera_h */
