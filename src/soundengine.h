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

#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include <kconfig.h>
#include <kdebug.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <kmimetype.h>
#include <kstandarddirs.h>
#include <kurl.h>
#include <qtimer.h>
#include <qfile.h>
#include <qdir.h>

#include <arts/connect.h>
#include <arts/dynamicrequest.h>
#include <arts/flowsystem.h>
#include <arts/kartsdispatcher.h>
#include <arts/kartsserver.h>
#include <arts/kmedia2.h>
#include <arts/kplayobjectfactory.h>
#include <arts/soundserver.h>
#include <qobject.h>
#include <kurl.h>

namespace Arts
{
class PlayObject;
class SoundServerV2;
}
  
/**
 * Handles all playing, connecting to aRts.
 * Does almost everything related to multimedia.
 * Most interfacing should be done with Player
 **/
class SoundEngine : public QObject
{
Q_OBJECT

public:
	SoundEngine(QObject *parent=0);
	~SoundEngine();

	enum EngineState { Stop, Play, Empty };

public slots:
	/**
	 * Load a file
	 **/
	bool load(const KURL &file);

	/**
	 * Pause while playing
	 **/
	void pause();
	
	/**
	 * Start
	 **/
	void play();
	
	/**
	 * stops, and unloads
	 **/
	void stop();

public:
	EngineState state();

private:
	bool reload(void);
	bool m_needReload;
	KPlayObject *playobj;
	KArtsDispatcher dispatcher;
	KArtsServer server;
	KURL file;	
};

#endif
