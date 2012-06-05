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

#define GLR_GENERATOR_TYPE GLr::wrappers::geometry::WMesh_GL2

#include <GLrMacros.hpp>

#include <wrappers/geometry/WMesh_GL2.hpp>
using namespace GLr::wrappers::geometry;
using namespace GLr::opengl::buffers;
using namespace GLr::rendering;

#include <geometry/mesh/buffer/VertexBuffer.hpp>
#include <geometry/mesh/Primitive.hpp>
using namespace Nigel::geometry::mesh;

GLR_GENERATOR_I_BEGIN
	GLR_GENERATOR_I_BEGIN_REGISTRATION
		GLR_GENERATOR_I_REGISTER( Nigel::geometry::mesh::Mesh )
	GLR_GENERATOR_I_END_REGISTRATION
	GLR_GENERATOR_I_PROBE_SCORE( GLR_CHECK_GL_EXT(ARB_vertex_buffer_object) ? 0 : -1 )
	GLR_GENERATOR_I_GENERATE_WRAPPER( return new WMesh_GL2(GLR_GENERATOR_ARG_RENDERER, static_cast<Mesh*>(GLR_GENERATOR_ARG_ELEMENT)); )
GLR_GENERATOR_I_END

WMesh_GL2::WMesh_GL2(Renderer* renderer, Mesh* mesh)
:	IWMesh(renderer, mesh),
 	_vertices(new BufferObject(renderer))
{
	// Create the buffer
	updateVerticesVBO();

	// Wrap the primitives !
	for(kint i = 0; i < mesh->primitivesNb(); i++)
	{
		addWrappedChild(mesh->primitive(i));
	}
}

void WMesh_GL2::updateVerticesVBO()
{
	// Lock the renderer to do our stuff
	RendererLocker locker(this->renderer());

	// Lock the data source to copy
	QMutexLocker verticesLocker(mesh()->verticesBuffer()->mutex());

	// Perform the copy.
	_vertices->create(
			mesh()->verticesBuffer()->dataSize(),
			GL_STATIC_DRAW_ARB,
			mesh()->verticesBuffer()->data()
		);
}

GLr::opengl::buffers::BufferObject* WMesh_GL2::verticesBuffer()
{
	return _vertices;
}
