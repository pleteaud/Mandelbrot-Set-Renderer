#include "Framebuffer.h"

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &ID);
	/* Maybe alter this constructor a bit more */
}

void Framebuffer::bindFBO(int fboType)
{
	glBindFramebuffer(fboType, ID);
}

void Framebuffer::unbindFBO(int fboType)
{
	glBindFramebuffer(fboType, 0);
}

void Framebuffer::attachTexture(int textID, int textAttachment, int textTarget)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, textAttachment, textTarget, textID, 0);
}

void Framebuffer::attachRenderBuffer(int rboID, int rboAttachment, int rboTarget)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, rboAttachment, rboTarget, rboID);
}

void Framebuffer::checkFBOStatus()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
}

Framebuffer::~Framebuffer()
{
}
