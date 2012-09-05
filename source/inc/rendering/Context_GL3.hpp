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

#include "IContext.hpp"

#include <rendering/MatrixStack.hpp>

namespace GLr { namespace rendering {

class Context_GL3_Root;

class Context_GL3 : public IContext
{
	friend class Context_GL3_Root;

private:
	Context_GL3(Renderer*, Nigel::rendering::MatrixStack&);

	inline Context_GL3(const GLr::graph::Node* node, Context_GL3& parent, kint nodeIndex)
	:	IContext(node, parent, nodeIndex),
		_matrixStack(parent._matrixStack)
	{
		_matrixStack.push();
	}

public:
	inline ~Context_GL3()
	{
		_matrixStack.pop();
	}

	virtual void renderScene(GLr::wrappers::scene::WVisualScene* scene, ICamera* camera);

	// Transformation stuff...
	inline const Nigel::rendering::MatrixStack& matrixStack() const { return _matrixStack; }

	virtual void translate(const Nigel::math::Vector4f& direction);
	virtual void rotate(const Nigel::math::Quaternionf& orientation);
	virtual void scale(const Nigel::math::Vector4f& weights);
	virtual void transform(const Nigel::math::Matrix4x4f& matrix);

private:
	void visitNode(Context_GL3& context, GLr::graph::Node* node);

protected:
	// Matrix stack
	Nigel::rendering::MatrixStack& _matrixStack;

};

class Context_GL3_Root : public IContext
{
public:
	Context_GL3_Root(Renderer*);

	virtual void renderScene(GLr::wrappers::scene::WVisualScene* scene, ICamera* camera);

	virtual void translate(const Nigel::math::Vector4f& direction);
	virtual void rotate(const Nigel::math::Quaternionf& orientation);
	virtual void scale(const Nigel::math::Vector4f& weights);
	virtual void transform(const Nigel::math::Matrix4x4f& matrix);

private:
	Nigel::rendering::MatrixStack _stack;
};

}}
