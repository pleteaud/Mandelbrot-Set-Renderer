#include "Renderbuffer.h"



Renderbuffer::Renderbuffer(int width, int height,uint32_t internalFormat)
{
	bindRBO(GL_RENDERBUFFER);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	unbindRBO(GL_RENDERBUFFER);
}

void Renderbuffer::bindRBO(int rboType)
{
	glBindRenderbuffer(rboType, ID);
}

void Renderbuffer::unbindRBO(int rboType)
{
	glBindRenderbuffer(rboType, 0);
}


Renderbuffer::~Renderbuffer()
{
}
