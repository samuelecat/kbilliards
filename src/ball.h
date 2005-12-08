/***************************************************************************
                          ball.h  -  description
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

#ifndef BALL_H
#define BALL_H

#include <qcanvas.h>
#include "samuele.h"
#include "kbilliards.h"
#include "edgeobject.h"


/**
  *@author Samuele Catuzzi
  */
class Kbilliards;

class  Ball_Physics {
public:
    Ball_Physics();

public:
    static const double mass;
    static const double diameter;

    /**  position of center ball */
    samuele::Vector vect_Pos;
    /**  velocity */
    samuele::Vector vect_Vel;
};

class Ball : public QCanvasSprite, public Ball_Physics {
public:
    Ball(QCanvasPixmapArray *,QCanvasPixmapArray *,QCanvas *,Kbilliards *,int,int Z = 9 );
    ~Ball();
    /** No descriptions */
    void advance(int);
    /** No descriptions */
    int rtti() const;
    /** No descriptions */
    void setPosition(double ,double );
    /** No descriptions */
    void DragTo(double ,double );
    /** No descriptions */
    void setVelocity(double ,double );
    /** No descriptions */
    double xVelocity();
    /** No descriptions */
    double yVelocity();
    /** No descriptions */
    double xcenter();
    /** No descriptions */
    double ycenter();
    /** No descriptions */
    bool stopped();
    /** No descriptions */
    double BallCPrediction(Ball *);
    /** No descriptions */
    void DoBallCollision(Ball * );
    /** if true is animated , if false is stopped */
    void setAnimateL(bool);
    /** reimplemented */
    void show();
    void hide();
public: // Public attributes
    /**  */
    QCanvasPixmapArray * images;
    /**  */
    double deceleration ;
    /**  */
    int ballnumber;
    /**  */
    double hitedgedecel;
    /**  */
    double hitballdecel;
    /** if true the ball is animated */
    bool light;
    /**  */
    bool is_draggable;
private: // Private attributes
    /**  */
    QCanvasSprite *shadow;
    /**  */
    Kbilliards * application;
    /**  */
    int anim_speed;
    /**  */
    int framespeed;
};

#endif
