/***************************************************************************
 *   Copyright (C) 2004 by Samuele Catuzzi                                 *
 *   samuele_catuzzi@yahoo.it, samuele.c@hostnotfound.it                   *
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
#include "holeobject.h"

HoleObject::HoleObject( double x, double y, double r ) {
    xc = x ;
    yc = y ;
    radius = r ;
}


HoleObject::~HoleObject() {}

/*!
    \fn HoleObject::isInHole()
 */
bool HoleObject::isInHole(Ball *b) {
    double x,y;

    x = b->vect_Pos.x ;
    y = b->vect_Pos.y ;

    if ( (( x - xc )*( x - xc ) + ( y - yc)*( y - yc)) < radius*radius )
        return true ;

    return false;
}

