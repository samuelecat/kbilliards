/***************************************************************************
                          powerbar.h  -  description
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

#ifndef POWERBAR_H
#define POWERBAR_H

#include <qframe.h>

/**
  *@author Samuele Catuzzi
  */

class Q_EXPORT PowerBar : public QFrame  {
   Q_OBJECT
public: 
   PowerBar(QWidget *parent=0, const char *name=0, WFlags f=0 );
   ~PowerBar();
   void Stop();
   void Start();
   /**  */
   int power;
   /**  */
   bool stopped;   
protected: // Protected methods
   /** No descriptions */
   void drawContents( QPainter * );
   /** No descriptions */
   void timerEvent( QTimerEvent * );
public slots: // Public slots
   /** No descriptions */
   void updateStep(int);
signals: // Signals
   /** No descriptions */
   void PBsignal(int);  
private: // Private attributes
   /**  */
   int count;
};

#endif
