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
#include "scoredisplay.h"

#include <kdialog.h>
#include <klocale.h>
#include <qvariant.h>
#include <qframe.h>
#include <qlabel.h>
//#include <qlcdnumber.h>
#include <kled.h>
#include <qlayout.h>
#include <limits.h>
#include "kimagenumbers.h"

ScoreDisplay::ScoreDisplay( QString path_data, QWidget* parent, const char* name, WFlags fl )
     : QWidget( parent, name, fl )
{
     TwoPlayersMode = FALSE ;
     score_player_1 = 0;
     score_player_2 = 0;
     currentPlayerNum = 1;
     repeat_light = 0;
     frameBorder_color_state1 = 3;
     frameBorder_color_state1 = 3;

     if ( !name )
          setName( "ScoreDisplay" );
     setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, sizePolicy().hasHeightForWidth() ) );
     setMinimumSize( QSize( 395, 30 ) );

     frame_player1 = new QFrame( this, "frame_player1" );
     frame_player1->setGeometry( QRect( 0, 0, 190, 30 ) );
     frame_player1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, frame_player1->sizePolicy().hasHeightForWidth() ) );
     frame_player1->setPaletteForegroundColor( QColor( 190, 190, 190 ) );
     frame_player1->setFrameShape( QFrame::StyledPanel );
     frame_player1->setFrameShadow( QFrame::Plain );
     frame_player1->setLineWidth( 1 );
     frame_player1->setMargin( 0 );

     textPlayer1 = new QLabel( frame_player1, "textPlayer1" );
     textPlayer1->setGeometry( QRect( 25, 4, 100, 21 ) );
     textPlayer1->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
     
     IMGNumber1 = new KImageNumbers(path_data + "numbers.png", frame_player1);
     IMGNumber1->setGeometry( QRect( 129, 1, 60, 28 ) );
     IMGNumber1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, IMGNumber1->sizePolicy().hasHeightForWidth() ) );

     kLed1 = new KLed( frame_player1, "kLed1" );
     kLed1->setGeometry( QRect( 5, 8, 16, 16 ) );
     kLed1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 12, 12, kLed1->sizePolicy().hasHeightForWidth() ) );
     kLed1->setState( KLed::On );
     kLed1->setShape( KLed::Circular );
     kLed1->setLook( KLed::Sunken );
     kLed1->setColor( QColor( 255, 0, 0 ) );
     kLed1->setDarkFactor( 300 );

     frame_player2 = new QFrame( this, "frame_player2" );
     frame_player2->setEnabled( FALSE );
     frame_player2->setGeometry( QRect( 205, 0, 190, 30 ) );
     frame_player2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, frame_player2->sizePolicy().hasHeightForWidth() ) );
     frame_player2->setPaletteForegroundColor( QColor( 128, 128, 128 ) );
     frame_player2->setFrameShape( QFrame::StyledPanel );
     frame_player2->setFrameShadow( QFrame::Plain );
     frame_player2->setLineWidth( 1 );

     textPlayer2 = new QLabel( frame_player2, "textPlayer2" );
     textPlayer2->setGeometry( QRect( 25, 4, 100, 21 ) );
     textPlayer2->setPaletteForegroundColor( QColor( 0, 0, 0 ) );

     IMGNumber2 = new KImageNumbers(path_data + "numbers.png", frame_player2);
     IMGNumber2->setGeometry( QRect( 129, 1, 60, 28 ) );
     IMGNumber2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, IMGNumber2->sizePolicy().hasHeightForWidth() ) );

     kLed2 = new KLed( frame_player2, "kLed2" );
     kLed2->setGeometry( QRect( 5, 8, 16, 16 ) );
     kLed2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 12, 12, kLed2->sizePolicy().hasHeightForWidth() ) );
     kLed2->setState( KLed::Off );
     kLed2->setLook( KLed::Sunken );
     kLed2->setColor( QColor( 237, 237, 237 ) );
     kLed2->setDarkFactor( 300 );

     textPlayer1->setText( i18n( "Player one" ) );
     textPlayer2->setText( i18n( "Player two" ) );

     resize( QSize(478, 30).expandedTo(minimumSizeHint()) );
     clearWState( WState_Polished );

     IMGNumber1->display(0);
     IMGNumber2->display(0);

     startTimer( 250 ); //diode lights
}

ScoreDisplay::~ScoreDisplay()
{

}

/** No descriptions */
void ScoreDisplay::enableTwoPlayersMode(bool etwop) {
     TwoPlayersMode = etwop;
     if ( TwoPlayersMode ) {
          frame_player2->setEnabled( TRUE );
          kLed2->setColor( QColor( 0, 0, 255 ) );

          IMGNumber1->display(0);
          IMGNumber2->display(0);
     } else {
          frame_player2->setEnabled( FALSE );
          kLed2->setColor( QColor( 237, 237, 237 ) );
     }

     kLed1->setState( KLed::Off );
     kLed2->setState( KLed::Off );
}

/** No descriptions */
void ScoreDisplay::setCurrentPlayer( int player_num )
{
     currentPlayerNum = player_num;

     if ( currentPlayerNum == 1 ) {
          kLed1->setState( KLed::On );
          kLed2->setState( KLed::Off );
          frameBorder_color_state1=0;
          frameBorder_color_state2=3; //can't change border color
     } else {
          kLed1->setState( KLed::Off );
          kLed2->setState( KLed::On );
          frameBorder_color_state1=3;
          frameBorder_color_state2=0;
     }

     //set orginal border color
     frame_player1->setPaletteForegroundColor( QColor( 180, 180, 180 ) );
     frame_player2->setPaletteForegroundColor( QColor( 180, 180, 180 ) );
     frameBorder_color_state1 = 0;
     frameBorder_color_state2 = 0;

     repeat_light = 0;
}

/** No descriptions */
void ScoreDisplay::setScorePlayerOne(int score)
{
     IMGNumber1->display(score);
     score_player_1 = score;
}

/** No descriptions */
void ScoreDisplay::setScorePlayerTwo(int score)
{
     IMGNumber2->display(score);
     score_player_2 = score;
}

/** No descriptions */
void ScoreDisplay::addScorePlayerNum(int player_num, int addscore)
{
     if ( player_num == 1 ) {
          score_player_1 = score_player_1 + addscore ;
          IMGNumber1->display(score_player_1);
     }

     if ( player_num == 2 ) {
          score_player_2 = score_player_2 + addscore ;
          IMGNumber2->display(score_player_2);
     }

     if ( TwoPlayersMode )
          repeat_light = 4;
}

/** No descriptions */
QString ScoreDisplay::getPlayerName(int player_num)
{
     if ( player_num == 1 ) {
          return textPlayer1->text();
     } else {
          return textPlayer2->text();
     }
}

/** No descriptions */
int ScoreDisplay::getPlayerScore(int player_num)
{
     if ( player_num == 1 ) {
          return score_player_1 ;
     } else {
          return score_player_2 ;
     }
}

/** No descriptions */
void ScoreDisplay::setPlayerName(int player_num, QString name)
{
     if ( player_num == 1 ) {
          textPlayer1->setText(name);
     } else {
          textPlayer2->setText(name);
     }
}

/** No descriptions */
void ScoreDisplay::timerEvent( QTimerEvent *)
{
     if ( repeat_light <= 4 ) {
          if ( currentPlayerNum == 1 ) {
               //swich light
               kLed1->toggle();
               if ( frameBorder_color_state1 == 0 ) {
                    frame_player1->setPaletteForegroundColor( QColor( 180, 180, 180 ) ); //grey
                    frameBorder_color_state1 = 1;
               } else if ( frameBorder_color_state1 == 1 ) {
                    frame_player1->setPaletteForegroundColor( QColor( 0, 0, 0 ) ); //black
                    frameBorder_color_state1 = 0;
               }
          } else {
               kLed2->toggle();
               if ( frameBorder_color_state2 == 0 ) {
                    frame_player2->setPaletteForegroundColor( QColor( 180, 180, 180 ) ); //grey
                    frameBorder_color_state2 = 1;
               } else if ( frameBorder_color_state2 == 1 ) {
                    frame_player2->setPaletteForegroundColor( QColor( 0, 0, 0 ) ); //black
                    frameBorder_color_state2 = 0;
               }
          }
          repeat_light++;
     } else {
          if ( currentPlayerNum == 1 ) {
               kLed1->setState( KLed::On );
               kLed2->setState( KLed::Off );
               frameBorder_color_state1=3;
               frame_player1->setPaletteForegroundColor( QColor( 180, 180, 180 ) ); //grey
          } else {
               kLed1->setState( KLed::Off );
               kLed2->setState( KLed::On );
               frameBorder_color_state2=3;
               frame_player2->setPaletteForegroundColor( QColor( 180, 180, 180 ) ); //grey
          }
     }

}


#include "scoredisplay.moc"
