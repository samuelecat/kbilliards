/***************************************************************************
                          kbilliards.h  -  description
                             -------------------
    begin                : gio gen 24 13:37:53 CET 2002
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

#ifndef KBILLIARDS_H
#define KBILLIARDS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <kapp.h>
#include <qwidget.h>
#include <kmainwindow.h>
#include <ktoolbar.h>
#include <kconfig.h>
#include <qcanvas.h>
#include <qlist.h>
#include <qlcdnumber.h>
#include <qmutex.h>

#include "powerbar.h"
#include "scoredisplay.h"
#include "edgeobject.h"
#include "holeobject.h"
#include "bouncesegmentob.h"
#include "ball.h"
#include "throwvector.h"
#include "psettings.h"
#include "configuration.h"
#include "dlgscores.h"
#include "mcanvas.h"
#include "soundmodule.h"  // not work well :(
#include "aiplayer.h"

class Ball;
class SpriteObject;
class HoleObject;
class BounceSegmentOb;
class ThrowVector;
class AIplayer;

/** Kbilliards is the base class of the project */
class Kbilliards : public KMainWindow
{
  Q_OBJECT
public:
  /** construtor */
  Kbilliards(QWidget* parent=0, const char *name=0);
  /** destructor */
  ~Kbilliards();
  /** No descriptions */
  void placeObjects();
  /** No descriptions */
  void keyPressEvent( QKeyEvent * );
  /** No descriptions */
  void placeBalls(int,int);
  /** No descriptions */
  void BallinHole(int);
  /** No descriptions */
  void loadSettings();
  /** No descriptions */
  void loadGameOptions();
  /** No descriptions */
  void saveGameOptions();
  /** check to see if any other ball than mine occupies the area at this coordinates */
  bool otherBallpresence(int myballnumber,double x,double y);
  /** enable two player mode */
  void enableTwoPlayersMode( bool );
public: // Public attributes
  /**  */
  Configuration defaultconfig;
  /**  */
  Configuration actualconfig;
  /**  */
  MusicConfiguration actualmusicconfig;
  /**  */
  GameOptions actualgameoptions;
  /**  */
  KMenuBar * menu;
  /**  */
  KStatusBar * status;
  /**  */
  KToolBar * toolbar;
  /**  */
  PowerBar * powerbar;
  /**  */
  ScoreDisplay * scoredisplay;
  /**  */
  ThrowVector * throwvector;
  /**  */
  QList<Ball> ingameballs;
  /**  */
  QList<Ball> outgameballs;
  /**  */
  QList<SpriteObject> spriteobjects;
  /**  */
  QList<BounceSegmentOb> edgeobjects;
  /**  */
  QList<HoleObject> holeobjects;
  /**  */
  double MaxThrowPower;
  /**  */
  KConfigBase * config;
  /**  */
  int nextBallseq;
  /**  */
  int firstHittedBall;
  /**  */
  bool ballinhole;
  /**  */
  bool checkballstatus;
  /**  */
  SoundModule * Music;
  /** balls */
  Ball * whiteball;
  /** 0 - game not running
  1 - game started and running */
  int gamestatus;
  AIplayer *AIp;
  /**  */
  QCanvas * canvas;

protected: // Protected attributes
  /**  */
  QCanvasView * BilliardArea;

public slots: // Public slots
  /** No descriptions */
  void slotAbout();
  /** No descriptions */
  void slotAboutKDE();
  /** No descriptions */
  void throw_ball();
  /** No descriptions */
  void newGame();
  /** show configuration window dialog */
  void confdialog();
  /** No descriptions */
  void saveSettings();
  /** No descriptions */
  void viewScores();
  /** No descriptions */
  void clock();
protected: // Protected methods
  /** No descriptions */
  void timerEvent( QTimerEvent *);
protected slots: // Protected slots
  /** No descriptions */
  void move_left_vector() ;
  /** No descriptions */
  void move_right_vector() ;
private: // Private attributes
  /**  */
  QString path_data;
  /**  */
  QMutex mutex;
  /** when begin new game increase power on first fire and other things.. */
  bool first_fire;
  int currentPlayerNum;
  int previousPlayerNum;
private: // Private methods
  /** No descriptions */
  void winner(int plnumber = 1);
  void activateNextPlayer();
  void initgamedialog();
  int loadMap(QString filename);
};

#endif
