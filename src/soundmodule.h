/***************************************************************************
                          soundmodule.h  -  description
                             -------------------
    begin                : ven lug 12 2002
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

#ifndef SOUNDMODULE_H
#define SOUNDMODULE_H


/**
  *@author Samuele Catuzzi
  */

#include <qobject.h>
#include <kurl.h>
#include "soundengine.h"

class SoundModule : public QObject
{
 Q_OBJECT


public: 
	SoundModule(QString p_sound, QObject *parent = 0);
	~SoundModule();
  /** No descriptions */
  void setLoop(bool );
  /** No descriptions */
  void stop();
  /** No descriptions */
  void loadfile(QString );
  /** No descriptions */
  void play();
  /** No descriptions */
  void OneShot(QString);
  /** retrive value of enablemusic  */
  bool getMusicState();
  /** retrive value of enableeffects */
  bool getEffectState();
private: // Private attributes
  /**  */
  SoundEngine * musicEngine;
  /**  */
  SoundEngine * effectEngine;
  /**  */
  bool unfinished;
  /**  */
  QString path_sound;
  /**  */ 
  bool sLoop;
  /**  */
  bool enablemusic;
  /**  */
  bool enableeffects;
  /**  */
  int numbofloop;
  /**  */
  bool loop_ok;
protected: // Protected methods
  /** No descriptions */
  void timerEvent( QTimerEvent * );
public slots: // Public slots
  /** if true Music can play if false not  */
  void enableMusic(bool );
  /** No descriptions */
  void enableEffects(bool);
private slots: // Private slots
  /** No descriptions */
  void setMusicOn();
};

#endif
