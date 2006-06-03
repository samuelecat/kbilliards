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

#include "canvasview.h"
#include <qcursor.h>
#include <qcanvas.h>

#include <iostream>
using namespace std;

CanvasView::CanvasView( QCanvas *canvas, Kbilliards* parent, const char* name ) : QCanvasView( canvas, parent, name ) {
     application=parent;
     m_movingBall=0 ;
     
     //enable mouse tracking
     viewport()->setMouseTracking(TRUE);
}

CanvasView::~CanvasView() {}

void CanvasView::contentsMousePressEvent( QMouseEvent *e ) {

    if (e->button() == Qt::RightButton) {
        //cout<<"P("<<e->pos().x()<<","<<e->pos().y()<<")"<<endl;
        application->throw_ball();
    } else {

        QCanvasItemList list = canvas()->collisions( e->pos() );
        bool BallOver=false;

        //cout<<"P("<<e->pos().x()<<","<<e->pos().y()<<")"<<endl;
        if ( application->gamestatus == 1 ) {
            for ( QCanvasItemList::iterator it = list.begin(); it != list.end(); ++it )
                if ( (*it)->rtti() == 4000 ) {
                    m_movingBall = (Ball *)*it;
                    m_pos = e->pos();
                    BallOver=true;
                    break;
                }
        }
        
        if ( !BallOver ) {
            updateThrowVectorAngle (e);
            m_movingBall = 0;
        }
    }
}

void CanvasView::contentsMouseReleaseEvent ( QMouseEvent * e ) {

    m_movingBall = 0; //stop drag

}

void CanvasView::contentsMouseMoveEvent( QMouseEvent *e ) {

    Ball *i ;
    samuele::Vector mouse(e->pos().x(),e->pos().y(),0.0);
    bool BallOver=false;

    if ( application->gamestatus == 0 ) {
        m_movingBall = 0;
        QApplication::setOverrideCursor( QCursor(QCursor::ArrowCursor) );
        return;
    }

    if ( m_movingBall ) {
        m_movingBall->DragTo(e->pos().x(), e->pos().y());
        canvas()->update();
    }

    for(i=application->ingameballs.first(); i != 0 && !BallOver ; i=application->ingameballs.next()) {
        if ( (mouse - i->vect_Pos).abs() < (Ball_Physics::diameter/2 -1) ) {
           BallOver=true ;
        }
    }

    if ( BallOver ) {
        if ( application->actualgameoptions.game == 2 )
           QApplication::setOverrideCursor( QCursor(QCursor::PointingHandCursor) );
        else
           QApplication::setOverrideCursor( QCursor(QCursor::ForbiddenCursor) );
    } else {
        if (( e->pos().x() > 51 ) && ( e->pos().x() < 693 ) && ( e->pos().y() > 48 ) && (  e->pos().y() < 348 ))
           QApplication::setOverrideCursor( QCursor(QCursor::CrossCursor) ); //in carpet
        else
           QApplication::setOverrideCursor( QCursor(QCursor::ArrowCursor) );
    }
}

void CanvasView::updateThrowVectorAngle ( QMouseEvent *e ) {
    if( application->throwvector->visible()) {
        Ball * ball = application->whiteball;
        ThrowVector * vector = application->throwvector;
        double tan_val = (e->pos().y() - ball->ycenter()) / (e->pos().x() - ball->xcenter());
        int angle = (int) (atan(tan_val)*vector->getResolution()/M_PI);
        if (e->pos().x() < ball->xcenter())
                angle += vector->getResolution();
        else if (angle < 0)
                angle += vector->getResolution()*2;
        vector->setdirection(angle);
    }
}

#include "canvasview.moc"
