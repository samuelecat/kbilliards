/***************************************************************************
                          throwvector.h  -  description
                             -------------------
    begin                : Thu Jan 17 2002
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

#ifndef THROWVECTOR_H
#define THROWVECTOR_H

#include <qcanvas.h>
#include "ball.h"

/**
  *@author Samuele Catuzzi
  */
class Ball;

class ThrowVector : public QCanvasLine  {
public:
      ThrowVector(QCanvas *,Ball *);
      ~ThrowVector();
  /** No descriptions */
  void setdirection(int degree=0);
  /** No descriptions */
  int getResolution();
  /** No descriptions */
  void setResolution(int);
  /** No descriptions */
  double getalpha();
  /** No descriptions */
  void advance(int);
  /**
      set correct animation speed
      setfromCanvasPeriod( int msec)
      msec depend of value chosed when setAdvancePeriod(int msec)
      on the widget parent is called
  */
  void setfromCanvasPeriod(int);
public: // Public attributes
  /**  */
  int angle;
  /**  */
  int anim_speed;
private: // Private attributes
  /**  */
  Ball * white_ball;
  /**  */
  double alpha;
  /**  */
  int resolution;
  /**  */
  int length;
  /**  */
  static const double Pi;
  /**  */
  int count;
  /**  */
  int speed;
  /**  */
  int xstrt_pos;
  /**  */
  int ystrt_pos;
  /**  */
  int len;
};

#endif
