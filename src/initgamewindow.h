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

#ifndef INITGAMEWINDOW_H
#define INITGAMEWINDOW_H

#include "initgamewindowui.h"
#include "configuration.h"

#include <klocale.h>
#include <qlineedit.h>
#include <qbutton.h>

class initGameWindow : public initGameWindowUI
{
  Q_OBJECT
public:
   initGameWindow(QWidget* parent = 0, const char* name = 0, GameOptions *g_options = 0 );
  ~initGameWindow();
  /*$PUBLIC_FUNCTIONS$*/

public slots:
   /** No descriptions */
   void slot_ok();
   /** No descriptions */
   void TwoPlayer_clicked();
   /** No descriptions */
   void AiPlayer_clicked();
protected:
  /*$PROTECTED_FUNCTIONS$*/

protected slots:
  /*$PROTECTED_SLOTS$*/
private:
   GameOptions * game_options;
};

#endif

