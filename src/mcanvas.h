/***************************************************************************
                          mcanvas.h  -  description
                             -------------------
    begin                : Wed Feb 13 2002
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

#ifndef MCANVAS_H
#define MCANVAS_H

#include <qwidget.h>
#include <qcanvas.h>
#include <qtimer.h>

/**
  *@author Samuele Catuzzi
  */

class MCanvas : public QCanvas  {
   Q_OBJECT
public: 
	MCanvas(QWidget* parent,int w, int h);
	~MCanvas();
public: // Public attributes
  /**  */
  QTimer * clock;
  /** No descriptions */
  void setAdvancePeriod ( int ms );
  /** No descriptions */
  void setUpdatePeriod(int ms);
private: // Private attributes
  /**  */
  QWidget * par;
};

#endif
