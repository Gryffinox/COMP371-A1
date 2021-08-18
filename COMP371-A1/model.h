//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//


//TODO: make constructor for wall to put wall in separate object

#ifndef Model_h
#define Model_h

class Model {
private:
	//our models are built explicitly with cubes, and our cube vao is built with triangles giving 36 vertices
	static const float SHUFFLE_SPEED;       //static const initalized at the bottom of the file
	static const float WALL_BORDER_THICKNESS;      //how many cubes border the edge of the wall
	static const float WALL_OFFSET;       //how far does the wall generate from the model by default

	glm::vec3* positions;                  //Array of centered coordinates of the model
	glm::vec3* positions_default;          //Array of original coordinates of the model
	glm::vec3 center;                      //Center of model

	glm::vec3* wallX;                       //array of the wall model on the x axis
	glm::vec3* wallY;                       //array on y axis
	glm::vec3* wallZ;                       //array on z axis

	//counters for the number of cubes in the models
	int numCubes;
	int numCubesWallX;
	int numCubesWallY;
	int numCubesWallZ;

public:
	static const int NUM_CUBE_VERTICES = 36;
	//transformations on the model
	glm::vec3 modelTranslation;
	glm::vec3 modelRotation;
	glm::vec3 wallTranslation;
	glm::vec3 wallRotation;
	float modelScale;

	//For selecting which wall to draw
	static enum Axes { xAxis = 0, yAxis = 1, zAxis = 2 };

	//========================================================
	//Default constructor
	//========================================================
	Model() {
		//initialize private members
		positions = new glm::vec3;
		positions_default = new glm::vec3;
		center = glm::vec3{ 0.0f, 0.0f, 0.0f };

		wallX = new glm::vec3;
		wallY = new glm::vec3;
		wallZ = new glm::vec3;

		numCubes = 0;
		numCubesWallX = 0;
		numCubesWallY = 0;
		numCubesWallZ = 0;

		//initialize public members
		modelTranslation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		modelRotation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		wallTranslation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		wallRotation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		modelScale = 1;
	}

	//TODO: why is destructor being called when object is not going out of scope
	~Model() {
		/*delete[] positions_;
		delete[] positions_default_;
		delete[] wall_x_;
		delete[] wall_y_;
		delete[] wall_z_;*/
	}

	//========================================================
	//filepath constructor
	//========================================================
	Model(const char* filepath) {
		//initialize variables
		center = glm::vec3{ 0.0f, 0.0f, 0.0f };
		//transformations
		modelTranslation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		modelRotation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		wallTranslation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		wallRotation = glm::vec3{ 0.0f, 0.0f, 0.0f };
		modelScale = 1;
		//Wall Stuff
		numCubesWallX = 0;
		numCubesWallY = 0;
		numCubesWallZ = 0;

		//read in file and count cubes
		std::string fileContent = "";
		std::ifstream modelFilestream;
		// ensure ifstream objects can throw exceptions:

		// open files to read
		modelFilestream.open(filepath);
		//get line count
		numCubes = 0;
		std::string line;
		while (std::getline(modelFilestream, line)) {
			++numCubes;
			fileContent += line;
		}
		// read file's buffer contents into stream
		// close file handlers
		modelFilestream.close();

		//Set size of arrays based on number of cubes read in input
		positions = new glm::vec3[numCubes];
		positions_default = new glm::vec3[numCubes];

		float x = 0, y = 0, z = 0;
		size_t end;
		std::string token = fileContent;

		//parse string to set positions
		//glm::vec3* original_model = new glm::vec3[num_cubes_];
		//string contains coordinates delimited by comma, every 3 values represent xyz coords of one cube
		for (int i = 0; i < numCubes; i++) {
			end = token.find(",");					//find next comma
			x = std::stof(token.substr(0, end));	//take substr until comma and convert to float and store
			token = token.substr(end + 1);			//set token equal to new substring without the first value of string 
			//repeat for y and z
			end = token.find(",");
			y = std::stof(token.substr(0, end));
			token = token.substr(end + 1);
			end = token.find(",");
			z = std::stof(token.substr(0, end));
			token = token.substr(end + 1);
			//set the vector for a single cube in the array of vectors
			positions[i] = glm::vec3(x, y, z);
			positions_default[i] = glm::vec3(x, y, z);
		}

		setModelData();

	}

	void setModelData() {
		//===========================================================================
		//Calculate model center and recenter model
		//===========================================================================
		//variables to keep track of min max in each axis. used to center model (and build wall)
		float maxX = positions[0].x;
		float maxY = positions[0].y;
		float maxZ = positions[0].z;
		float minX = positions[0].x;
		float minY = positions[0].y;
		float minZ = positions[0].z;

		//Calculate the physical center of the model based on the cubes on the outer most edge of each dimension
		for (int i = 0; i < numCubes; i++) {
			//Find min in each dimension, store it
			if (positions[i].x > maxX)
				maxX = positions[i].x;
			if (positions[i].y > maxY)
				maxY = positions[i].y;
			if (positions[i].z > maxZ)
				maxZ = positions[i].z;
			//Find max in each dimension, store it
			if (positions[i].x < minX)
				minX = positions[i].x;
			if (positions[i].y < minY)
				minY = positions[i].y;
			if (positions[i].z < minZ)
				minZ = positions[i].z;
		}

		//Calculate average point (middle) from min/max
		float xHalfRange, yHalfRange, zHalfRange;
		xHalfRange = (maxX - minX) / 2;
		yHalfRange = (maxY - minY) / 2;
		zHalfRange = (maxZ - minZ) / 2;

		//Store center position as vector
		center = glm::vec3((minX + xHalfRange), (minY + yHalfRange), (minZ + zHalfRange));

		//===========================================================================
		//procedurally generate a wall model that comes with the model for all 3 axes
		//===========================================================================
		//each wall model is slightly bigger than the original model
		//thus the wall on X axis is of (y + kEdgeThickness)*(z + kEdgeThickness) dimension.
		//same thing for Y and Z axis walls.
		numCubesWallX = ((maxY - minY) + (WALL_BORDER_THICKNESS * 2 + 1)) * ((maxZ - minZ) + (WALL_BORDER_THICKNESS * 2 + 1));
		numCubesWallY = ((maxX - minX) + (WALL_BORDER_THICKNESS * 2 + 1)) * ((maxZ - minZ) + (WALL_BORDER_THICKNESS * 2 + 1));
		numCubesWallZ = ((maxX - minX) + (WALL_BORDER_THICKNESS * 2 + 1)) * ((maxY - minY) + (WALL_BORDER_THICKNESS * 2 + 1));
		//set wall model to new array of appropriate size dimension1 * dimension2 size
		wallX = new glm::vec3[numCubesWallX];
		wallY = new glm::vec3[numCubesWallY];
		wallZ = new glm::vec3[numCubesWallZ];
		//for each wall, add a cube to the model
		//X wall
		int counter = 0;
		//runtime is O(d1 * d2 * n) where d1 * d2 is some dimensions pertaining to the model when flat
		//in worse case, d1*d2 is of n*3, and in best case, sqrt(n) so runtime is about O(n^2)
		//traverse along the Y axis
		for (float y = minY - (WALL_BORDER_THICKNESS); y < maxY + (WALL_BORDER_THICKNESS + 1); y++) {
			//traverse along the z axis
			for (float z = minZ - (WALL_BORDER_THICKNESS); z < maxZ + (WALL_BORDER_THICKNESS + 1); z++) {
				//add the cube at (y, z) to the wall model on (x, y) because were placing all 3 walls on the z axis
				wallX[counter++] = glm::vec3(z, y, center.z);
				//if the cube we just added is in the flat model, remove it from the wall model so that there's a hole
				for (int i = 0; i < numCubes; i++) {
					//the (y, z) cube in the original model has been translated into (x, y) in the wall
					if (wallX[counter - 1].y == positions[i].y && wallX[counter - 1].x == positions[i].z) {
						counter--;
						break;
					}
				}
			}
		}
		//counter counted the number of cubes we added to the wall
		numCubesWallX = counter;
		//resize wall array
		glm::vec3* temp = new glm::vec3[numCubesWallX];
		memcpy(temp, wallX, numCubesWallX * sizeof(glm::vec3));
		delete[] wallX;
		wallX = temp;
		//Y wall
		counter = 0;
		for (float x = minX - (WALL_BORDER_THICKNESS); x < maxX + (WALL_BORDER_THICKNESS + 1); x++) {
			for (float z = minZ - (WALL_BORDER_THICKNESS); z < maxZ + (WALL_BORDER_THICKNESS + 1); z++) {
				wallY[counter++] = glm::vec3(x, z, center.z);
				for (int k = 0; k < numCubes; k++) {
					if (wallY[counter - 1].x == positions[k].x && wallY[counter - 1].y == positions[k].z) {
						counter--;
						break;
					}
				}
			}
		}
		numCubesWallY = counter;
		temp = new glm::vec3[numCubesWallY];
		memcpy(temp, wallY, numCubesWallY * sizeof(glm::vec3));
		delete[] wallY;
		wallY = temp;
		//Z wall
		counter = 0;
		for (float x = minX - (WALL_BORDER_THICKNESS); x < maxX + (WALL_BORDER_THICKNESS + 1); x++) {
			for (float y = minY - (WALL_BORDER_THICKNESS); y < maxY + (WALL_BORDER_THICKNESS + 1); y++) {
				wallZ[counter++] = glm::vec3(x, y, center.z);
				for (int i = 0; i < numCubes; i++) {
					if (wallZ[counter - 1].x == positions[i].x && wallZ[counter - 1].y == positions[i].y) {
						counter--;
						break;
					}
				}
			}
		}
		numCubesWallZ = counter;
		temp = new glm::vec3[numCubesWallZ];
		memcpy(temp, wallZ, numCubesWallZ * sizeof(glm::vec3));
		delete[] wallZ;
		wallZ = temp;

		//Modify model coordinates to be relative to it's center point instead
		for (int i = 0; i < numCubes; i++) {
			positions[i] -= center;
			positions_default[i] -= center;
		}
		for (int i = 0; i < numCubesWallX; i++) {
			//wallX[i] -= center;// + glm::vec3(.0f, .0f, zHalfRange + 10.f);
			wallX[i] -= glm::vec3((minZ + zHalfRange), (minY + yHalfRange), (minZ + zHalfRange));
		}
		
		for (int i = 0; i < numCubesWallY; i++) {
			//wallY[i] -= center;// + glm::vec3(.0f, .0f, zHalfRange + 20.f);
			wallY[i] -= glm::vec3((minX + xHalfRange), (minZ + zHalfRange), (minZ + zHalfRange));
		}
		for (int i = 0; i < numCubesWallZ; i++) {
			wallZ[i] -= center;// + glm::vec3(.0f, .0f, zHalfRange + 30.f);
		}
	}

	//========================================================
	//Transformation Functions
	//========================================================
	void scaleUp(float scaleIncrease) {
		modelScale += scaleIncrease;
	}

	void scaleDown(float scaleDecrease) {
		modelScale -= scaleDecrease;
		if (modelScale < 0)
			modelScale = 0;
	}

	void moveLeft(float speed) {
		modelTranslation.x += speed;
	}
	void moveRight(float speed) {
		modelTranslation.x -= speed;
	}
	void moveForward(float speed) {
		modelTranslation.z += speed;
	}
	void moveBackward(float speed) {
		modelTranslation.z -= speed;
	}

	void rotateModel(float angle, int axis) {
		glm::mat4 rotationMatrix;
		switch (axis) {
		case xAxis:
			rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
			for (int i = 0; i < numCubes; i++) {
				positions[i] = glm::vec3(rotationMatrix * glm::vec4(positions[i], 1.0f));
			}
			break;
		case yAxis:
			rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
			for (int i = 0; i < numCubes; i++) {
				positions[i] = glm::vec3(rotationMatrix * glm::vec4(positions[i], 1.0f));
			}
			break;
		case zAxis:
			rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
			for (int i = 0; i < numCubes; i++) {
				positions[i] = glm::vec3(rotationMatrix * glm::vec4(positions[i], 1.0f));
			}
			break;
		default:
			break;
		}
	}

	//========================================================
	//Draw functions
	//========================================================
	//Pass in each transformation as a set of coordinates except for scale which applies to all dimensions
	void draw(Shader shader, int vaoStartIndex, glm::vec3 offset) {
		//set each transformation matrix based on input variables
		//all cubes share scaling, rotation and translation matrices
		glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(modelScale, modelScale, modelScale));
		glm::mat4 rotationMatrix =
			glm::rotate(glm::mat4(1.0f), glm::radians(modelRotation.x), glm::vec3(1.f, .0f, .0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(modelRotation.y), glm::vec3(.0f, 1.f, .0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(modelRotation.z), glm::vec3(.0f, .0f, 1.f)
			);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), modelTranslation + offset);
		glUniformMatrix4fv(shader.getUniform("normalVectorRotation"), 1, GL_FALSE, &rotationMatrix[0][0]);

		int uniformLocation = shader.getUniform("worldMatrix");
		//set transformation matrix for each cube
		for (int i = 0; i < numCubes; i++) {
			//set matrix for position of each cube relative to model
			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), modelScale * (positions[i]));
			//Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
			glm::mat4 transformationMatrix = translationMatrix * rotationMatrix * modelMatrix * scalingMatrix;
			glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &transformationMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, vaoStartIndex, NUM_CUBE_VERTICES); // 36 vertices, starting at index 0
		}
	}

	void draw(Shader shader, int vaoStartIndex) {
		draw(shader, vaoStartIndex, glm::vec3(0, 0, 0));
	}

	//axis is 1, 2 or 3 matching x y or z
	void drawWall(Shader shader, int vaoStartIndex, int axis, glm::vec3 offset) {

		//set each transformation matrix based on input variables
		glm::mat4 scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(modelScale, modelScale, modelScale));
		//glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), coordinates + scale*(positions[i]));
		glm::mat4 rotationMatrix =
			glm::rotate(glm::mat4(1.0f), glm::radians(wallRotation.x), glm::vec3(1.f, .0f, .0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(wallRotation.y), glm::vec3(.0f, 1.f, .0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(wallRotation.z), glm::vec3(.0f, .0f, 1.f)
			);
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), wallTranslation + offset);
		glUniformMatrix4fv(shader.getUniform("normalVectorRotation"), 1, GL_FALSE, &rotationMatrix[0][0]);

		glm::mat4 transformationMatrix;
		int uniformLocation = shader.getUniform("worldMatrix");
		switch (axis) {
		case xAxis:
			for (int i = 0; i < numCubesWallX; i++) {
				glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), modelScale * (wallX[i]));
				//Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
				transformationMatrix = translationMatrix * rotationMatrix * positionMatrix * scalingMatrix;
				glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &transformationMatrix[0][0]);
				glDrawArrays(GL_TRIANGLES, vaoStartIndex, NUM_CUBE_VERTICES); // 36 vertices, starting at index 0
			}
			break;
		case yAxis:
			for (int i = 0; i < numCubesWallY; i++) {
				glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), modelScale * (wallY[i]));
				//Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
				transformationMatrix = translationMatrix * rotationMatrix * positionMatrix * scalingMatrix;
				glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &transformationMatrix[0][0]);
				glDrawArrays(GL_TRIANGLES, vaoStartIndex, NUM_CUBE_VERTICES); // 36 vertices, starting at index 0
			}
			break;
		case zAxis:
			for (int i = 0; i < numCubesWallZ; i++) {
				glm::mat4 positionMatrix = glm::translate(glm::mat4(1.0f), modelScale * (wallZ[i]));
				//Scale first, place all cubes relative to center of model, rotate them on center, then move out to proper place
				transformationMatrix = translationMatrix * rotationMatrix * positionMatrix * scalingMatrix;
				glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &transformationMatrix[0][0]);
				glDrawArrays(GL_TRIANGLES, vaoStartIndex, NUM_CUBE_VERTICES); // 36 vertices, starting at index 0
			}
			break;
		default:
			break;
		}
	}
	void drawWall(Shader shader, int vaoStartIndex, int axis) {
		drawWall(shader, vaoStartIndex, axis, glm::vec3(0, 0, 0));
	}

};

const float Model::SHUFFLE_SPEED = 2.5f;
const float Model::WALL_BORDER_THICKNESS = 1;
const float Model::WALL_OFFSET = 5;

#endif /* Model_h */
