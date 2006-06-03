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


#include "initgamewindow.h"
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qlabel.h>
#include <qcheckbox.h>

initGameWindow::initGameWindow(QWidget* parent, const char* name, GameOptions *g_options )
        : initGameWindowUI(parent,name,true,0) {
    game_options = g_options;

    labelPlayer1->setText( i18n("Player One") );
    labelPlayer2->setText( i18n("Player Two") );

    namePlayer1->setText( g_options->name_player1 );
    namePlayer2->setText( g_options->name_player2 );

    if ( GameTypeGroup->find( g_options->game ) != 0 ) {
        ((QRadioButton *)GameTypeGroup->find( g_options->game ))->setChecked(true);
    }

    if ( game_options->TwoPlayers ) {
        labelPlayer2->setEnabled(true);
        namePlayer2->setEnabled(true);
        ((QCheckBox *)TwoPcheckBox)->setChecked(true);
    } else {
        labelPlayer2->setDisabled(true);
        namePlayer2->setDisabled(true);
        ((QCheckBox *)TwoPcheckBox)->setChecked(false);
    }

    if ( game_options->AiPlayerTwo ) {
        labelPlayer2->setDisabled(true);
        namePlayer2->setText("Computer");
        namePlayer2->setDisabled(true);
        ((QCheckBox *)TwoPcheckBox)->setChecked(true);
        ((QCheckBox *)TwoPcheckBox)->setDisabled(true);
        ((QCheckBox *)AiPcheckBox)->setChecked(true);
    } else {
        ((QCheckBox *)AiPcheckBox)->setChecked(false);
    }

    connect((QObject *)ok_button, SIGNAL( clicked() ),this,SLOT(slot_ok()));
    connect((QObject *)TwoPcheckBox, SIGNAL( clicked() ),this,SLOT(TwoPlayer_clicked()));
    connect((QObject *)AiPcheckBox, SIGNAL( clicked() ),this,SLOT(AiPlayer_clicked()));
}

initGameWindow::~initGameWindow() {}

/** No descriptions */
void initGameWindow::slot_ok() {

    game_options->name_player1 = namePlayer1->text();
    game_options->name_player2 = namePlayer2->text();

    game_options->game = GameTypeGroup->selectedId();

    done(1);
}

/** No descriptions */
void initGameWindow::TwoPlayer_clicked() {
    if ( ((QCheckBox *)TwoPcheckBox)->isChecked() ) {
        labelPlayer2->setEnabled(true);
        namePlayer2->setEnabled(true);
        game_options->TwoPlayers = true;
    } else {
        labelPlayer2->setDisabled(true);
        namePlayer2->setDisabled(true);
        game_options->TwoPlayers = false;
        game_options->AiPlayerTwo = false;
    }
}

/** No descriptions */
void initGameWindow::AiPlayer_clicked() {
    if ( ((QCheckBox *)AiPcheckBox)->isChecked() ) {
        labelPlayer2->setDisabled(true);
        namePlayer2->setText("Computer");
        namePlayer2->setDisabled(true);
        ((QCheckBox *)TwoPcheckBox)->setChecked(true);
        ((QCheckBox *)TwoPcheckBox)->setDisabled(true);
        game_options->AiPlayerTwo = true;
    } else {
        labelPlayer2->setEnabled(true);
        namePlayer2->setEnabled(true);
        namePlayer2->setText(game_options->name_player2);
        ((QCheckBox *)TwoPcheckBox)->setEnabled(true);
        game_options->AiPlayerTwo = false;
    }
}

#include "initgamewindow.moc"

