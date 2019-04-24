#include "Vertex.h"
#include <iostream>


Vertex::Vertex()
{
}

Vertex::Vertex(std::vector<float> vertexVec, std::vector<uint32_t> indexVec)
{
	setVertices(vertexVec);
	setIndices(indexVec);
	createVertexBufferAndArrayObjects();
}

Vertex::~Vertex()
{
}

void Vertex::createVertexBufferAndArrayObjects() 
{
	//Generate array object
	//Bind the Vertex Array Object first, then bind and set vertex buffer(s)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Vertex buffer
	//2. Copy our vertices array in a buffer for OpenGL to use
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float),
		vertices.data(), GL_STATIC_DRAW);

	//Element Buffer Object
	//3. Copy our index array in a element buffer for OpenGL to use
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32_t),
		indices.data(), GL_STATIC_DRAW);
	
	//4. Then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

void Vertex::useVertexArrObj(enum BindState_e useVAO)
{
	if (useVAO)
	{
		glBindVertexArray(VAO);
	}
	else
	{
		glBindVertexArray(0);
	}
}

void Vertex::setVertices(const std::vector<float> vec)
{
	vertices = vec;
}
void Vertex::setIndices(const std::vector<uint32_t> vec)
{
	indices = vec;
}
