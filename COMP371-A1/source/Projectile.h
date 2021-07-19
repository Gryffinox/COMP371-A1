#pragma once

//GLEW
#include <gl/glew.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Projectile {
public:
	GLuint mWorldMatrixLocation;
	vec3 mPosition;
	vec3 mVelocity;
	Projectile(vec3 position, vec3 velocity, int shaderProgram);
	void Update(float dt);
	void Draw();
};