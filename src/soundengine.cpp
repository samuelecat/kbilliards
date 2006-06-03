/***************************************************************************
                          soundengine.h  -  description
                             -------------------
    begin                : Sat Jul 27 2002
    copyright            : (C) 2002 by Samuele Catuzzi
    email                : samuele_catuzzi@yahoo.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/********************************************************************************
 *                                                                              *
 *  This class is a remake of portions of software from kaboodle (class Engine) *
 *  I report here noatun copyright notice and permission notice.                *
 *                                                        Samuele Catuzzi       *
 ********************************************************************************/

/*****************************************************************

Copyright (c) 2000-2001 the noatun authors. See file NOATUN_AUTHORS.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIAB\ILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

#include "soundengine.h"
#include <string.h>
       

SoundEngine::SoundEngine(QObject *parent)
	: QObject(parent), playobj(0) , dispatcher()	, server()
{
}

SoundEngine::~SoundEngine()
{
	stop();
  delete playobj;
}

bool SoundEngine::load(const KURL &nfile)
{
	if(nfile.path().length())
	{
		file = nfile;
		return reload();
	}
	else return false;
}

bool SoundEngine::reload(void)
{
	delete playobj;
	playobj = 0;

	KPlayObjectFactory factory(server.server());
	playobj = factory.createPlayObject(file, true);
	
	m_needReload = false;

	return !playobj->object().isNull();
}

void SoundEngine::play()
{
	if(playobj && !playobj->object().isNull())
	{
		switch(playobj->state())
		{
		case Arts::posIdle:
			if (m_needReload) 
				reload();
			m_needReload = true;
			playobj->play();
			break;
		case Arts::posPaused:
			playobj->play();
			break;
    default: break;
		}
	}
}

void SoundEngine::pause()
{
	if(playobj && !playobj->object().isNull())
		playobj->pause();
}

void SoundEngine::stop()
{
	if(playobj && !playobj->object().isNull())
  		playobj->halt();
}

SoundEngine::EngineState SoundEngine::state()
{
	if(!playobj || playobj->object().isNull()) return Empty;

	switch(playobj->state())
	{
	case Arts::posIdle:
		return Stop;
		break;
	case Arts::posPlaying:
		return Play;
		break;
  default: break;
	}
	return Stop;
}

#include "soundengine.moc"

