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
#include "bouncesegmentob.h"
#include <iostream>
#include <math.h>


using namespace std;

BounceSegmentOb::BounceSegmentOb()
{
    vect_Dir.null();
    vect_n.null();
    A.null();
    B.null();
}

BounceSegmentOb::~BounceSegmentOb()
{
}

/** No descriptions */
void BounceSegmentOb::setBounceLine(double Ax,double Ay,double Bx,double By) {

    samuele::Vector norm, J, P;

    A.x = Ax;
    A.y = Ay;
    A.z = 0.0 ;

    B.x = Bx;
    B.y = By;
    B.z = 0.0 ;

    // error check
    if (( Ax == Bx )&&( Ay == By )||( (B - A).abs() < 3 )) {
        cout<<"ERROR: TOO SMALL EDGE SEGMENT, LENGTH MUST BE > 3 "<<endl;
        return  ;
    } else
        bounce_segment = true;

    // used internally for better bounce around points A and B
    Ak = A + (B - A).unit();
    Bk = B - (B - A).unit();

    // r: ax + by + c = 0
    a = B.y - A.y ;
    b = A.x - B.x ;
    c = A.y*(B.x - A.x) - A.x*(B.y - A.y) ;

    //normal vector segment
    norm.x = (B - A).y ;
    norm.y = - (B - A).x ;

    // r1: a1x + b1y + c1 = 0
    J = norm + A;
    a1 = J.y - A.y ;
    b1 = A.x - J.x ;
    c1 = A.y*(J.x - A.x) - A.x*(J.y - A.y) ;

    // r2: a2x + b2y + c2 = 0
    J = norm + B;
    a2 = J.y - B.y ;
    b2 = B.x - J.x ;
    c2 = B.y*(J.x - B.x) - B.x*(J.y - B.y) ;

    //middle point from A to B
    P = A + (B - A)*0.5 ;

    // *must be:  r1 < P(x,y) < r2

    // I want  P > r1
    if (( a1*P.x + b1*P.y + c1 ) < 0 ) {
        if ( a1 != 0 )
            a1 = -a1 ; // (if ..) prevent assignment " -0 "
        if ( b1 != 0 )
            b1 = -b1 ;
        if ( c1 != 0 )
            c1 = -c1 ;
    }

    // I want  P < r2
    if (( a2*P.x + b2*P.y + c2 ) > 0 ) {
        if ( a2 != 0 )
            a2 = -a2 ; // (if ..) prevent assignment " -0 "
        if ( b2 != 0 )
            b2 = -b2 ;
        if ( c2 != 0 )
            c2 = -c2 ;
    }

}

/** No descriptions */
void BounceSegmentOb::setBounceDir(double xdir,double ydir) {

    samuele::Vector v1,P;

    vect_Dir.x = xdir;
    vect_Dir.y = ydir;
    vect_Dir.z = 0.0;

    if ( ! bounce_segment ) {
        cout<<"ERROR: FIRST SET BounceLine THEN SET BounceDirection"<<endl;
        return;
    }

    // error if  ( A == B ) or (vect_Dir == null) !!
    if ( ( xdir == 0.0) && (ydir == 0.0) ) {
        cout<<"ERROR: INVALID BOUNCE DIRECTION (BounceDir == null)"<<endl;
        return ;
    }

    //determine normal segment direction concorde with vect_Dir
    v1.x = (B - A).y ;
    v1.y = - (B - A).x ;
    v1 = v1.unit();
    if ( v1.x == 0 )
        v1.x = 0.0 ; // remove  +0  -0
    if ( v1.y == 0 )
        v1.y = 0.0 ;

    if ( (v1*(vect_Dir.unit())) > 0 )
        vect_n=v1;
    else {
        vect_n.x = - v1.x;
        vect_n.y = - v1.y;
        vect_n.z = 0.0 ;
        if ( vect_n.x == 0 )
            vect_n.x = 0.0 ; // remove  +0  -0
        if ( vect_n.y == 0 )
            vect_n.y = 0.0 ;
    }

    if ( (v1*(vect_Dir.unit())) == 0 ) {
        cout<<"ERROR: INVALID BOUNCE DIRECTION (BounceDir parallel to edge segment)"<<endl;
        return ;
    }

    // I want  P > r
    P = A + vect_Dir*10 ;
    if (( a*P.x + b*P.y + c ) < 0 ) {
        if ( a != 0 )
            a = -a ; // (if ..) prevent assignment " -0 "
        if ( b != 0 )
            b = -b ;
        if ( c != 0 )
            c = -c ;
    }

    is_a_bouncer = true;
}

/** No descriptions */
double BounceSegmentOb::CPrediction(Ball * ball) {

    if ( !is_a_bouncer )
        return -1;

    samuele::Vector P,K,J;
    double time,delta,deltax,deltay;
    double ak,bk,ck,my_sqrt;

    // vel = 0 , the ball is stopped
    if (( ball->vect_Vel.x == 0.0 )&&( ball->vect_Vel.y == 0.0 ))
        return -1 ;


    double dist; //usefull "dist" on debug operations
    dist=a*(ball->vect_Pos.x) + b*(ball->vect_Pos.y) + c ;
    if ( dist < 0 )
        dist = -dist ;
    dist = dist/sqrt(a*a + b*b);


    // is come to this segment ?
    if (( (ball->vect_Vel.unit()*vect_n) > 0 )||(( a*(ball->vect_Pos.x) + b*(ball->vect_Pos.y) + c ) <= 0 ))
        return -1 ;

    P = ball->vect_Pos - ( vect_n*(ball->diameter/2) ) ;

    // rk from P position
    J = P + ball->vect_Vel ;
    ak = J.y - P.y ;
    bk = P.x - J.x ;
    ck = P.y*(J.x - P.x) - P.x*(J.y - P.y) ;

    delta = a*bk - b*ak ;
    if ( delta == 0 )
        return -1; // rk parallel to r
    deltax = -c*bk + b*ck ;
    deltay = -a*ck + c*ak ;

    // K (a point on r)
    K.x = deltax/delta ;
    K.y = deltay/delta ;

    //if (( ball->ballnumber == 0 )&&(dist < 30)) cout<<" --- "<<endl;
    //if (( ball->ballnumber == 0 )&&(dist < 30)) cout<<" edgenum="<<edgenumber<<" K("<<K.x<<","<<K.y<<") "<<endl;

    // intersect segment?
    if (( ( a1*K.x + b1*K.y + c1 ) >= 0 )&&( ( a2*K.x + b2*K.y + c2 ) <= 0 )) {

        time = ((K - P).abs())/(ball->vect_Vel.abs());

    } else {

        P = ball->vect_Pos ;

        // rk from ball->vect_Pos
        J = P + ball->vect_Vel ;
        ak = J.y - P.y ;
        bk = P.x - J.x ;
        ck = P.y*(J.x - P.x) - P.x*(J.y - P.y) ;

        // Bounce out from margin points Ak,Bk (near A,B points)

        my_sqrt = sqrt(ak*ak + bk*bk);

        if (( fabs(ak*Ak.x + bk*Ak.y + ck)/my_sqrt ) <= ((ball->diameter)/2) ) {
            J = Ak - P;
            time = (J.abs() - (ball->diameter)/2)/(ball->vect_Vel*(J.unit()));
        } else if (( fabs(ak*Bk.x + bk*Bk.y + ck)/my_sqrt ) <= ((ball->diameter)/2) ) {
            J = Bk - P;
            time = (J.abs() - (ball->diameter)/2)/(ball->vect_Vel*(J.unit()));
        } else
            time = -1 ;

    }

    //if (( ball->ballnumber == 0 )&&(dist < 30)) cout<<" edgenum="<<edgenumber<<" time="<<time<<endl;

    return time;
}

/** No descriptions */
void BounceSegmentOb::DoCollision(Ball * ball) {

    samuele::Vector P, n2 ;
    double xvel,yvel;
    double xhit,yhit ;
    double modhit,projection,speed;
    double dist; //usefull "dist" on debug operations
    dist=a*(ball->vect_Pos.x) + b*(ball->vect_Pos.y) + c ;
    xvel=ball->vect_Vel.x;
    yvel=ball->vect_Vel.y;
    xhit=vect_Dir.x ;
    yhit=vect_Dir.y ;

    if (xhit != 0 || yhit != 0)  // x & y direction == 0 -> no bounce
    {
        P = ball->vect_Pos ;
        n2 = (B - A).unit();

        modhit=sqrt(xhit*xhit + yhit*yhit);
        projection=(xhit/modhit)*xvel + (yhit/modhit)*yvel;

        if ( projection < 0 ) // the ball direction is opposite of the edge one
        {
            if (( a1*(P.x) + b1*(P.y) + c1 ) <= 0 ) {// ball going to A point
                n2 = (P - A).unit(); // bounce out direction from A point
                projection = ball->vect_Vel*n2 ;
                if ( projection < 0 ) {
                    xvel=xvel - 2*projection*(n2.x);
                    yvel=yvel - 2*projection*(n2.y);
                    //if ( (P - A).abs() < ( Ball_Physics::diameter / 2 ) )
                    //    cout<<"P-A ="<<(P - A).abs()<<endl;
                }
            } else if (( a1*(P.x) + b1*(P.y) + c1 ) <= 0 ) { // ball going to B point
                n2 = (P - B).unit(); // bounce out direction from B point
                projection = ball->vect_Vel*n2 ;
                if ( projection < 0 ) {
                    xvel=xvel - 2*projection*(n2.x);
                    yvel=yvel - 2*projection*(n2.y);
                    //if ( (P - B).abs() < ( Ball_Physics::diameter / 2 ) )
                    //    cout<<"P-B ="<<(P - B).abs()<<endl;
                }
            } else {
                xvel=xvel - 2*projection*xhit;
                yvel=yvel - 2*projection*yhit;
                //if ( dist < ( Ball_Physics::diameter / 2 ) )
                //    cout<<"dist ="<<dist<<endl;
            }

            ball->setVelocity(xvel,yvel);

            //***** ball-edge resistance

            speed=ball->vect_Vel.abs();
            //cout<<"pre speed="<<speed<<endl;
            if ( speed < (ball->hitedgedecel) ) { // stop a ball !
                ball->vect_Vel.null() ;
            } else {
                speed -= ball->hitedgedecel ;
                ball->vect_Vel = (ball->vect_Vel.unit()) * speed ;
            }
            //cout<<"post speed="<<speed<<endl;

        }
        if ( projection == 0 ) // the ball direction is tangent of the edge line
        {
            //cout<<"ball="<<ballnumber<<" tangent to edge num="<<edg->edgenumber<<endl;
            //little bounce out
            ball->vect_Vel = ball->vect_Vel + (vect_Dir.unit()) * 0.01 ;
        }
    }
}


