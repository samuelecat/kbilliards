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
#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "ball.h"
#include "kbilliards.h"
#include "configuration.h"
#include "samuele.h"
#include <qlist.h>

#define TL_HOLE     52,49
#define TM_HOLE     373,40
#define TR_HOLE     692,49

#define BL_HOLE     52,347
#define BM_HOLE     373,358
#define BR_HOLE     692,347

#include <qcanvas.h>
#include <qobject.h>

/**
@author Samuele Catuzzi
*/

class Kbilliards;

class Q_EXPORT AIplayer : public QObject {
     Q_OBJECT
public:
     AIplayer(Kbilliards *parent, QCanvas * canvas );
    ~AIplayer();

    void stroke();
    void hideTrajectories();
public:
     bool show_trajectory;
private:
     Kbilliards *kbill;
     QCanvasLine *line1;
     QCanvasLine *line2;
     QCanvasLine *line3;
     double error_factor;
private:
     bool isPathFree(Ball * from,Ball *exclude,samuele::Vector v);
     bool goInHole(samuele::Vector P,samuele::Vector tr);
     void drawLine(int id, samuele::Vector From, samuele::Vector To, QColor color);
public slots:
    void slotShowTrajectory(bool flag);
};

#endif
