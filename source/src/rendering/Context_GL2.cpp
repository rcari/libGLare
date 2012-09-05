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

#include <rendering/Context_GL2.hpp>
#include <rendering/ICamera.hpp>
using namespace GLr::rendering;

#include <wrappers/scene/WVisualScene.hpp>
using namespace GLr::wrappers::scene;

using namespace GLr::graph;

#include <math/Vector4f.hpp>
#include <math/Quaternionf.hpp>
using namespace Nigel::math;

Context_GL2::Context_GL2(Renderer* renderer)
:	IContext(renderer)
{
	glPushMatrix();
	glLoadIdentity();
}

void Context_GL2::renderScene(WVisualScene* scene, ICamera* camera)
{
	// Setup the camera projection...
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	switch(camera->projection())
	{
	case ICamera::Perspective:
		gluPerspective(
				camera->yFov(),
				camera->ratio(),
				camera->zNear(),
				camera->zFar()
			);
		break;
	case ICamera::Orthogonal:
		glOrtho(
				camera->left(),
				camera->right(),
				camera->bottom(),
				camera->top(),
				camera->zNear(),
				camera->zFar()
			);
		break;
	}

	// Setup the camera position and orientation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector4f pos;
	camera->position(pos);

	Vector4f at(0.0f, 0.0f, 1.0f, 0.0f); // Positive Z
	Vector4f up(0.0f, 1.0f, 0.0f, 0.0f); // Positive Y

	Quaternionf rot;
	camera->orientation(rot);
	rot.rotateVector(at);
	rot.rotateVector(up);

	// The point we look at depends on the current position !
	at += pos;

//	qDebug(
//			"---------------\n"
//			"From:	%f %f %f\n"
//			"At:	%f %f %f\n"
//			"Up:	%f %f %f",
//			pos.values().x,		pos.values().y,		pos.values().z,
//			at.values().x,		at.values().y,		at.values().z,
//			up.values().x,		up.values().y,		up.values().z
//		);

	gluLookAt(
			pos.values().x,		pos.values().y,		pos.values().z,
			at.values().x,		at.values().y,		at.values().z,
			up.values().x,		up.values().y,		up.values().z
		);

	// And let's go !
	this->visitNode(*this, scene);
}

void Context_GL2::translate(const Nigel::math::Vector4f& direction)
{
	glTranslatef(
			direction.values().x,
			direction.values().y,
			direction.values().z
		);
}

void Context_GL2::rotate(const Nigel::math::Quaternionf& orientation)
{
	kfloat data[4];

	// Store the axis/angle representation.
	orientation.toAxisAngle(data);

	glRotatef(
			data[0], // Angle
			data[1], // X
			data[2], // Y
			data[3]  // Z
		);
}

void Context_GL2::scale(const Nigel::math::Vector4f& weights)
{
	glScalef(
			weights.values().x,
			weights.values().y,
			weights.values().z
		);
}

void Context_GL2::transform(const Nigel::math::Matrix4x4f& matrix)
{
	glMultTransposeMatrixf(matrix.values());
}

void Context_GL2::visitNode(Context_GL2& context, Node* node)
{
	if	(
			// We visited all the pre-visit nodes
			context.stackDepth() == node->preVisitNodesNb() ||
			// The node is not supposed to be rendered according to the current context, no need to "pre-visit"
			!(node->type() & context.activeNodeTypes())
		)
	{
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
				Context_GL2 ctx(childNode, context, i);
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
