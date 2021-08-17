//
// COMP 371 PrOJeCt asSiGnMenT by team LastMinuteFormed
//
// Built on in-class Lab Framework
// And built on ressources from https://learnopengl.com
//

#ifndef vao_h
#define vao_h

//TODO: cleanup constants
namespace vao_consts {
	const int numCubeVertices = 36;
	const int numVertexAttributes = 4;
	const int vertexSize = (3 * 3 + 2); //11 floats
}

GLuint createCubeVAO(glm::vec3 color) {
	//vertices (with normal)
	float cubeVertices[] = {
		//position           //norm vector
		//Back Face (neg z)
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		//Front face (pos z)
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
		//Left face (neg x)
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		//Right Face (pos x)
		0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
		//Bottom Face (neg y)
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
		//Top face (pos y)
		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
	};
	//note third coordinate not used, implemented as vec3 to simplify holding all info in 1 vertexArray of vec3
	float textureCoords[] =
	{
		//back face
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f, 
		1.0f, 1.0f, 
		0.0f, 0.0f, 
		0.0f, 1.0f, 
		//front face
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		//left face
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		//right face
		1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		//bottom face
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		//top face
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
	};

	//36 vertices each containing x number of floats where x = stride
	float vertexArray[vao_consts::numCubeVertices * vao_consts::vertexSize];
	//populate the vertex array with the vertex data: position, normal vector, color and texture coordinate
	for (int i = 0; i < vao_consts::numCubeVertices; i++) {
		//copy the position and normal vector into vertex array from the cube vertices array
		int offset;
		for (offset = 0; offset < 6; offset++) {
			vertexArray[i * vao_consts::vertexSize + offset] = cubeVertices[i * 6 + offset];
		}
		//copy in the colour
		vertexArray[i * vao_consts::vertexSize + offset++] = color.x;
		vertexArray[i * vao_consts::vertexSize + offset++] = color.y;
		vertexArray[i * vao_consts::vertexSize + offset++] = color.z;
		//copy in texture coordinate
		vertexArray[i * vao_consts::vertexSize + offset++] = textureCoords[i * 2];
		vertexArray[i * vao_consts::vertexSize + offset++] = textureCoords[i * 2 + 1];
	}

	GLuint cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	// fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
	// link vertex attributes
	glBindVertexArray(cubeVAO);

	//Vertex position
	glVertexAttribPointer(0,
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		vao_consts::vertexSize * sizeof(float), // stride - each vertex contains "X" size of certain data
		(void*)0             // offset
	);
	glEnableVertexAttribArray(0);
	//Normal Vector
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vao_consts::vertexSize * sizeof(float), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(1);
	//RGB Color
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vao_consts::vertexSize * sizeof(float), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);
	//Texture coordinate
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, vao_consts::vertexSize * sizeof(float), (void*)(3 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(3);

	//Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return cubeVAO;
}

#endif

GLuint createCubeVAO() {
	return createCubeVAO(WHITE);
}
