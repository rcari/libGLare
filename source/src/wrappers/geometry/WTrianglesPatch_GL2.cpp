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

#define GLR_GENERATOR_TYPE GLr::wrappers::geometry::WTrianglesPatch_GL2

#include <wrappers/geometry/IWMesh.hpp>
#include <wrappers/geometry/WTrianglesPatch_GL2.hpp>
using namespace GLr::wrappers::geometry;
using namespace GLr::rendering;
using namespace GLr::opengl::buffers;

using namespace Nigel::geometry::mesh;

#include <GLrMacros.hpp>

GLR_GENERATOR_I_BEGIN
	GLR_GENERATOR_I_BEGIN_REGISTRATION
		GLR_GENERATOR_I_REGISTER( Nigel::geometry::mesh::TrianglesPatch )
	GLR_GENERATOR_I_END_REGISTRATION
	GLR_GENERATOR_I_PROBE_SCORE( GLR_CHECK_GL_EXT(ARB_vertex_buffer_object) ? 0 : -1 )
	GLR_GENERATOR_I_GENERATE_WRAPPER( return new WTrianglesPatch_GL2(GLR_GENERATOR_ARG_RENDERER, static_cast<TrianglesPatch*>(GLR_GENERATOR_ARG_ELEMENT)); )
GLR_GENERATOR_I_END

WTrianglesPatch_GL2::WTrianglesPatch_GL2(Renderer* renderer, TrianglesPatch* patch)
:	IWTrianglesPatch(renderer, patch),
 	_faces(new BufferObject(renderer))
{
	// Load the faces data !
	updateFacesData();
}

void WTrianglesPatch_GL2::renderNode(Node* node, IContext& context)
{
	K_ASSERT( node == this )

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Bind the vertices buffer
	meshWrapper()->verticesBuffer()->bind(GL_ARRAY_BUFFER_ARB);

	// Bind the indices buffer
	_faces->bind(GL_ELEMENT_ARRAY_BUFFER_ARB);

	// Set the pointer to the vertices data in the Mesh's vertex buffer
	glVertexPointer(
			4, GL_FLOAT, 0,
			meshWrapper()->staticVerticesData(element<TrianglesPatch>()->verticesOffset())
		);

	// Enable the vertex array state
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor3f(0.0f, 0.0f, 0.0f);

	// Draw call
	glDrawElements(
			GL_TRIANGLES,
			element<TrianglesPatch>()->count(),
			GL_UNSIGNED_INT,
			GLR_BUFFER_OFFSET(0)
		);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.0f, 1.0f);

	glPolygonOffset(-1.0f, -1.0f);
	glEnable(GL_POLYGON_OFFSET_LINE);

	glDrawElements(
			GL_TRIANGLES,
			element<TrianglesPatch>()->count(),
			GL_UNSIGNED_INT,
			GLR_BUFFER_OFFSET(0)
		);

	glDisable(GL_POLYGON_OFFSET_LINE);

	// Disable the vertex array state
	glDisableClientState(GL_VERTEX_ARRAY);

	// Unbind the indices buffer
	_faces->unbind();

	// Unbind the vertices buffer
	meshWrapper()->verticesBuffer()->unbind();

	glDisable(GL_CULL_FACE);
}

void WTrianglesPatch_GL2::renderPatch(GLr::rendering::IContext& context)
{
	// Render from client memory !

	// Set the pointer to the vertices data in the Mesh's vertex buffer
	glVertexPointer(
			4, GL_FLOAT, 0,
			meshWrapper()->dynamicVerticesData(element<TrianglesPatch>()->verticesOffset())
		);

	// Enable the vertex array state
	glEnableClientState(GL_VERTEX_ARRAY);

	// Draw call
	glDrawElements(
			GL_TRIANGLES,
			element<TrianglesPatch>()->count(),
			GL_UNSIGNED_INT,
			element<TrianglesPatch>()->indices()
		);

	// Disable the vertex array state
	glDisableClientState(GL_VERTEX_ARRAY);
}

void WTrianglesPatch_GL2::renderFaces(GLr::rendering::IContext& context, kvoid* facesData)
{
	ksize facesNb = *static_cast<ksize*>(facesData);

	kbyte* ptr = static_cast<kbyte*>(facesData);
	ptr += sizeof(ksize);

	GLsizei* counts = reinterpret_cast<GLsizei*>(ptr);
	ptr += facesNb * sizeof(GLsizei);

	const kvoid** indices = reinterpret_cast<const kvoid**>(ptr);

	// Enable the vertex array state
	glEnableClientState(GL_VERTEX_ARRAY);

	// Set the pointer to the vertices data in the Mesh's vertex buffer
	glVertexPointer(
			4, GL_FLOAT, 0,
			meshWrapper()->dynamicVerticesData(element<TrianglesPatch>()->verticesOffset())
		);

	// Draw call
	glMultiDrawElements(
			GL_TRIANGLES,
			counts,
			GL_UNSIGNED_INT,
			indices,
			facesNb
		);

	// Disable the vertex array state
	glDisableClientState(GL_VERTEX_ARRAY);
}

kvoid* WTrianglesPatch_GL2::createFacesRenderData(GLr::rendering::IContext& context, const QList<kint>& faces)
{
	kvoid* data = kApp->memoryManager()->mAlloc( sizeof(ksize) + faces.size() * ( sizeof(kvoid*) + sizeof(GLsizei) ) );

	kbyte* ptr = static_cast<kbyte*>(data);

	// Store the number of faces
	(*reinterpret_cast<ksize*>(ptr)) = faces.size();
	ptr += sizeof(ksize);

	GLsizei* counts = reinterpret_cast<GLsizei*>(ptr);
	ptr += faces.size() * sizeof(GLsizei);

	const kvoid** indices = reinterpret_cast<const kvoid**>(ptr);

	for(kint i = 0; i < faces.size(); i++)
	{
		// 3 vertices per triangle...
		counts[i] = 3;
		indices[i] = element<TrianglesPatch>()->triangles() + faces.at(i); // Data comes from client memory
	}

	return data;
}

void WTrianglesPatch_GL2::destroyFacesRenderData(kvoid* data)
{
	kApp->memoryManager()->mFree(data);
}

void WTrianglesPatch_GL2::renderVertices(GLr::rendering::IContext& context, kvoid* verticesData)
{
	ksize verticesNb = *static_cast<ksize*>(verticesData);

	kbyte* ptr = static_cast<kbyte*>(verticesData);
	ptr += sizeof(ksize);

	GLint* firsts = reinterpret_cast<GLint*>(ptr);

	ptr += verticesNb * sizeof(GLint);

	GLsizei* counts = reinterpret_cast<GLsizei*>(ptr);

	// Enable the vertex array state
	glEnableClientState(GL_VERTEX_ARRAY);

	// Set the pointer to the vertices data in the Mesh's vertex buffer
	glVertexPointer(
			4, GL_FLOAT, 0,
			meshWrapper()->dynamicVerticesData(element<TrianglesPatch>()->verticesOffset())
		);

	// Draw call
	glMultiDrawArrays(
			GL_POINTS,
			firsts,
			counts,
			verticesNb
		);

	// Disable the vertex array state
	glDisableClientState(GL_VERTEX_ARRAY);
}

kvoid* WTrianglesPatch_GL2::createVerticesRenderData(GLr::rendering::IContext& context, const QList<kint>& vertices)
{
	kvoid* data = kApp->memoryManager()->mAlloc( sizeof(ksize) + vertices.size() * ( sizeof(GLint) + sizeof(GLsizei) ) );

	kbyte* ptr = static_cast<kbyte*>(data);

	// Store the number of vertices
	(*reinterpret_cast<ksize*>(ptr)) = vertices.size();
	ptr += sizeof(ksize);

	GLint* firsts = reinterpret_cast<GLint*>(ptr);

	ptr += vertices.size() * sizeof(GLint);

	GLsizei* counts = reinterpret_cast<GLsizei*>(ptr);

	for(kint i = 0; i < vertices.size(); i++)
	{
		firsts[i] = vertices.at(i);
		// 1 vertex per vertex...
		counts[i] = 1;
	}

	return data;
}

void WTrianglesPatch_GL2::destroyVerticesRenderData(kvoid* data)
{
	kApp->memoryManager()->mFree(data);
}

void WTrianglesPatch_GL2::selectFaces(GLr::rendering::IContext& context)
{
	const TrianglesPatch* patch = element<TrianglesPatch>();

	// Bind the indices buffer
	_faces->bind(GL_ELEMENT_ARRAY_BUFFER_ARB);

	// Enable the vertex array state
	glEnableClientState(GL_VERTEX_ARRAY_EXT);

	// Set the pointer to the vertices data in the Mesh's vertex buffer
	glVertexPointer(
			4, GL_FLOAT, 0,
			GLR_BUFFER_OFFSET( patch->verticesOffset() * sizeof(Vertex) )
		);

	for(kint i = 0; i < patch->trianglesNb(); i++)
	{
		// Push the name on the stack.
		glLoadName(i);
		// We draw a single triangle...
		glDrawElements(
				GL_TRIANGLES,
				3, // Just one triangle !
				GL_UNSIGNED_INT,
				GLR_BUFFER_OFFSET(i*sizeof(TriangleFace))
			);
	}

	// Disable the vertex array state
	glDisableClientState(GL_VERTEX_ARRAY_EXT);

	// Unbind the indices buffer
	_faces->unbind();
}

void WTrianglesPatch_GL2::selectVertices(GLr::rendering::IContext& context)
{
	const TrianglesPatch* patch = element<TrianglesPatch>();

	// Enable the vertex array state
	glEnableClientState(GL_VERTEX_ARRAY);

	// Set the pointer to the vertices data in the Mesh's vertex buffer
	glVertexPointer(
			4, GL_FLOAT, 0,
			GLR_BUFFER_OFFSET( patch->verticesOffset() * sizeof(Vertex) )
		);

	for(kint i = 0; i < patch->trianglesNb(); i++)
	{
		// Push the name on the stack.
		glLoadName(i);
		// We draw a single point...
		glDrawArrays( GL_POINTS, i, 1 );
	}

	// Disable the vertex array state
	glDisableClientState(GL_VERTEX_ARRAY);
}

void WTrianglesPatch_GL2::updateFacesData()
{
	RendererLocker locker(this->renderer());

	_faces->create(
			element<TrianglesPatch>()->count()*sizeof(kint),
			GL_STATIC_DRAW_ARB,
			element<TrianglesPatch>()->indices()
		);
}

void WTrianglesPatch_GL2::updateFacesDataRange(kint offset, kint nb)
{
	RendererLocker locker(this->renderer());

	_faces->update(
			offset * sizeof(TriangleFace),
			nb * sizeof(TriangleFace),
			element<TrianglesPatch>()->triangles() + offset
		);
}
