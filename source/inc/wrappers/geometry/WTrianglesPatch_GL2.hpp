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

#include "IWTrianglesPatch.hpp"
#include "../IWrapperGenerator.hpp"
#include "../../opengl/buffers/BufferObject.hpp"

namespace GLr { namespace wrappers { namespace geometry {

/*!
 * Depends on ARB_vertex_buffer_object and EXT_vertex_array
 */
class WTrianglesPatch_GL2 : public IWTrianglesPatch
{
	GLR_GENERATOR

protected:
	WTrianglesPatch_GL2(GLr::rendering::Renderer* renderer, Nigel::geometry::mesh::TrianglesPatch* patch);

	virtual void renderNode(Node* node, GLr::rendering::IContext& context);

public:
	virtual void renderPatch(GLr::rendering::IContext& context);

	virtual void renderFaces(GLr::rendering::IContext& context, kvoid* facesData);
	virtual kvoid* createFacesRenderData(GLr::rendering::IContext& context, const QList<kint>& faces);
	virtual void destroyFacesRenderData(kvoid* data);

	virtual void renderVertices(GLr::rendering::IContext& context, kvoid* verticesData);
	virtual kvoid* createVerticesRenderData(GLr::rendering::IContext& context, const QList<kint>& vertices);
	virtual void destroyVerticesRenderData(kvoid* data);

	virtual void selectFaces(GLr::rendering::IContext& context);
	virtual void selectVertices(GLr::rendering::IContext& context);

	virtual void updateFacesData();
	virtual void updateFacesDataRange(kint offset, kint nb);

private:
	GLr::opengl::buffers::BufferObject*	_faces;
};

}}}
