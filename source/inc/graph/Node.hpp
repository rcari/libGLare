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

#pragma once

#include <GLrExport.hpp>

#include <Types.hpp>
#include <QtCore/QVector>

#include "../rendering/IRendererObject.hpp"
#include "NodeDelegate.hpp"

namespace GLr {

namespace rendering {
class Renderer;
class IContext;
}

namespace graph {

class ElementNode;

class GLrExport Node : public GLr::rendering::IRendererObject, protected NodeDelegate
{

public:
	enum Type
	{
		Undefined		= 0x0,

		// Track-able nodes
		Camera			= 0x1 << 0,
		Light			= 0x1 << 1,
		Trackable		= Camera | Light,

		// Pre-visit nodes
		Acceleration	= 0x1 << 2,
		Transform		= 0x1 << 3,
		Texture			= 0x1 << 4,
		Shader			= 0x1 << 5,
		PrivateData		= 0x1 << 6,
		PreVisit		= Acceleration | Transform | Texture | Shader,

		// Visit types
		Geometry		= 0x1 << 7,
		SceneNode		= 0x1 << 8,
		Visit			= Geometry | SceneNode,
	};

protected:
	Node(GLr::rendering::Renderer* renderer, kuint type);
	virtual ~Node();

public:
	virtual void destroy();

	inline kuint type() const { return _type; }

	virtual void addChildNode(Node* child);
	virtual void removeChildNode(Node* child);

	Node** preVisitNodes();
	inline kint preVisitNodesNb() const { return _childrenOffset; }

	Node** children();
	inline kint childrenNb() const { return _parentsOffset - _childrenOffset; }

	Node** parents();
	inline kint parentsNb() const { return _links.size() - _parentsOffset; }

	void installDelegate(NodeDelegate* delegate);
	void uninstallDelegate();

	virtual ElementNode* toElementNode();

private:
	bool addParentNode(Node* parent);
	void removeParentNode(Node* parent);

protected:
	void removeFromTree();

public:
	virtual kbool enterNode(GLr::rendering::IContext& context);
	void visitNode(GLr::rendering::IContext& context);
	void leaveNode(GLr::rendering::IContext& context);

protected:
	virtual void renderNode(Node* node, GLr::rendering::IContext&);
	virtual void leaveNode(Node* node, GLr::rendering::IContext&);

protected:
	QVector<Node*>	_links;
	kint			_childrenOffset;
	kint			_parentsOffset;

private:
	NodeDelegate*	_delegate;
	kuint			_type;
	kbool			_preventDestroy;
};

}}
