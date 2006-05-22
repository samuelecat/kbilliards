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
//********************************************************
//    This class is similar to kimagenumbers from kodo
//    thanks to Armen Nakashian
//********************************************************

#ifndef KIMAGENUMBERS_H
#define KIMAGENUMBERS_H

/**
@author Samuele Catuzzi <samuele.c@hostnotfound.it>
*/

#include <qframe.h>
#include <qpixmap.h>

class KImageNumbers : public QFrame {
	Q_OBJECT
public:
    KImageNumbers(const QString& font, QWidget* parent=0, const char* name=0);

    ~KImageNumbers();
    void display(int);
    void paintEvent(QPaintEvent*);
protected:
    int m_value;
    QPixmap* img;
};

#endif
