#include "Projectile.h"

//GLEW
#include <gl/glew.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

Projectile::Projectile(vec3 position, vec3 velocity, int shaderProgram) : mPosition(position), mVelocity(velocity) {
	mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
}

void Projectile::Update(float dt) {
	mPosition += mVelocity * dt;
}

void Projectile::Draw() {
    // this is a bit of a shortcut, since we have a single vbo, it is already bound
       // let's just set the world matrix in the vertex shader
    mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}