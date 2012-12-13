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

#include <QtCore/QEvent>
#include <QtCore/QObject>

#include "Node.hpp"
#include <COLLADA/Element.hpp>

namespace GLr { namespace graph {

class GLrExport ElementNode
    : public QObject
    , public Node
{
    Q_OBJECT

protected:
    ElementNode( GLr::rendering::Renderer* renderer,
                 Nigel::COLLADA::Element* element, kuint type );

public:
    virtual ~ElementNode();

    template< typename T >
    inline const T* element() { return static_cast< const T* >( _element ); }

    /*!
     * This method is safe to use from another thread than this ElementNode's
     * current thread.
     */
    virtual void addChildNode( Node* child );

    virtual ElementNode* toElementNode();

protected:
    void addWrappedChild( Nigel::COLLADA::Element* element );

    virtual void customEvent( QEvent* e );

public slots:
    virtual void destroy();

private slots:
    void elementAdded( Nigel::COLLADA::Element* element );
    void elementWasRemoved();

private:
    Nigel::COLLADA::Element* _element;

};

}}
