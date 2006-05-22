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

#ifndef SCOREDISPLAY_H
#define SCOREDISPLAY_H

#include <qvariant.h>
#include <qwidget.h>
#include <qevent.h>
#include <qstring.h>
#include "kimagenumbers.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QLabel;
class KImageNumbers;
class KLed;

class ScoreDisplay : public QWidget
{
Q_OBJECT
public:
     /** No descriptions */
     ScoreDisplay( QString path, QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
     /** No descriptions */
     ~ScoreDisplay();

     /** No descriptions */
     void enableTwoPlayersMode(bool);

     /** No descriptions */
     void setScorePlayerOne(int);
     /** No descriptions */
     void setScorePlayerTwo(int);
     /** No descriptions */
     void setCurrentPlayer(int);

     /** No descriptions */
     QString getPlayerName(int player_num);
     /** No descriptions */
     int getPlayerScore(int player_num);

     /** No descriptions */
     void setPlayerName(int player_num, QString name);

     /** add score No descriptions */
     void addScorePlayerNum(int playernum,int addscore);
protected: // Protected methods
     /** No descriptions */
     void timerEvent( QTimerEvent * );
private:
     /** No descriptions */
     int score_player_1;
     int score_player_2;
     int currentPlayerNum;
     /** number of times that light of diode switch on-off */
     int repeat_light;

     /** 0 original color, 1 alternative color, 3 stop switchig */
     int frameBorder_color_state1;
     int frameBorder_color_state2;

     bool TwoPlayersMode;

     QFrame* frame_player1;
     QLabel* textPlayer1;
     KImageNumbers* IMGNumber1;
     KLed* kLed1;
     QFrame* frame_player2;
     QLabel* textPlayer2;
     KImageNumbers* IMGNumber2;
     KLed* kLed2;
};

#endif // SCOREDISPLAY_H

