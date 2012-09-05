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

#include <GL/glew.h>

#include <opengl/shading/Program.hpp>
#include <opengl/shading/Shader.hpp>
using namespace GLr::opengl::shading;

using namespace GLr::rendering;

#include <Types.hpp>

Program::Program(Renderer* renderer)
:	IRendererObject(renderer),
 	_program(glCreateProgram())
{
	K_ASSERT( glIsProgram(_program) )
}

Program::~Program()
{
	glDeleteProgram(_program);
}

GLuint Program::program() const
{
	return _program;
}

void Program::attach(Shader* shader)
{
	glAttachShader(_program, shader->_shader);
}

void Program::detach(Shader* shader)
{
	glDetachShader(_program, shader->_shader);
}

kbool Program::link()
{
	glLinkProgram(_program);

	GLint linkStatus[1];
	glGetProgramiv(_program, GL_LINK_STATUS, linkStatus);
	return GL_TRUE == linkStatus[0];
}

QString Program::infoLog() const
{
	GLint infoLogLength[1];

	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, infoLogLength);

	if(infoLogLength[0] == 0)
	{
		return QString();
	}

	QByteArray infoLog(infoLogLength[0], 0x00);
	glGetProgramInfoLog(_program, infoLog.length(), NULL, infoLog.data());

	return QString::fromLocal8Bit(infoLog.data(), infoLog.length());
}
