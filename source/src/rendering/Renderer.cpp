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

#include <rendering/Renderer.hpp>
#include <rendering/ICamera.hpp>
#include <rendering/IRendererObject.hpp>
#include <rendering/Context_GL2.hpp>
#include <rendering/Context_GL3.hpp>
#include <rendering/PrivateGLWidget.hpp>
using namespace GLr::rendering;

#include <wrappers/IWrapperGenerator.hpp>
using namespace GLr::wrappers;

using namespace Kore::data;
using namespace Nigel::COLLADA;
using namespace Nigel::math;

#include <wrappers/scene/WVisualScene.hpp>
using namespace GLr::wrappers::scene;
using namespace GLr::graph;

#include <QtCore/QMutexLocker>

#define K_BLOCK_TYPE GLr::rendering::Renderer
#include <BlockMacros.hpp>
K_BLOCK_BEGIN
	K_BLOCK_ICON_DEFAULT
	K_BLOCK_ALLOCABLE
	K_BLOCK_PROPERTY_DEFAULT
K_BLOCK_END

Renderer::Renderer()
:	_GLWidget(new PrivateGLWidget(this, QGLFormat())),
 	_GLWidgetMutex(QMutex::Recursive)
{
	addFlag(SystemOwned);

	blockName("GLr OpenGL Renderer");

	_GLVendor = QString::fromLocal8Bit((const char*)glGetString(GL_VENDOR));
	_GLRenderer = QString::fromLocal8Bit((const char*)glGetString(GL_RENDERER));
	_GLVersion = QString::fromLocal8Bit((const char*)glGetString(GL_VERSION));
	_GLSLVersion = QString::fromLocal8Bit((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
	_GLExtensions = QString::fromLocal8Bit((const char*)glGetString(GL_EXTENSIONS));

	_runtimeID.sprintf("__GLR_%p", this);

	/*if(_GLWidget->format().openGLVersionFlags() > QGLFormat::OpenGL_Version_3_0)
	{
		_context = new Context_GL3_Root(this);
	}
	else
	{
		_context = new Context_GL2(this);
	}*/

	_context = new Context_GL2(this);
}

Renderer::~Renderer()
{
	delete _context;
	_context = K_NULL;
}

void Renderer::renderScene(WVisualScene* scene, ICamera* camera)
{
	// We lock the renderer because the context is shared !
	//QMutexLocker locker(mutex());

	// Start the visit.
	_context->renderScene(scene, camera);
}

ElementNode* Renderer::getWrapperForElement(Element* element)
{
	// Lock the renderer and make the context current !
	RendererLocker rendererLock(this);

	QVariant wrapperVariant = element->property(qPrintable(_runtimeID));
	if(wrapperVariant.isValid())
	{
		return static_cast<ElementNode*>(wrapperVariant.value<kvoid*>());
	}

	IWrapperGenerator* generator = K_NULL;
	kint probeScore = -1;

	// Find the generator with the highest probe score !
	QList<BlockExtension*> extensions = element->metaBlock()->blockExtensions(GLR_WRAPPER_GENERATOR_NAME);
	for(kint i = 0; i < extensions.size(); i++)
	{
		IWrapperGenerator* temp = static_cast<IWrapperGenerator*>(extensions.at(i));
		kint tempScore = temp->probeScore(this);
		if(tempScore > probeScore)
		{
			generator = temp;
			probeScore = tempScore;
		}
	}

	if(generator == K_NULL)
	{
		qWarning("GLr / No available wrapper generator for type %s", element->metaObject()->className());
		return K_NULL;
	}

	return generator->createWrapper(this, element);
}

void Renderer::makeGLContextCurrent()
{
	// Due to the multi-threaded calls to this method, we use a mutex.
	_GLWidgetMutex.lock();
	_GLWidget->makeCurrent();
}

void Renderer::doneGLContextCurrent()
{
	_GLWidget->doneCurrent();
	_GLWidgetMutex.unlock();
}

const QString& Renderer::GLVendor() const
{
	return _GLVendor;
}

const QString& Renderer::GLRenderer() const
{
	return _GLRenderer;
}

const QString& Renderer::GLVersion() const
{
	return _GLVersion;
}

const QString& Renderer::GLSLVersion() const
{
	return _GLSLVersion;
}

const QString& Renderer::GLExtensions() const
{
	return _GLExtensions;
}

const QString& Renderer::runtimeID() const
{
	return _runtimeID;
}
