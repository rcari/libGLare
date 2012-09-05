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

#include <rendering/Context_GL3.hpp>
#include <rendering/ICamera.hpp>
using namespace GLr::rendering;

#include <wrappers/scene/WVisualScene.hpp>
using namespace GLr::wrappers::scene;

using namespace GLr::graph;

using namespace Nigel::rendering;

Context_GL3::Context_GL3(Renderer* renderer, MatrixStack& stack)
:	IContext(renderer),
 	_matrixStack(stack)
{
	// Setup the transformation matrix stack.
	_matrixStack.reset();
	_matrixStack.push(); // By default, there is nothing on the stack !
	_matrixStack.top().identity();
}

void Context_GL3::renderScene(WVisualScene* scene, ICamera* camera)
{
	/*translate(camera->position() * -1.0f);
	rotate(camera->orientation().inverse());*/
	this->visitNode(*this, scene);
}

void Context_GL3::translate(const Nigel::math::Vector4f& direction)
{
	_matrixStack.translate(direction);
}

void Context_GL3::rotate(const Nigel::math::Quaternionf& orientation)
{
	_matrixStack.rotate(orientation);
}

void Context_GL3::scale(const Nigel::math::Vector4f& weights)
{
	_matrixStack.scale(weights);
}

void Context_GL3::transform(const Nigel::math::Matrix4x4f& matrix)
{
	_matrixStack.applyTransformationMatrix(matrix);
}

void Context_GL3::visitNode(Context_GL3& context, Node* node)
{
	if	(
			// We visited all the pre-visit nodes
			context.stackDepth() == node->preVisitNodesNb() ||
			// The node is not supposed to be rendered according to the current context, no need to "pre-visit"
			!(node->type() & context.activeNodeTypes())
		)
	{
		// Transformation matrix
		glUniformMatrix4fv(
				0, // TODO: Get the uniform location...
				16,
				true,
				context._matrixStack.top().values()
			);

		// Inverse transformation matrix
		glUniformMatrix4fv(
				1, // TODO: Get the uniform location...
				16,
				true,
				context._matrixStack.inverseTop().values()
			);

		// Visit the node if the node type is right !
		if(node->type() & context.activeNodeTypes())
		{
			node->visitNode(context);
		}

		// Visit the sub nodes...
		const kint childrenNb = node->childrenNb();
		Node** childNodes = node->children();
		for(kint i = 0; i < childrenNb; i++)
		{
			Node* childNode = childNodes[i];
			if(childNode->enterNode(context)) // Do some node check such as layers...
			{
				// Push a context for the sub node.
				Context_GL3 ctx(childNode, context, i);
				// Recursively visit the subnode.
				this->visitNode(ctx, childNodes[i]);
			}
		}

		// Cleanup what has to be cleaned up...
		node->leaveNode(context);

		// We are done rendering here.
		return;
	}

	// Do the pre-visists !

	Node* preVisitNode = node->preVisitNodes()[context.stackDepth()];

	// We are visiting a pre-visit node and must update the context (stack depth).
	context.enterPreVisitNode();

	// Only apply the pre-visit effect if necessary...
	if(preVisitNode->enterNode(context))
	{
		// Apply whatever the previsit node has to apply.
		preVisitNode->visitNode(context);

		// Recursive calls to keep everything on the stack !
		this->visitNode(context, node);

		// When unstacked, leave the current node, and cleanup !
		preVisitNode->leaveNode(context);
	}
	else
	{
		// Recursive calls to keep everything on the stack !
		this->visitNode(context, node);
	}
}

Context_GL3_Root::Context_GL3_Root(Renderer* renderer)
:	IContext(renderer)
{
}

void Context_GL3_Root::renderScene(GLr::wrappers::scene::WVisualScene* scene, ICamera* camera)
{
	Context_GL3 ctx(this->renderer(), _stack);
	ctx.renderScene(scene, camera);
}

void Context_GL3_Root::translate(const Nigel::math::Vector4f&)
{
}

void Context_GL3_Root::rotate(const Nigel::math::Quaternionf&)
{
}

void Context_GL3_Root::scale(const Nigel::math::Vector4f&)
{
}

void Context_GL3_Root::transform(const Nigel::math::Matrix4x4f& matrix)
{
}
