#include "Shader.h"
#include "Core.h"

char Shader::log[log_size];

Shader::Shader() {}
Shader::Shader(const std::string& name) : name(name)
{
	uint32_t vertex_shader{create(true)}, fragment_shader{create(false)};

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex_shader);
	glAttachShader(ID, fragment_shader);
	glLinkProgram(ID);

	//Check for compilation errors
	int32_t success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ID, log_size, nullptr, log);
		throw Core::Exception{"Could not link the \""+name+"\" shader program. Log: \n"+log};
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(fragment_shader);
	glDeleteShader(vertex_shader);
}

uint32_t Shader::create(bool vertex)
{
	//Compile
	const uint32_t shader{glCreateShader(vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER)};
	const std::string data{Core::readFile("Resources/Shaders/"+name+(vertex ? ".vert" : ".frag"))};
	const char *converted_data{data.c_str()};
	glShaderSource(shader, 1, &converted_data, nullptr);
	glCompileShader(shader);

	//Error checking
	int32_t success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(shader, log_size, nullptr, log);
		throw Core::Exception{"Could not compile the \""+name+"\" "+
			(vertex ? "vertex" : "fragment")+" shader. Log: \n"+log};
	}

	return shader;
}

// activate the shader
// ------------------------------------------------------------------------
void Shader::use()
{
	glUseProgram(ID);
}
// utility uniform functions
	// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setDouble(const std::string &name, double value) const
{
	glUniform1d(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2Float(const std::string &name, float value[2]) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

void Shader::draw(DrawType_e drawingFormat, uint32_t primitiveType, uint32_t drawCount)
{
	if (drawingFormat)
	{
		glDrawElements(primitiveType, drawCount, GL_UNSIGNED_INT, 0);
	}
	else
	{
		glDrawArrays(primitiveType, 0, drawCount);
	}
}