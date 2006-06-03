/***************************************************************************
                          powerbar.cpp  -  description
                             -------------------
    begin                : Thu Jan 31 2002
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

#include "powerbar.h"
#include <qpainter.h>
#include <qdrawutil.h>
#include <qapplication.h>
#include <limits.h>


PowerBar::PowerBar(QWidget *parent, const char *name, WFlags f ) : QFrame( parent, name, f )
{
 power=0;
 count=0;
 setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
 setMinimumSize( 75, 32 );
 show();

 setFrameStyle(QFrame::NoFrame);
 setLineWidth( 1 );
 
 connect(this,SIGNAL(PBsignal(int)),this,SLOT(updateStep(int)));
 startTimer( 140 );
}
PowerBar::~PowerBar(){
}
/** No descriptions */
void PowerBar::timerEvent( QTimerEvent *) {
 if ( !stopped ){
     count++;
     
     if ( count >= 20 )
          count=0;
     
     if (( count >= 0 ) && (count <11) )
          emit PBsignal(count);
     else if ( count >= 11 )
          emit PBsignal(10 - count%10);
 }
}
/** No descriptions */
void PowerBar::updateStep(int number){
 power=number;
 repaint(TRUE);
}
/** No descriptions */
void PowerBar::drawContents( QPainter *paint )
{
 paint->setPen( QColor(blue) );

 paint->drawRect(66, 3,5,26);
 paint->drawRect(59, 9,5,20);
 paint->drawRect(52,14,5,15);
 paint->drawRect(45,17,5,12);
 paint->drawRect(38,19,5,10);
 paint->drawRect(31,21,5,8 );
 paint->drawRect(24,22,5,7 );
 paint->drawRect(17,23,5,6 );
 paint->drawRect(10,24,5,5 );
 paint->drawRect( 3,24,5,5 );

 switch(power)
     {
      case 10: paint->fillRect(66, 3,5,26, QBrush( QColor(blue)) );
      case 9:  paint->fillRect(59, 9,5,20, QBrush( QColor(blue)) );
      case 8:  paint->fillRect(52,14,5,15, QBrush( QColor(blue)) );
      case 7:  paint->fillRect(45,17,5,12, QBrush( QColor(blue)) );
      case 6:  paint->fillRect(38,19,5,10, QBrush( QColor(blue)) );
      case 5:  paint->fillRect(31,21,5,8 , QBrush( QColor(blue)) );
      case 4:  paint->fillRect(24,22,5,7 , QBrush( QColor(blue)) );
      case 3:  paint->fillRect(17,23,5,6 , QBrush( QColor(blue)) );
      case 2:  paint->fillRect(10,24,5,5 , QBrush( QColor(blue)) );
      case 1:  paint->fillRect( 3,24,5,5 , QBrush( QColor(blue)) );
     }
}


/*!
    \fn PowerBar::Stop()
 */
void PowerBar::Stop()
{
     stopped=true;
     power=0;
     count=0;
     emit PBsignal(0);
}


/*!
    \fn PowerBar::Start()
 */
void PowerBar::Start()
{
     stopped=false;
}

#include "powerbar.moc"

