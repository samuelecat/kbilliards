/***************************************************************************
 *   Copyright (C) 2004 by Samuele Catuzzi                                 *
 *   samuele_catuzzi@yahoo.it                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef HOLEOBJECT_H
#define HOLEOBJECT_H

#include <qobject.h>

#include "kbilliards.h"
#include "ball.h"

/**
@author Samuele Catuzzi
*/

class Kbilliards;
class Ball;

class HoleObject : public QObject
{
public:
    HoleObject( double x, double y, double radius );

    ~HoleObject();
    bool isInHole(Ball *b) ;
public:
    double xc,yc,radius ;
    int holenumber ;
};

#endif
