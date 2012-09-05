/*
 * 	Copyright (c) 2010-2011, Romuald CARI
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *		* Redistributions of source code must retain the above copyright
 *		  notice, this list of conditions and the following disclaimer.
 *		* Redistributions in binary form must reproduce the above copyright
 *		  notice, this list of conditions and the following disclaimer in the
 *		  documentation and/or other materials provided with the distribution.
 *		* Neither the name of the <organization> nor the
 *		  names of its contributors may be used to endorse or promote products
 *		  derived from this software without specific prior written permission.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL Romuald CARI BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <opengl/buffers/BufferObject.hpp>
using namespace GLr::opengl::buffers;
using namespace GLr::rendering;

BufferObject::BufferObject(Renderer* renderer)
:	IRendererObject(renderer),
 	_target(0)
{
	glGenBuffersARB(1, &_name);
}

BufferObject::~BufferObject()
{
	glDeleteBuffersARB(1, &_name);
}

void BufferObject::create(GLsizeiptr size, GLenum usage, const GLvoid* data)
{
	bind(GL_ARRAY_BUFFER_ARB);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size, data, usage);
	unbind();
}

void BufferObject::update(GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
	bind(GL_ARRAY_BUFFER_ARB);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, size, data);
	unbind();
}

void BufferObject::bind(GLenum target)
{
	_target = target;
	glBindBufferARB(_target, _name);
}

void BufferObject::unbind()
{
	K_ASSERT( _target != 0 ) // Not unbinding an unbound buffer
	glBindBuffer(_target, 0);
	_target = 0;
}

kvoid* BufferObject::map(GLenum access)
{
	bind(GL_ARRAY_BUFFER_ARB);
	return glMapBufferARB(GL_ARRAY_BUFFER_ARB, access);
}

void BufferObject::unmap()
{
	glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);
	unbind();
}

GLsizei BufferObject::size() const
{
	GLsizei result;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _name);
	glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &result);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	return result;
}

GLenum BufferObject::usage() const
{
	GLint result;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, _name);
	glGetBufferParameteriv(GL_ARRAY_BUFFER_ARB, GL_BUFFER_USAGE_ARB, &result);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	return result;
}
