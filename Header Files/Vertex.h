#pragma once
#include <Windows.h>
#include <GL3W/gl3w.h>
#include <GL3W/wglext.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <vector>


enum BindState_e { OFF, ON };
class Vertex
{
public:
	Vertex();
	Vertex(std::vector<float> vertexVec, std::vector<uint32_t> indexVec);
	~Vertex();
	void createVertexBufferAndArrayObjects();
	void useVertexArrObj(enum BindState_e useVAO);
	void setVertices(const std::vector<float> vec);
	void setIndices(const std::vector<uint32_t> vec);
	//unsigned int getVAO() { return VAO; }

private:
	std::vector<float> vertices;
	std::vector<uint32_t> indices;
	// ID to store buffers and array objects
	unsigned int VBO;
	unsigned int VAO;
	//ID to store Element Buffer Object--> object that stores/ holds indices that OpenGL uses to decide what vertices to draw.
	unsigned int EBO;


};
