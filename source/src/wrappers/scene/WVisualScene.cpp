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

#define GLR_GENERATOR_TYPE GLr::wrappers::scene::WVisualScene

#include <wrappers/scene/WVisualScene.hpp>
using namespace GLr::wrappers::scene;
using namespace GLr::wrappers;
using namespace GLr::rendering;

#include <scene/Node.hpp>
using namespace Nigel::scene;

GLR_GENERATOR_I_BEGIN
	GLR_GENERATOR_I_BEGIN_REGISTRATION
		GLR_GENERATOR_I_REGISTER( Nigel::scene::VisualScene )
	GLR_GENERATOR_I_END_REGISTRATION
	GLR_GENERATOR_I_PROBE_SCORE( 0 )
	GLR_GENERATOR_I_GENERATE_WRAPPER( return new WVisualScene(GLR_GENERATOR_ARG_RENDERER, static_cast<VisualScene*>(GLR_GENERATOR_ARG_ELEMENT)); )
GLR_GENERATOR_I_END

WVisualScene::WVisualScene(Renderer* renderer, VisualScene* visualScene)
:	ElementNode(renderer, visualScene, SceneNode)
{
	// Wrap all child nodes in the scene !
	for(kint i = 0; i < visualScene->nodesNb(); i++)
	{
		addWrappedChild(visualScene->node(i));
	}
}

void WVisualScene::renderNode(Node* node, GLr::rendering::IContext&)
{
	// Nothing yet !
	// TODO: Setup lights !
}
