#ifndef SHADER_H
#define SHADER_H

#include <GL3W/gl3w.h>
#include <Gl3W/wglext.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

enum DrawType_e{ARRAY,ELEMENT};
class Shader
{
public:
	unsigned int ID;
	Shader();
	// constructor generates the shader on the fly
	// ------------------------------------------------------------------------
	Shader(const std::string& name);
	void use();
	// utility uniform functions 
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2Float(const std::string &name, float value[2]) const;
	void draw(DrawType_e drawingFormat, uint32_t primitiveType, uint32_t drawCount);
	void setDouble(const std::string &name, double value) const;

private:
	static constexpr uint16_t log_size{512};
	static char log[log_size];
	std::string name;

	uint32_t create(bool vertex);
};
#endif