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
#ifndef BOUNCESEGMENTOB_H
#define BOUNCESEGMENTOB_H

#include "ball.h"
#include "samuele.h"

/**
@author Samuele Catuzzi
*/
class Ball;

class BounceSegmentOb{

public:
    BounceSegmentOb();

    ~BounceSegmentOb();

     /** No descriptions */
     double CPrediction(Ball * ball);
     /** No descriptions */
     void DoCollision(Ball * ball);
     /** set bounce segment from pos A to pos B */
     void setBounceLine(double Ax,double Ay,double Bx,double By);
     /** set bounce direction with module, and determine vect_n */
     void setBounceDir(double xdir,double ydir);
public: //  Public attributes
     /** direction vector */
     samuele::Vector vect_Dir;
     /** normal bounce direction */
     samuele::Vector vect_n;
     /**  */
     bool is_a_bouncer;
private:
     /** bounce segment start from pos A */
     samuele::Vector A;
     /** end in pos B */
     samuele::Vector B;
     /** r: ax + by + c = 0 */
     double a,b,c;
     /** r1: a1x + b1y + c1 = 0 */
     double a1,b1,c1;
     /** r2: a2x + b2y + c2 = 0 */
     double a2,b2,c2;

     /** internal */
     bool bounce_segment;
     /** internal use only */
     samuele::Vector Ak,Bk;
};

#endif
