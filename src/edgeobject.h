/***************************************************************************
                          edgeobject.h  -  description
                             -------------------
    begin                : Sat Jan 12 2002
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

#ifndef SPRITEOBJECT_H
#define SPRITEOBJECT_H

#include <qcanvas.h>
#include "ball.h"
#include "samuele.h"

/**
  *@author Samuele Catuzzi
  */
class Ball;

class SpriteObject : public  QCanvasSprite {

public:
     SpriteObject(QCanvasPixmapArray *,QCanvas *,int Z = 10 );
     ~SpriteObject();

     /** No descriptions */
     int rtti() const;
     /**  */
     QString name;
public: //  Public attributes
     /**  */
     QCanvasPixmapArray * images;
};

#endif
