/***************************************************************************
                          spriteobject.cpp  -  description
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

#include "edgeobject.h"
#include <iostream>
#include <math.h>

#include <iostream>
using namespace std;

SpriteObject::SpriteObject( QCanvasPixmapArray * bitmap,QCanvas * canvas,int Z)
        :QCanvasSprite( bitmap ,canvas) {
    images=bitmap;
    setZ(Z);
    show();
    name="Edge";
}

SpriteObject::~SpriteObject() {
    delete(images);
}

/** No descriptions */
int SpriteObject::rtti() const {
    return 2000 ;
}

