#pragma once
#include <string>
#include <GL3W/gl3w.h>
#include <GL3W/wglext.h>
#include <iostream>
class Renderbuffer
{
public:
	Renderbuffer(int width, int height, uint32_t internalFormat);
	void bindRBO(int rboType);
	void unbindRBO(int rboType);
	~Renderbuffer();
private:
	unsigned int ID;
};

