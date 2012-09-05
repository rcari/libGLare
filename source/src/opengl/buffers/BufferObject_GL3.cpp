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

#include <opengl/buffers/BufferObject_GL3.hpp>
using namespace GLr::opengl::buffers;
using namespace GLr::rendering;

BufferObject_GL3::BufferObject_GL3(Renderer* renderer)
:	BufferObject(renderer)
{
}

void BufferObject_GL3::copyTo(BufferObject& buffer, GLsizeiptr size, GLintptr readOffset, GLintptr writeOffset) const
{
	copyBuffer(name(), buffer.name(), size, false, readOffset, writeOffset);
}


kvoid* BufferObject_GL3::mapRange(GLintptr offset, GLsizeiptr size, GLbitfield access)
{
	bind(GL_ARRAY_BUFFER_ARB);
	return glMapBufferRange(GL_ARRAY_BUFFER_ARB, offset, size, access);
}

void BufferObject_GL3::copyBuffer(
		GLuint src, GLuint dest,
		GLsizeiptr size,
		kbool create,
		GLintptr readOffset, GLintptr writeOffset,
		GLenum usage
	) const
{
	// Bind the source buffer
	glBindBuffer(GL_COPY_READ_BUFFER, src);

	if(size == -1)
	{
		// Retrieve the buffer size
		GLint sizei;
		glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &sizei);
		size = sizei;
	}

	// Bind the destination buffer
	glBindBuffer(GL_ARRAY_BUFFER, dest);

	if(create)
	{
		// Allocate memory for the buffer object
		glBufferData(GL_ARRAY_BUFFER, size, K_NULL, usage);

		if(usage == 0)
		{
			// Retrieve the buffer usage
			glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_USAGE, (GLint*)&usage);
		}
	}

	// Copy between the buffers in graphics memory ideally !
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, readOffset, writeOffset, size);

	// Unbind the source buffer
	glBindBuffer(GL_COPY_READ_BUFFER, 0);

	// Unbind the destination buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
