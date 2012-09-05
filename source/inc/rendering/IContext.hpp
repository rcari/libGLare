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

#include <Types.hpp>

#include "IRendererObject.hpp"

#include <math/Matrix4x4f.hpp>
#include <math/Quaternionf.hpp>
#include <math/Vector4f.hpp>

#include "../graph/Node.hpp"

namespace GLr { namespace rendering {

class Renderer;

class GLrExport IContext : public IRendererObject
{
	friend class Renderer;

public:
	IContext(Renderer*);
	virtual ~IContext();

	inline IContext(const GLr::graph::Node* node, IContext& parent, kint nodeIndex)
	:	IRendererObject(parent.renderer()),
		_parentContext(parent),
		_stackDepth(0),
		_currentNodeIndex(nodeIndex),
		_currentNode(node),
		_state(parent._state)
	{
	}

	virtual void renderScene(GLr::wrappers::scene::WVisualScene* scene, ICamera* camera) = K_NULL;

	// Node types stuff...
	inline void enableNodeType(GLr::graph::Node::Type type)
	{
		_state.activeNodeTypes = ( _state.activeNodeTypes & type )
				?	_state.activeNodeTypes ^ type
				:	_state.activeNodeTypes;
	}
	inline void disableNodeType(GLr::graph::Node::Type type) { _state.activeNodeTypes |= type; }

	// Transformation stuff...
	virtual void translate(const Nigel::math::Vector4f& direction) = K_NULL;
	virtual void rotate(const Nigel::math::Quaternionf& orientation) = K_NULL;
	virtual void scale(const Nigel::math::Vector4f& weights) = K_NULL;
	virtual void transform(const Nigel::math::Matrix4x4f& matrix) = K_NULL;

	// Filtering stuff
	inline kuint activeNodeTypes() const { return _state.activeNodeTypes; }

	// Browse rendering stack stuff
	inline const IContext& parentContext() const { return _parentContext; }
	inline const GLr::graph::Node* currentSceneNode() const { return _currentNode; }

	// Shading stuff...
	inline void useProgram(GLuint program)
	{
		_state.currentShaderProgram = program;
		glUseProgram(program);
	}

	inline void restoreProgram()
	{
		useProgram(_parentContext._state.currentShaderProgram);
	}

protected:
	// Pre-visit stuff...
	inline void enterPreVisitNode() { _stackDepth++; }
	inline kint stackDepth() const { return _stackDepth; }

private:
	// Keep track of the parent node's context.
	IContext& _parentContext;

	//! How many nodes were recursively visited since the context was created. Used to find the end of the pre-visit nodes.
	kint _stackDepth;

	kuint _currentNodeIndex; //! Index of the child node in its parent's child list

	const GLr::graph::Node* _currentNode;

	struct PrivateState
	{
		kuint	activeNodeTypes; //! Currently supported node types.
		GLuint	currentShaderProgram; //! Active shader program
		GLuint	currentVertexArrayObject; //! Active array object
	} _state;

};

}}
