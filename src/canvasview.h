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
#ifndef CANVASVIEW_H
#define CANVASVIEW_H

#include <qcanvas.h>
#include "ball.h"

/**
@author Samuele Catuzzi
*/
class CanvasView : public QCanvasView
{
 Q_OBJECT
public:
     CanvasView( QCanvas *canvas, Kbilliards* parent , const char* name = "canvas view" ) ;

     ~CanvasView();

protected:
     void contentsMousePressEvent( QMouseEvent *e );
     void contentsMouseReleaseEvent( QMouseEvent *e );
     void contentsMouseMoveEvent( QMouseEvent *e );
     void updateThrowVectorAngle ( QMouseEvent *e );

private:
     Ball *m_movingBall;
     QPoint m_pos;
     Kbilliards * application;
};


#endif
