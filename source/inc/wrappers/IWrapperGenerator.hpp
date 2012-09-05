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

#define GLR_WRAPPER_GENERATOR_NAME	"__glr_wrapper_generator"

#include <data/BlockExtension.hpp>

#include <COLLADA/Element.hpp>

#include "../GLrEngine.hpp"

namespace GLr {

namespace graph { class ElementNode; }

namespace rendering { class Renderer; }

namespace wrappers {

class GLrExport IWrapperGenerator : public Kore::data::BlockExtension
{
public:
	IWrapperGenerator(const QString& name)
	: 	BlockExtension(name, GLR_WRAPPER_GENERATOR_NAME)
	{
	}

	virtual ~IWrapperGenerator() { }

	virtual kint probeScore(GLr::rendering::Renderer* renderer) const = K_NULL;

	virtual GLr::graph::ElementNode* createWrapper(GLr::rendering::Renderer* renderer, Nigel::COLLADA::Element* element) const = K_NULL;
};

}}

#define GLR_GENERATOR_ARG_RENDERER	renderer
#define GLR_GENERATOR_ARG_ELEMENT	element

#define GLR_GENERATOR private:\
	class Generator : public GLr::wrappers::IWrapperGenerator\
	{\
	public:\
		Generator();\
		virtual ~Generator();\
		virtual kint probeScore(GLr::rendering::Renderer*) const;\
		virtual GLr::graph::ElementNode* createWrapper(GLr::rendering::Renderer*, Nigel::COLLADA::Element*) const;\
	};\
	static Generator __GLr_Generator;

#define GLR_GENERATOR_I_BEGIN
#define GLR_GENERATOR_I_BEGIN_REGISTRATION		GLR_GENERATOR_TYPE::Generator::Generator() : GLr::wrappers::IWrapperGenerator(K_BLOCK_XSTR(GLR_GENERATOR_TYPE)) {
#define GLR_GENERATOR_I_REGISTER( element )		K_BLOCK_EXTENSION_REGISTER( element )
#define GLR_GENERATOR_I_END_REGISTRATION		} GLR_GENERATOR_TYPE::Generator::~Generator() {}

#define GLR_GENERATOR_I_PROBE_SCORE( code ) kint GLR_GENERATOR_TYPE::Generator::probeScore(GLr::rendering::Renderer* GLR_GENERATOR_ARG_RENDERER) const\
	{ return ( code ); }

#define GLR_GENERATOR_I_GENERATE_WRAPPER( code ) GLr::graph::ElementNode* GLR_GENERATOR_TYPE::Generator::createWrapper(GLr::rendering::Renderer* GLR_GENERATOR_ARG_RENDERER, Nigel::COLLADA::Element* GLR_GENERATOR_ARG_ELEMENT) const\
	{ code }

#define GLR_GENERATOR_I_END GLR_GENERATOR_TYPE::Generator GLR_GENERATOR_TYPE::__GLr_Generator;

// Efficient
#define GLR_CHECK_GL_EXT( x ) (*(const GLboolean*)&(GLR_GENERATOR_ARG_RENDERER->__GLEW_ ## x))

// Slower, based on string parsing !
#define GLR_CHECK_GL_EXTS( x ) ( glewContextIsSupported( GLR_GENERATOR_ARG_RENDERER , x ) )
