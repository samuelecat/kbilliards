/***************************************************************************
                          mcanvas.cpp  -  description
                             -------------------
    begin                : Wed Feb 13 2002
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

#include "mcanvas.h"

MCanvas::MCanvas(QWidget* parent,int w,int h) : QCanvas(w,h) {
    clock=0;
    par=parent;
    setDoubleBuffering(TRUE);
}
MCanvas::~MCanvas() {}
/** No descriptions */
void MCanvas::setAdvancePeriod ( int ms ) {
    if ( ms<0 ) {
        if ( clock )
            clock->stop();
    } else {
        if ( !clock ) {
            clock = new QTimer(this);
            connect(clock,SIGNAL(timeout()),this,SLOT(advance()));
            connect(clock,SIGNAL(timeout()),par,SLOT(clock()));
        }
        clock->start(ms);
    }
}
/** No descriptions */
void MCanvas::setUpdatePeriod(int ms) {
    if ( ms<0 ) {
        if ( clock )
            clock->stop();
    } else {
        if ( !clock ) {
            clock = new QTimer(this);
            connect(clock,SIGNAL(timeout()),this,SLOT(advance()));
            connect(clock,SIGNAL(timeout()),par,SLOT(clock()));
        }
        clock->start(ms);
    }

}

#include "mcanvas.moc"

