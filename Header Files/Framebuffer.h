#pragma once
#include <string>
#include <GL3W/gl3w.h>
#include <GL3W/wglext.h>
#include <iostream>

class Framebuffer
{
	
private:
	unsigned int ID;

public:
	Framebuffer();
	void bindFBO(int fboType);
	void unbindFBO(int fboType);
	void attachTexture(int textID, int textAttachment, int textTarget);
	void attachRenderBuffer(int rboID, int rboAttachment, int rboTarget);
	void checkFBOStatus();
	~Framebuffer();
};