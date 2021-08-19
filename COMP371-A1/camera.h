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

	Camera(float width, float height, glm::vec3 defaultPosition, glm::vec3 defaultFront, glm::vec3 defaultUp) {
		position = defaultPosition;
		front = glm::normalize(defaultFront);
		up = glm::normalize(defaultUp);
		worldUp = up;
		right = glm::normalize(glm::cross(front, worldUp));
		//yaw = DEFAULT_YAW;
		yaw = std::atan2f(-front.z, front.x) * (180.0 / 3.141592653589793238463);
		//pitch = DEFAULT_PITCH;
		//pitch = std::atan2(std::sqrt(std::pow(front.z, 2) + std::pow(front.x, 2)) ,-front.y) * (180.0 / 3.141592653589793238463);
		pitch = std::asin(front.y) * (180.0 / 3.141592653589793238463);
		fov = DEFAULT_FOV;
		this->width = width;
		this->height = height;
	}

	Camera(float width, float height) {
		position = DEFAULT_POS;
		front = DEFAULT_FRONT;
		up = DEFAULT_UP;
		worldUp = up;
		right = glm::normalize(glm::cross(front, worldUp));
		//yaw = DEFAULT_YAW;
		yaw = std::atan2f(-front.z, front.x) * (180.0 / 3.141592653589793238463);
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
	//Update camera view based on settings
	//========================================================
	void updateProjectionViewMatrices() {
		viewMatrix = glm::lookAt(position, position + front, up);
		projectionMatrix = glm::perspective(glm::radians(fov),  // field of view in degrees
			width / height,      // aspect ratio
			0.1f, 100.0f);       // near and far (near > 0)
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
