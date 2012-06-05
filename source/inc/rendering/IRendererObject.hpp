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

#pragma once

#include "Renderer.hpp"

#include <GLrExport.hpp>

namespace GLr { namespace rendering {

/*!
 * All classes that require access to OpenGL must derive from this class.
 * This is due to the way GLEW handles creating multiple rendering contexts.
 * This class defines a glewGetContext() method that is used by GLEW when using
 * multiple contexts. Moreover it grants quick access to the renderer's instance.
 */
class GLrExport IRendererObject
{
protected:
	inline IRendererObject(Renderer* renderer) : _renderer(renderer) {}

public:
	inline Renderer* renderer() { return _renderer; }
	inline const Renderer* renderer() const { return _renderer; }

protected:
	// For GLEW !
	inline GLEWContext* glewGetContext() const { return _renderer; }

private:
	Renderer* _renderer;
};

class GLrExport RendererLocker
{
public:
	inline RendererLocker(Renderer* renderer)
	:	_renderer(K_NULL)
	{
		// Only lock the renderer's context if there is no current OpenGL context...
		if(!QGLContext::currentContext())
		{
			_renderer = renderer;
			_renderer->makeGLContextCurrent();
		}
	}
	inline ~RendererLocker()
	{
		if(_renderer)
		{
			_renderer->doneGLContextCurrent();
		}
	}

private:
	Renderer* _renderer;
};

}}
