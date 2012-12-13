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

#include <graph/ElementNode.hpp>
using namespace GLr::graph;
using namespace GLr::rendering;
using namespace Nigel::COLLADA;
using namespace Kore::data;

#include <QtCore/QCoreApplication>
#include <QtCore/QMutexLocker>
#include <QtCore/QThread>
#include <QtCore/QVariant>

namespace {

class AddChildNodeEvent : public QEvent
{
public:
    AddChildNodeEvent( Node* c )
    : QEvent( QEvent::User )
    , child( c )
    {
    }

public:
    Node* const child;
};

}

ElementNode::ElementNode( Renderer* renderer, Element* element, kuint type )
    : Node( renderer, type )
    , _element( element )
{
    // Store this wrapper as a dynamic property of the Element.
    // TODO: Wrap that in a simple API call that takes care of threading issues
    // at the MetaBlock level!
    _element->setProperty(
            qPrintable( renderer->runtimeID() ),
            qVariantFromValue( static_cast< kvoid* >( this ) )
        );

    // To be notified of new elements
    connect( element, SIGNAL( elementAdded( Nigel::COLLADA::Element* ) ),
             SLOT( elementAdded( Nigel::COLLADA::Element* ) ) );
    // Delete when the element is deleted (no leaking).
    connect( element, SIGNAL( blockRemoved() ), SLOT( elementWasRemoved() ) );
    // Delete when the element is deleted (no leaking).
    connect( element, SIGNAL( blockDeleted() ), SLOT( destroy() ) );
}

ElementNode::~ElementNode()
{
    // Un-store this wrapper as a dynamic property of the Element.
    // TODO: Wrap that in a simple API call that takes care of threading issues
    // at the MetaBlock level!
    _element->setProperty( qPrintable( renderer()->runtimeID() ), QVariant() );
}

void ElementNode::destroy()
{
    // Clean delete later !
    deleteLater();
}

void ElementNode::addChildNode( Node* child )
{
    if( this->thread() == QThread::currentThread() )
    {
        // Add the node immediately if this is the object's thread.
        Node::addChildNode( child );
    }
    else
    {
        // Add the child by posting a message to the owner thread.
        AddChildNodeEvent* event = new AddChildNodeEvent( child );
        QCoreApplication::postEvent( this, event );
    }
}

ElementNode* ElementNode::toElementNode()
{
    return this;
}

void ElementNode::addWrappedChild( Element* element )
{
    ElementNode* node = renderer()->getWrapperForElement( element );
    if( node )
    {
        addChildNode( node );
    }
}

void ElementNode::customEvent( QEvent* e )
{
    K_ASSERT( e->type() == QEvent::User )
    // We might be again in another thread than the actual thread of that node,
    // so do it properly !
    addChildNode( static_cast< AddChildNodeEvent* >( e )->child );
}

void ElementNode::elementAdded( Nigel::COLLADA::Element* element )
{
    addWrappedChild( element );
}

void ElementNode::elementWasRemoved()
{
    removeFromTree();
}
