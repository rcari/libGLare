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

#include <math/Quaternionf.hpp>
#include <math/Vector4f.hpp>

namespace GLr { namespace rendering {

class GLrExport ICamera
{
public:
	enum Projection
	{
		Perspective,
		Orthogonal,
	};

	virtual ~ICamera() {}
	virtual void orientation(Nigel::math::Quaternionf&) const = K_NULL;
	virtual void position(Nigel::math::Vector4f&) const = K_NULL;

	virtual Projection projection() const = K_NULL;

	//! Common
	virtual kfloat zNear() const = K_NULL;
	virtual kfloat zFar() const = K_NULL;

	//! Perspective
	virtual kfloat yFov() const = K_NULL;
	virtual kfloat ratio() const = K_NULL;

	//! Orthogonal
	virtual kfloat left() const = K_NULL;
	virtual kfloat right() const = K_NULL;
	virtual kfloat bottom() const = K_NULL;
	virtual kfloat top() const = K_NULL;

};

}}
