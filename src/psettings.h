/***************************************************************************
                          psettings.h  -  description
                             -------------------
    begin                : Mon Feb 4 2002
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

#ifndef PSETTINGS_H
#define PSETTINGS_H

#include <qwidget.h>
#include "configuration.h"
#include "confdialog.h"

#include <qlineedit.h>
#include <qbutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qpushbutton.h>

/**
  *@author Samuele Catuzzi
  */

class PSettings : public confdialog
{
  Q_OBJECT
public: 
	PSettings(QWidget *parent=0, const char *name=0, Configuration *conf=0);
	~PSettings();
  /** No descriptions */
  bool checkvalues();
private: // Private attributes
  /**  */
  Configuration * configuration;
  /**  */
  double decel;
  /**  */
  double hitba;
  /**  */
  double hited;
  /**  */
  int check;
  /**  */
  int angle;
public slots: // Public slots
  /** No descriptions */
  void slot_update();
  /** No descriptions */
  void slot_ok();
  /** No descriptions */
  void slot_close();
};

#endif
