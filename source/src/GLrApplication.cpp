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

#include <GLrApplication.hpp>
#include <GLrEngine.hpp>
#include <GLrModule.hpp>
using namespace GLr;

#include <KoreApplication.hpp>
using namespace Kore::data;
using namespace Kore;

GLrApplication* GLrApplication::_Instance = K_NULL;

GLrApplication* GLrApplication::Instance()
{
	K_ASSERT( _Instance )
	return _Instance;
}

GLrApplication::GLrApplication(kint argc, kchar** argv)
{
	qDebug("GLr / Loading GLrApplication");

	// Only one instance is allowed !!!
	K_ASSERT( _Instance == K_NULL )

	_Instance = this;

	GLrModule::PrivateInstance()->load();

	// Create Pantin's Internals
	_glrLib = new Library(Block::System);
	_glrLib->blockName("GLr Internals");
	_glrLib->addBlock(GLrEngine::PrivateInstance());

	const_cast<Library*>(kApp->rootLibrary())->insertBlock(_glrLib, kApp->dataLibrary()->index());
}

GLrApplication::~GLrApplication()
{
	qDebug("GLr / Unloading GLrApplication");
}

QString GLrApplication::Version()
{
	return QString::fromAscii(_GLARE_VERSION);
}
