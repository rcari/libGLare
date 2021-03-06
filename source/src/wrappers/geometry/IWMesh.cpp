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

#include <wrappers/geometry/IWMesh.hpp>
using namespace GLr::wrappers::geometry;
using namespace GLr::rendering;

#include <geometry/mesh/Primitive.hpp>
using namespace Nigel::geometry::mesh;

#include <GLrMacros.hpp>

#include <geometry/mesh/Structures.hpp>
#include <geometry/mesh/buffer/VertexBuffer.hpp>
using namespace Nigel::geometry::mesh;

IWMesh::IWMesh(Renderer* renderer, Mesh* mesh)
:	ElementNode(renderer, mesh, Geometry)
{
}

const Mesh* IWMesh::mesh()
{
	return element<Mesh>();
}

const kvoid* IWMesh::staticVerticesData(kint offset)
{
	return GLR_BUFFER_OFFSET( offset * sizeof(Nigel::geometry::mesh::Vertex) );
}

const kvoid* IWMesh::dynamicVerticesData(kint offset)
{
	return element<Mesh>()->verticesBuffer()->vertices() + offset;
}
