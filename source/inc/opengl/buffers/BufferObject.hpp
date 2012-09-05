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

#pragma once

#include "../../rendering/IRendererObject.hpp"

namespace GLr { namespace opengl { namespace buffers {

/*
 * OpenGL buffer object.
 * Depends on the ARB_vertex_buffer_object extension.
 */
class BufferObject : public GLr::rendering::IRendererObject
{
public:
	BufferObject(GLr::rendering::Renderer* renderer);
	virtual ~BufferObject();

	inline GLuint name() const { return _name; }

	inline GLboolean isBuffer() const { return glIsBufferARB(_name); }

	virtual void bind(GLenum target);
	void unbind();

	void create(GLsizeiptr size, GLenum usage, const GLvoid* data = K_NULL);
	void update(GLintptr offset, GLsizeiptr size, const GLvoid* data);

	kvoid* map(GLenum access);
	void unmap();

	GLsizei size() const;

	GLenum usage() const;

private:
	GLuint _name;
	GLenum _target;
};

}}}
