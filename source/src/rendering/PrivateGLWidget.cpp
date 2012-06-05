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

#include <rendering/PrivateGLWidget.hpp>
using namespace GLr::rendering;

PrivateGLWidget::PrivateGLWidget(Renderer* renderer, const QGLFormat& format)
:	QGLWidget(format),
 	IRendererObject(renderer)
{
	// Initialize the private widget.
	glInit();

	glEnable(GL_DEPTH_TEST);

	// To disappear with the renderer !
	connect(renderer, SIGNAL(blockDeleted()), SLOT(deleteLater()));
}

PrivateGLWidget::~PrivateGLWidget()
{
	qDebug("GLr / Cleaning up OpenGL context");
}

void PrivateGLWidget::initializeGL()
{
	const GLenum err = glewInit(); // This will initialize the renderer's context.
	if(GLEW_OK == err)
	{
		qDebug("GLr / Successfully initialized GLEW version %s", glewGetString(GLEW_VERSION));
		qDebug("GLr / OpenGL Vendor: %s", glGetString(GL_VENDOR));
		qDebug("GLr / OpenGL Renderer: %s", glGetString(GL_RENDERER));
		qDebug("GLr / OpenGL Version: %s", glGetString(GL_VERSION));
		qDebug("GLr / OpenGL Profile: %s", format().testOption(QGL::NoDeprecatedFunctions) ? "Core" : "Compatibility");
		qDebug("GLr / GLSL Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	else
	{
		qWarning("GLr / Failed to initialize GLEW with error: %s", glewGetErrorString(err));
	}
}
