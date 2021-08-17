//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//

#ifndef vertexArrayHandler_h
#define vertexArrayHandler_h

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//vertex start index and count variables
//start indices
int crosshairsIndex;
int groundIndex;

// NOTE ---
// first 5 colors correspond to the 5 model colors
// --------
glm::vec3 colorList[] =
{
	PURPLE_NAVY, // Amanda color
	YELLOW, // Calvin color
	DARK_ORANGE, // Charles color
	FUSCHIA, // Dante color
	LIGHT_GREEN, //Yeeho color
	WHITE
};

//coloredCube index dynamically set based on number of colors in color[]
int numberOfColors = (sizeof(colorList) / sizeof(glm::vec3));
int* colorIndex = new int[numberOfColors];

//counts
int gvCount;
int chvCount;
int cubevCount;

int lightCubeIndex;

// texture variables
unsigned int loadTexture(char const* path, unsigned int* textureID);

unsigned int brickTexture, metalTexture, emissionMap, tileTexture;
bool textured = true;
bool glowing = true;
float glowIntensity = 1;
bool glowIncreasing = false;

//texture variables type
enum Texture {
	BRICK,
	METAL,
	TILE,
	NONE
};


int createVertexArrayObject() {
	//vertices (with normal)
	float cubeVertices[] = {
		//position           //norm vector
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
	};

	//note third coordinate not used, implemented as vec3 to simplify holding all info in 1 vertexArray of vec3
	glm::vec3 textureCoords[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),

		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
	};

	//TODO: figure out ground texture coordinate
	glm::vec3 groundArray[] = {
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		YELLOW,
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(0.0f,0.0f,1.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		YELLOW,
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(1.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		YELLOW,
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(0.0f,0.0f,1.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		YELLOW,
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(1.0f,0.0f,1.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		YELLOW,
		glm::vec3(1.0f, 1.0f, 0.0f),

		glm::vec3(1.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		YELLOW,
		glm::vec3(1.0f, 0.0f, 0.0f),
	};

	//note, texture coordinates are placeholders and are not used.
	glm::vec3 crosshairArray[] = {
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		BLUE,
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(0.0f,0.0f,1.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		BLUE,
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		RED,
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(0.0f,1.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		RED,
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		GREEN,
		glm::vec3(0.0f, 0.0f, 0.0f),

		glm::vec3(1.0f,0.0f,.0f),
		glm::vec3(0.0f,1.0f,0.0f),
		GREEN,
		glm::vec3(0.0f, 0.0f, 0.0f),
	};

	// array size is (cubes: colors * 36 vertices + crosshairs: 6 vertices + ground: 4 vertices) * 4 -- vertice coords, norm vector, color, texture coords
	int numParameters = 4;
	int arraySize = (numberOfColors * 36 + 6 + 6) * numParameters;
	//glm::vec3* vertexArray = new glm::vec3[arraySize];
	glm::vec3 vertexArray[912];

	groundIndex = 0;
	gvCount = 6;
	int istop = gvCount * numParameters;
	int ioffset = 0;
	int j = 0;
	for (int i = 0; i < istop; i++) {
		vertexArray[i + groundIndex] = groundArray[j];
		j++;
	}

	crosshairsIndex = 6;
	chvCount = 6;
	istop = chvCount * numParameters;
	ioffset = crosshairsIndex * numParameters;
	j = 0;
	for (int i = 0; i < istop; i++) {
		vertexArray[i + ioffset] = crosshairArray[j];
		j++;
	}
	//add colored cube for each color in color[]
	cubevCount = 36;
	istop = cubevCount * numParameters;

	for (int kolor = 0; kolor < numberOfColors; kolor++) {
		colorIndex[kolor] = 12 + 36 * kolor;
		ioffset = colorIndex[kolor] * numParameters;

		if (colorList[kolor] == WHITE) {
			lightCubeIndex = colorIndex[kolor];
		}

		j = 0;
		int k = 0;
		for (int i = 0; i < istop; i++) {
			//if location 0 or 1, turn coordinates from cubeVertices into vec3
			if (i % numParameters == 0 || i % numParameters == 1) {
				vertexArray[i + ioffset] = glm::vec3(cubeVertices[j], cubeVertices[j + 1], cubeVertices[j + 2]);
				j = j + 3;

			}
			else {
				//if location 2, set color
				if (i % numParameters == 2) {
					vertexArray[i + ioffset] = colorList[kolor];
				}
				else {
					//if location 3, set texture coords
					vertexArray[i + ioffset] = textureCoords[k++];
				}

			}
		}
	}

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
		numParameters * sizeof(glm::vec3), // stride - each vertex contain 3 vec3 (position, norm, color)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,                   // attribute 1 matches aNorm in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		numParameters * sizeof(glm::vec3),
		(void*)sizeof(glm::vec3)
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2,                   // attribute 2 matches aColor in Vertex Shader
		3,
		GL_FLOAT,
		GL_FALSE,
		numParameters * sizeof(glm::vec3),
		(void*)(2 * sizeof(glm::vec3))
	);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3,                   // attribute 3 matches textCoords in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		numParameters * sizeof(glm::vec3),
		(void*)(3 * sizeof(glm::vec3))
	);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// load textures
	loadTexture("brick.jpg", &brickTexture);
	loadTexture("silver.jpg", &metalTexture);
	loadTexture("glowMap-01.jpg", &emissionMap);
	loadTexture("tile.jpg", &tileTexture);

	return vertexArrayObject;
}

unsigned int loadTexture(char const* path, unsigned int* textureID) {
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}
	stbi_image_free(data);

	return *textureID;
}

#endif /* vertexArrayHandler_h */
