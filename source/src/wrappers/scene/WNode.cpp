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

#define GLR_GENERATOR_TYPE GLr::wrappers::scene::WNode

#include <wrappers/scene/WNode.hpp>
using namespace GLr::wrappers::scene;
using namespace GLr::rendering;
using namespace Nigel::COLLADA;

#include <geometry/GeometryInstance.hpp>
#include <scene/transformation/Transformation.hpp>

#include <scene/Node.hpp>
#include <scene/NodeInstance.hpp>
using namespace Nigel::scene;

GLR_GENERATOR_I_BEGIN
	GLR_GENERATOR_I_BEGIN_REGISTRATION
		GLR_GENERATOR_I_REGISTER( Nigel::scene::Node )
	GLR_GENERATOR_I_END_REGISTRATION
	GLR_GENERATOR_I_PROBE_SCORE( 0 )
	GLR_GENERATOR_I_GENERATE_WRAPPER(
			return new WNode(GLR_GENERATOR_ARG_RENDERER, static_cast<Nigel::scene::Node*>(GLR_GENERATOR_ARG_ELEMENT));
		)
GLR_GENERATOR_I_END

WNode::WNode(Renderer* renderer, Nigel::scene::Node* node)
:	ElementNode(renderer, node, SceneNode)
{
	// We need wrappers for all the possible children !

	// Transformations
	for(kint i = 0; i < node->transformationsNb(); i++)
	{
		addWrappedChild(node->transformation(i));
	}

	// TODO: Add cameras ?

	// Geometry instances
	for(kint i = 0; i < node->geometryInstancesNb(); i++)
	{
		addWrappedChild(node->geometryInstance(i));
	}

	// TODO: Add lights ?

	// Node instances
	for(kint i = 0; i < node->nodeInstancesNb(); i++)
	{
		addWrappedChild(node->nodeInstance(i));
	}

	// Nodes
	for(kint i = 0; i < node->nodesNb(); i++)
	{
		addWrappedChild(node->node(i));
	}
}

kbool WNode::enterNode(GLr::rendering::IContext& ctx)
{
	return element<Nigel::scene::Node>()->enabled() && ElementNode::enterNode(ctx);
}

void WNode::leaveNode(Node* node, GLr::rendering::IContext&)
{
	glDisable(GL_DEPTH_TEST);

	static kfloat vertices[] =
	{
			0.0f,		0.0f,		0.0f,
			1000.0f,	0.0f,		0.0f,
			0.0f,		0.0f,		0.0f,
			0.0f,	 	1000.0f,	0.0f,
			0.0f,		0.0f,		0.0f,
			0.0f,	 	0.0f,		1000.0f,
	};

	static kfloat colors[] =
	{
			1.0f,	0.0f,	0.0f,
			1.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,
			0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	1.0f,
			0.0f,	0.0f,	1.0f,
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);

	glDrawArrays(GL_LINES, 0, 6);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_DEPTH_TEST);

}
