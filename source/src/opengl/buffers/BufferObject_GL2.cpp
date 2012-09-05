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

#include <opengl/buffers/BufferObject_GL2.hpp>
using namespace GLr::opengl::buffers;
using namespace GLr::rendering;

BufferObject_GL2::BufferObject_GL2(Renderer* renderer)
:	IBufferObject(renderer)
{
	glGenBuffers(1, &_name);
}

BufferObject_GL2::~BufferObject_GL2()
{
	glDeleteBuffers(1, &_name);
}

void BufferObject_GL2::create(GLsizeiptr size, GLenum usage, const GLvoid* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, _name);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void BufferObject_GL2::update(GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, _name);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BufferObject_GL2::bind(GLenum target)
{
	glBindBuffer(target, _name);
}

void BufferObject_GL2::unbind(GLenum target)
{
	glBindBuffer(target, 0);
}

kvoid* BufferObject_GL2::map(GLenum access)
{
	glBindBuffer(GL_ARRAY_BUFFER_ARB, _name);
	return glMapBuffer(GL_ARRAY_BUFFER_ARB, access);
}

void BufferObject_GL2::unmap()
{
	glUnmapBuffer(GL_ARRAY_BUFFER_ARB);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint BufferObject_GL2::name() const
{
	return _name;
}

GLsizei BufferObject_GL2::size() const
{
	GLsizei result;
	glBindBuffer(GL_ARRAY_BUFFER, _name);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &result);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return result;
}
