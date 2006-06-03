/***************************************************************************
                          ball.cpp  -  description
                             -------------------
    begin                : Mon Jan 14 2002
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

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <bitset>
#include <math.h>
#include "ball.h"

using namespace std;

const double Ball_Physics::mass = 1;
const double Ball_Physics::diameter = 20;

Ball_Physics::Ball_Physics() {
    vect_Pos.null();
    vect_Vel.null();
}

Ball::Ball(QCanvasPixmapArray *bitmap,QCanvasPixmapArray *shadow_bitmap,QCanvas *canvas,Kbilliards * applic,int number,int Z ) :QCanvasSprite( bitmap ,canvas) {
    images=bitmap;
    application=applic;
    setZ(Z);
    ballnumber=number;
    deceleration = 0.028;
    hitedgedecel = 0.020;
    hitballdecel = 0.010;
    setVelocity(0,0);
    anim_speed=12;
    framespeed=0;
    light=false;
    is_draggable=false;
    //graphicvector=new GraphicVector(canvas); //experimental
    shadow=new QCanvasSprite( shadow_bitmap ,canvas);
    shadow->setZ(1); //under all objects
    //graphicvector->show(); //experimental
}

Ball::~Ball() {
    delete(images);
}

/** No descriptions */
void Ball::advance(int stage) {

    setPosition(vect_Pos.x,vect_Pos.y);

    switch(stage) {
    case 0:
        if (visible()) {
            //if (ballnumber == 1)
            //     cout<<"Ball:"<<ballnumber<<"Position X,Y ="<<vect_Pos.x<<","<<vect_Pos.y<<endl;

            for(HoleObject *hole=application->holeobjects.first(); hole != 0; hole=application->holeobjects.next()) {
                if ( hole->isInHole(this) ) {
                    hide();
                    application->ingameballs.remove(this);
                    application->outgameballs.append(this);
                    setVelocity(0,0);
                    application->BallinHole(ballnumber);
                    break;
                }
            }

            double xvel = vect_Vel.x ;
            double yvel = vect_Vel.y ;
            double speed;

            speed=sqrt(xvel*xvel + yvel*yvel);

            double cos,sin;
            cos = xvel/speed ;
            sin = yvel/speed ;

            if ( speed < deceleration ) { // stop a ball !
                xvel=0;
                yvel=0;
                speed=0;
            } else {
                speed -= deceleration ;
                xvel=speed*cos ;
                yvel=speed*sin ;
            }

            setVelocity(xvel,yvel);

            //update shadow position:
            shadow->move(x()-3,y()+3);
            //no shadow near bottom border:
            if (( vect_Pos.y > 352 )||( vect_Pos.y < 46 ))
                shadow->hide();
            else
                shadow->show();

        } else {
            //if ball is not visible, then shadow hide()
            shadow->hide();
        }
        break;
    case 1:

        framespeed=(framespeed + 1)%anim_speed;// animation speed
        if (( framespeed == 0 ) && light ) {
            if ( frame() == 0 ) {
                setFrame(1);
                shadow->setFrame(1);
            } else {
                setFrame(0);
                shadow->setFrame(0);
            }
        }
        break;
    } // end switch

    //graphicvector->setDirection(vect_Pos.x,vect_Pos.y,vect_Pos.x + vect_Vel.x,vect_Pos.y + vect_Vel.y); //experimental
}

/** No descriptions */
int Ball::rtti() const {
    return 4000;
}

/** No descriptions */
void Ball::setPosition(double xval,double yval) {
    vect_Pos.x=xval;
    vect_Pos.y=yval;
    move(xval-(diameter/2),yval-(diameter/2));
    shadow->move(xval-(diameter/2) -3,yval-(diameter/2) +3);
}

/** No descriptions */
void Ball::DragTo(double xval,double yval) {
    if ( is_draggable && ( xval > 51 ) && ( xval < 693 ) && ( yval > 48 ) && ( yval < 348 ))  {
        if ( ! application->otherBallpresence( ballnumber,xval,yval) ) {
            vect_Pos.x=xval;
            vect_Pos.y=yval;
            move(xval-(diameter/2),yval-(diameter/2));
            shadow->move(xval-(diameter/2) -3,yval-(diameter/2) +3);
        }
    }
}

/** No descriptions */
void Ball::setVelocity(double xval,double yval) {
    vect_Vel.x=xval;
    vect_Vel.y=yval;
    QCanvasSprite::setVelocity(xval,yval);
}

/** No descriptions */
double Ball::xVelocity() {
    return  vect_Vel.x;
}

/** No descriptions */
double Ball::yVelocity() {
    return  vect_Vel.y;
}

/** No descriptions */
double Ball::xcenter() {
    return  vect_Pos.x; //cambiato
}

/** No descriptions */
double Ball::ycenter() {
    return  vect_Pos.y; //cambiato
}

/** No descriptions */
bool Ball::stopped() {
    return (( xVelocity() == 0 ) && ( yVelocity() == 0 )) ;
}

/** Time left before ball to ball collision ( -1 no collision ) */
double Ball::BallCPrediction(Ball *B) {

    double deltaVx,deltaVy,dx,dy;
    long double a,b,c,my_sqrt;
    double result1,result2;

    //vettore differenza
    deltaVx = vect_Vel.x - B->vect_Vel.x;
    deltaVy = vect_Vel.y - B->vect_Vel.y;
    //distanza
    dx = vect_Pos.x - B->vect_Pos.x;
    dy = vect_Pos.y - B->vect_Pos.y;

    a = deltaVx*deltaVx + deltaVy*deltaVy;

    if ( a == 0.0 )
        return -1 ;

    b = 2 * (dx*deltaVx + dy*deltaVy);
    c = dx*dx + dy*dy - diameter*diameter ;

    my_sqrt = b*b - 4*a*c ;
    if ( my_sqrt < 0 )
        return -1;

    result1=( ( -b - sqrtl(my_sqrt) ) / (2*a) );
    result2=( ( -b + sqrtl(my_sqrt) ) / (2*a) );

/*
    if ( ( result1 > result2 )&&( ((result1 < 1)&&(result1 > 0))||((result2 < 1)&&(result2 > 0)) ))
        cout<<"result1="<<result1<<" < result2="<<result2<<endl;
*/

    //qui sotto filtro via l'evento di due palle che si sfiorano tangentemente in un solo punto
    //cioè non compenetrano ma si toccano, non va considerarla come collisione o si ottiene un loop infinito.
    if (( result1 == result2 )&&( result1 == 0.0 ))
        return -1;

    return result1;

}

/** No descriptions */
void Ball::DoBallCollision(Ball *b) {

    /*
    if ( ( this->ballnumber == 1 ) || ( b->ballnumber == 1 ) ) {
        cout<<"DoBallCollision"<<endl;
        cout<<"distance="<<((this->vect_Pos - b->vect_Pos).abs())<<endl;
        cout<<"Ball="<<this->ballnumber<<" vect_Vel X,Y ="<<this->vect_Vel.x<<","<<this->vect_Vel.y<<endl;
        cout<<"       vect_Pos X,Y ="<<this->vect_Pos.x<<","<<this->vect_Pos.y<<endl;
        cout<<"Ball="<<b->ballnumber<<" vect_Vel X,Y ="<<b->vect_Vel.x<<","<<b->vect_Vel.y<<endl;
        cout<<"       vect_Pos X,Y ="<<b->vect_Pos.x<<","<<b->vect_Pos.y<<endl;
          }
    */

    samuele::Vector vel,bvel,vdiff,pdiff_unit;

    double projection,pr1,pr2;
    double speed;

    pdiff_unit = (b->vect_Pos - vect_Pos).unit() ;

    pr1=vect_Vel*pdiff_unit ;
    pr2=b->vect_Vel*pdiff_unit ;

    //determine if each ball is going to approach other one
    if ( ( pr1 <= 0 )&&( pr2 >= 0 ) || ( (pr1 - pr2) < 0 ) ) return;

    //first determine if balls have enough kinetic energy
    speed=vect_Vel.abs() ;
    if ( speed < hitballdecel ) { // stop a ball !
        vect_Vel.null() ;
    } else {
        speed -= hitballdecel ;
        vect_Vel = (vect_Vel.unit()) * speed ;
    }

    speed=b->vect_Vel.abs() ;
    if ( speed < hitballdecel ) { // stop a ball !
        b->vect_Vel.null() ;
    } else {
        speed -= hitballdecel ;
        b->vect_Vel = (b->vect_Vel.unit()) * speed ;
    }

    vdiff = b->vect_Vel - vect_Vel ;
    projection = vdiff*pdiff_unit ;

    vel= vect_Vel + (pdiff_unit*projection);
    bvel = b->vect_Vel - (pdiff_unit*projection);

    setVelocity(vel.x,vel.y);
    b->setVelocity(bvel.x,bvel.y);

}

/** if true is animated , if false is stopped */
void Ball::setAnimateL(bool anim) {
    if ( anim )
        framespeed = 0;
    else {
        setFrame(0);
        shadow->setFrame(0);
    }
    light=anim;
}


void Ball::show() {
    setVisible(TRUE);
    shadow->setVisible(TRUE);
}

void Ball::hide() {
    setVisible(FALSE);
    shadow->setVisible(FALSE);
}
