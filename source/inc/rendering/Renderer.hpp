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

#include <GL/glew.h>

#include <GLrExport.hpp>

#include <data/Library.hpp>
#include <COLLADA/Element.hpp>

#include <QtCore/QMutex>
#include <QtOpenGL/QGLWidget>

namespace GLr {

namespace graph {
class Node;
class ElementNode;
}

namespace opengl {
class GLWidget;
}

namespace wrappers { namespace scene {
class WVisualScene;
}}

namespace rendering {

class IContext;
class ICamera;
class PrivateGLWidget;

class GLrExport Renderer
:	public Kore::data::Library,
 	public GLEWContext
{
	Q_OBJECT

	Q_PROPERTY( QString GLVendor READ GLVendor STORED false DESIGNABLE true )
	Q_PROPERTY( QString GLRenderer READ GLRenderer STORED false DESIGNABLE true )
	Q_PROPERTY( QString GLVersion READ GLVersion STORED false DESIGNABLE true )
	Q_PROPERTY( QString GLSLVersion READ GLSLVersion STORED false DESIGNABLE true )
	Q_PROPERTY( QString GLExtensions READ GLExtensions STORED false DESIGNABLE true )

	K_BLOCK

	friend class GLr::opengl::GLWidget;

private:
	Renderer();

public:
	virtual ~Renderer();

	void renderScene(GLr::wrappers::scene::WVisualScene* scene, ICamera* camera);

	/*!
	 * Will return the Wrapper for the given element, or create one if necessary.
	 * Return can be NULL if no appropriate wrapper could be generated !
	 */
	GLr::graph::ElementNode* getWrapperForElement(Nigel::COLLADA::Element* element);

	/*!
	 * The renderer has a private GL context shared with all GL views contexts.
	 * The private context can be used to perform asynchronous operations such as texture loading, shader compiling,
	 * buffer filling and other task that can be performed in the background to keep the UI responsive.
	 */
	void makeGLContextCurrent();
	void doneGLContextCurrent();

	const QString& GLVendor() const;
	const QString& GLRenderer() const;
	const QString& GLVersion() const;
	const QString& GLExtensions() const;
	const QString& GLSLVersion() const;

	const QString& runtimeID() const;

private:
	PrivateGLWidget*	_GLWidget;
	QMutex				_GLWidgetMutex;

	IContext*			_context;

	QString _GLVendor;
	QString _GLRenderer;
	QString _GLVersion;
	QString _GLSLVersion;
	QString _GLExtensions;

	QString _runtimeID;

};

}}
