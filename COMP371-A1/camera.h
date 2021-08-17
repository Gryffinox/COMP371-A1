//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//

#ifndef camera_h
#define camera_h

#include "shader.h"

class Camera {
private:
	static const float PAN_CONSTANT;
	static const float ROTATION_SPEED_MULT;
	static const glm::vec3 DEFAULT_POS;
	static const glm::vec3 DEFAULT_FRONT;
	static const glm::vec3 DEFAULT_UP;
	static const float DEFAULT_YAW;
	static const float DEFAULT_PITCH;
	static const float DEFAULT_FOV;
	static const float SPEED;
	static const float CAMERA_ANGULAR_SPEED;

	//Camera projection-view-model matrices
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	// euler Angles
	float yaw;      //left right
	float pitch;    //up down

	//movement and zoom controls
	float fov;

	//window size
	float width;
	float height;

	// camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
public:

	//========================================================
	//Default constructor
	//========================================================
	Camera() {}

	Camera(float width, float height) {
		position = DEFAULT_POS;
		front = DEFAULT_FRONT;
		up = DEFAULT_UP;
		worldUp = up;
		right = glm::normalize(glm::cross(front, worldUp));
		yaw = DEFAULT_YAW;
		pitch = DEFAULT_PITCH;
		fov = DEFAULT_FOV;
		this->width = width;
		this->height = height;
	}

	//========================================================
	//Getters and Setters
	//========================================================
	glm::mat4 getViewMatrix() {
		return viewMatrix;
	}
	glm::mat4 getProjectionMatrix() {
		return projectionMatrix;
	}
	glm::vec3 getPosition() {
		return position;
	}


	//========================================================
	//Camera movement
	//========================================================
	void moveLeft(float deltaTime) {
		position -= right * SPEED * deltaTime;
	}

	void moveRight(float deltaTime) {
		position += right * SPEED * deltaTime;
	}

	void moveForward(float deltaTime) {
		position += front * SPEED * deltaTime;
	}

	void moveBack(float deltaTime) {
		position -= front * SPEED * deltaTime;
	}

	//Move camera position relative to world space
	void changePosition(glm::vec3 newPosition) {
		this->position = newPosition;
	}

	//========================================================
	//Camera orientation
	//========================================================
	void panCamera(double dx, double dy) {

		// Convert to spherical coordinates
		yaw -= dx * CAMERA_ANGULAR_SPEED;
		pitch -= dy * CAMERA_ANGULAR_SPEED;

		lookAt(yaw, pitch);
	}

	//change FOV
	void zoomCamera(float dy) {

		//use difference in previous postion to change fov angle
		//clamp to 1 to 150. FOV angle affects zoom
		fov += (dy * SPEED);
		if (fov < 1) {
			fov = 1;
		}
		if (fov > 150) {
			fov = 150;
		}
	}

	//make camera look somewhere
	void lookAt(float inYaw, float inPitch) {
		yaw = inYaw;
		pitch = inPitch;
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

	//========================================================
	//Changing position and look orientation
	//========================================================
	void rotateAboutCenter(float deltaTime) {
		//calculate angular positions relative to object to get polar coordinate
		float angle = glm::degrees(atan2(position.z, position.x));
		//change angle by deltaTime to move camera
		angle += (deltaTime * ROTATION_SPEED_MULT);
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

	//========================================================
	//Update camera view based on settings
	//========================================================
	void updateProjectionViewMatrices() {
		//shader->use();
		//set matrix in both shaders
		//glm::mat4 viewMatrix = glm::lookAt(position, position + front, up);
		viewMatrix = glm::lookAt(position, position + front, up);
		//glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

		//glm::mat4 projectionMatrix = glm::perspective(glm::radians(fov),  // field of view in degrees
		//	width / height,      // aspect ratio
		//	0.1f, 100.0f);       // near and far (near > 0)
		projectionMatrix = glm::perspective(glm::radians(fov),  // field of view in degrees
			width / height,      // aspect ratio
			0.1f, 100.0f);       // near and far (near > 0)

		//glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

		//make sure view position updated.
		//glUniform3fv(viewPosLight, 1, &position[0]);

		//lightShader->use();
		//glUniformMatrix4fv(viewLight, 1, GL_FALSE, &viewMatrix[0][0]);
		//glUniformMatrix4fv(projectionLight, 1, GL_FALSE, &projectionMatrix[0][0]);
	}

	//========================================================
	//Utility
	//========================================================
	//Resets the camera to the default position it starts in with default zoom
	void reset() {
		position = DEFAULT_POS;
		front = DEFAULT_FRONT;
		up = DEFAULT_UP;
		worldUp = up;
		right = glm::normalize(glm::cross(front, worldUp));
		yaw = DEFAULT_YAW;
		pitch = DEFAULT_PITCH;
		fov = DEFAULT_FOV;
	}

	void setViewportDimensions(float width, float height) {
		this->width = width;
		this->height = height;
	}

};

const float Camera::PAN_CONSTANT = 5.f;
const float Camera::ROTATION_SPEED_MULT = 25.f;
const glm::vec3 Camera::DEFAULT_POS = glm::vec3(0.0f, 0.0f, -10.0f);
const glm::vec3 Camera::DEFAULT_FRONT = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 Camera::DEFAULT_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const float Camera::DEFAULT_YAW = -90.f;
const float Camera::DEFAULT_PITCH = .0f;
const float Camera::DEFAULT_FOV = 75.f;
const float Camera::SPEED = 7.5f;
const float Camera::CAMERA_ANGULAR_SPEED = 45.f;

#endif /* camera_h */
