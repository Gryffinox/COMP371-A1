#ifndef GLEW_STATIC
#define GLEW_STATIC 1
#endif // !GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


void drawCalvin(int worldMatrixLocation, GLFWwindow* window, float coor[4][2], bool status[], float angle) {
	//rotation
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	//translation
	glm::mat4 translationMatrix;

	//worldMatric
	glm::mat4 worldMatrix;

	//point of the ship
	//cube 1
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//v of the ship
	//cube 2
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 2.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 3
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 2.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 4
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 5
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 1.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 6
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 7
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//dome of the ship
	//cube 8
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 0.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 9
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, 0.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//bottom of the ship
	//cube 10
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 1.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 11
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//the shaft that goes all the way down
	//cube 12
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 13
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 14
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//cube 15
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//additional cubes because fuck symmetry
	//cube 16
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, -1.0f, 0.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	//cube 17
	translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
	worldMatrix = rotationMatrix * translationMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}