/***************************************************************************
                          dlgscores.h  -  description
                             -------------------
    begin                : Fri Feb 8 2002
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

#ifndef DLGSCORES_H
#define DLGSCORES_H

#include <qlineedit.h>
#include <qbutton.h>
#include <qlabel.h>
#include <kconfig.h>

#include <qwidget.h>
#include <dlgscoresui.h>
#include <configuration.h>


class DlgScores : public DlgScoresUI  {
   Q_OBJECT
public:
     DlgScores(QWidget *parent=0, const char *name=0,int scorep=0,QString *pname=0,KConfigBase *conf=0 );
     ~DlgScores();
  /** No descriptions */
  void init();
  /** No descriptions */
  int filltable();
private: // Private attributes
  /**  */
  int score;
  /**  */
  QString playername;
  /**  */
  KConfigBase * config;
public slots:
    void reset_scores();
};

#endif
