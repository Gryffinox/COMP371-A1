//
//  camera.h
//  Assignment
//
//  Created by Amanda Simard on 2021-07-14.
//  Copyright © 2021 Concordia. All rights reserved.
//

#ifndef camera_h
#define camera_h

#include <GL/glew.h> // include glew to get all the required OpenGL headers
#include "shader.h"

const float panConstant = 5.f;

class Camera
{
public:
    // Shader
    Shader* shader;
    // camera Attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    // euler Angles
    float yaw;
    float pitch;
    
    float speed;
    float zoom;
    
    GLuint viewMatrixLocation;
    GLuint projectionMatrixLocation;
    
    //Default constructor
    Camera(){}
    
    Camera(Shader* shader)
    {
        this->shader = shader;
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
    
    /*void turnRX(float deltaTime)
    {
        
    }
    void turnRnX(float deltaTime)
    {
       
    }
    void turnRY(float deltaTime)
    {
        
    }
    void turnRnY(float deltaTime)
    {
        
    }*/
    
    void panLeft(float deltaTime)
    {
        yaw += panConstant * speed * deltaTime;
        // calculate the new Front vector
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));
    }
    
    void panRight(float deltaTime)
    {
        yaw += panConstant * speed * deltaTime;
        // calculate the new Front vector
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));
    }
    
    void tiltUp(float deltaTime)
    {
        pitch += panConstant * speed * deltaTime;
        // calculate the new Front vector
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));
    }
    void tiltDown(float deltaTime)
    {
        pitch -= panConstant * speed * deltaTime;
        // calculate the new Front vector
        glm::vec3 f;
        f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y = sin(glm::radians(pitch));
        f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(f);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));
    }

    //Set FOV angle. clamp to 1 to 150. FOV angle affects zoom
    void setZoom(float newZoom) {
        if (newZoom < 1) {
            newZoom = 1;
        }
        if (newZoom > 150) {
            newZoom = 150;
        }
        this->zoom = newZoom;
    }
  
    void updateCam()
    {
        glm::mat4 viewMatrix = glm::lookAt(position, position + front, up);
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glm::mat4 projectionMatrix = glm::perspective(glm::radians(zoom),  // field of view in degrees
                                                      1024.f / 768.f,      // aspect ratio
                                                      0.1f, 100.0f);       // near and far (near > 0)
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    }
    
    //Move camera position relative to world space
    void changePosition(glm::vec3 newPosition) {
        this->position = newPosition;
    }

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

};

#endif /* camera_h */
