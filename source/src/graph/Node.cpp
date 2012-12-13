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

#include <graph/Node.hpp>
using namespace GLr::graph;

#include <rendering/IContext.hpp>
using namespace GLr::rendering;

Node::Node( Renderer* renderer, kuint type )
    : IRendererObject( renderer )
    , _childrenOffset( 0 )
    , _parentsOffset( 0 )
    , _delegate( this )
    , _type( type )
    , _preventDestroy( false )
{
}

Node::~Node()
{
    _preventDestroy = true;

    QVector< Node* > children = _links.mid( 0, _parentsOffset );
    QVector< Node* > parents = _links.mid( _parentsOffset );

    // Remove children (pre-visit + child nodes)
    for( kint i = 0; i < children.size(); ++i )
    {
        // Get rid of the children.
        removeChildNode( children.at( i ) );
    }

    // Remove parents
    for( kint i = 0; i < parents.size(); ++i )
    {
        // Tell the parents we are gone.
        parents.at( i )->removeChildNode( this );
    }
}

void Node::destroy()
{
    delete this;
}

void Node::addChildNode( Node* child )
{
    // Only add the child node if it is not already a child.
    if( child->addParentNode( this ) )
    {
        if( child->type() & PreVisit )
        {
            _links.insert( _childrenOffset, child );
            ++_childrenOffset;
        }
        else
        {
            _links.insert( _childrenOffset, child );
        }
        ++_parentsOffset;
    }
}

void Node::removeChildNode( Node* child )
{
    //K_ASSERT( _links.indexOf( child ) < _parentsOffset )

    if( child->type() & PreVisit )
    {
        child->removeParentNode( this );
        _links.remove( _links.indexOf( child ) );
        --_childrenOffset;
    }
    else
    {
        child->removeParentNode( this );
        _links.remove( _links.indexOf( child, _childrenOffset ) );
    }
    --_parentsOffset;
}

Node** Node::preVisitNodes()
{
    return _links.data();
}

Node** Node::children()
{
    return ( _links.data() + _childrenOffset );
}

Node** Node::parents()
{
    return ( _links.data() + _parentsOffset );
}

void Node::installDelegate( NodeDelegate* delegate )
{
    _delegate = delegate;
}

void Node::uninstallDelegate()
{
    _delegate = this;
}

ElementNode* Node::toElementNode()
{
    return K_NULL;
}

bool Node::addParentNode( Node* parent )
{
    // No need for the child node to have this parent multiple times.
    if( _links.contains( parent ) )
    {
        return false;
    }
    else
    {
        _links.append( parent );
        return true;
    }
}

void Node::removeParentNode( Node* parent )
{
    int index = _links.indexOf( parent, _parentsOffset );
    K_ASSERT( index != -1 )
    _links.remove( index );

    if( ( ! _preventDestroy ) && ( _parentsOffset == _links.size() ) )
    {
        qDebug( "Destroying node @ %p because its an orphan !", this );
        destroy(); // We don't have a parent anymore, we have to go !
    }
}

void Node::removeFromTree()
{
    _preventDestroy = true;

    QVector< Node* > parents = _links.mid( _parentsOffset );

    for( kint i = 0; i < parents.size(); ++i )
    {
        parents.at( i )->removeChildNode( this );
    }
    _preventDestroy = false;
}

kbool Node::enterNode( IContext& ctx )
{
    return ctx.activeNodeTypes() & _type;
}

void Node::visitNode( IContext& ctx )
{
    _delegate->renderNode( this, ctx );
}

void Node::leaveNode( IContext& ctx )
{
    _delegate->leaveNode( this, ctx );
}

void Node::renderNode( Node* node, IContext& )
{
    K_ASSERT( node == this )
}

void Node::leaveNode( Node* node, IContext& )
{
    K_ASSERT( node == this )
}
