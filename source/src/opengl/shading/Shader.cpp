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
 *	DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 *	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <GL/glew.h>

#include <opengl/shading/Shader.hpp>
using namespace GLr::opengl::shading;
using namespace GLr::rendering;

#include <Types.hpp>

Shader::Shader(Renderer* renderer, Shader::Type type)
:	IRendererObject(renderer),
 	_shader(glCreateShader((GLenum)type))
{
	K_ASSERT( glIsShader(_shader) )
}

Shader::~Shader()
{
	glDeleteShader(_shader);
}

void Shader::source(const QString& src)
{
	QByteArray srcData = src.toLocal8Bit();

	const GLchar* srcString[] = { srcData.data(), };

	const GLint srcLength[] = { srcData.length(), };

	glShaderSource(_shader, 1, srcString, srcLength);
}

void Shader::source(const QStringList& srcs)
{
	source(srcs.join("\n"));
}

kbool Shader::compile()
{
	glCompileShader(_shader);

	GLint compileStatus[1];
	glGetShaderiv(_shader, GL_COMPILE_STATUS, compileStatus);

	return GL_TRUE == compileStatus[0];
}

QString Shader::infoLog() const
{
	GLint infoLogLength[1];
	glGetShaderiv(_shader, GL_INFO_LOG_LENGTH, infoLogLength);

	if(infoLogLength[0] == 0)
	{
		return QString();
	}

	QByteArray infoLog(infoLogLength[0], 0x00);
	glGetShaderInfoLog(_shader, infoLog.length(), NULL, infoLog.data());

	return QString::fromLocal8Bit(infoLog.data(), infoLog.length());
}
