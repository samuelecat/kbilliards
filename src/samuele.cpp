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
#include "samuele.h"

namespace samuele {

Vector::Vector()
{
   x=0.0;
   y=0.0;
   z=0.0;
}

Vector::Vector(double xval,double yval,double zval)
{
   x=xval;
   y=yval;
   z=zval;
}

Vector::Vector(double xval,double yval)
{
   x=xval;
   y=yval;
   z=0.0;
}

Vector::~Vector()
{
}

Vector Vector::operator +(Vector v)
{
   Vector result;
   result.x = x + v.x;
   result.y = y + v.y;
   result.z = z + v.z;
   return result;
}

Vector Vector::operator -(Vector v)
{
   Vector result;
   result.x = x - v.x;
   result.y = y - v.y;
   result.z = z - v.z;
   return result;
}

double Vector::operator *(Vector v)
{
   return ( x*v.x + y*v.y + z*v.z );
}

Vector Vector::operator *(double delta)
{
   Vector result;
   result.x = x*delta;
   result.y = y*delta;
   result.z = z*delta;
   return result;
}

Vector Vector::unit(){
   Vector result;
   double module;
   module=sqrt( x*x + y*y + z*z );

   if( module > 0 ){
       result.x=x/module;
       result.y=y/module;
       result.z=z/module;
   } else {
       result.x=0.0;
       result.y=0.0;
       result.z=0.0;
   }
   return result;
}

double Vector::abs(){
   return ( sqrt( x*x + y*y + z*z ) );
}

void Vector::null(){
   x=0.0;
   y=0.0;
   z=0.0;
}

void Vector::set(double xval,double yval,double zval)
{
   x=xval;
   y=yval;
   z=zval;
}

Vector Vector::projection( Vector v1, Vector v2 )
{
   double v2ls;

   v2ls = v2.x*v2.x + v2.y*v2.y + v2.z*v2.z;

   if( v2ls > 0.0 ){
       return (  v2 * ((v1*v2)/v2ls) );
   } else {
       return ( v1 );
   }
}

};
