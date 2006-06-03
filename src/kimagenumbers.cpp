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
#include "kimagenumbers.h"
#include <iostream>

KImageNumbers::KImageNumbers(const QString& imgnumbers, QWidget* parent,const char* name) :
  QFrame(parent,name),
  m_value(0)
{
  img = new QPixmap(imgnumbers);
  display(0);
}

KImageNumbers::~KImageNumbers()
{
  delete img;
}

void KImageNumbers::display(int score)
{
  int each = img->width()/11;
  int wl;
  QString data;

  m_value=score;
  if ( m_value < -99 )
      data="---";
  else if (( m_value >= -99 ) && ( m_value < 0 ))
      data.sprintf("-%.2d", (m_value*-1));
  else
      data.sprintf("%.3d", m_value);
  
  // img[-0123456789]
  for(int i=0; i < 3; i++) {
      if(data.at(i) == '-') wl = 0;
      else wl = data.at(i).digitValue() + 1 ;
      bitBlt(this, i*each, 0, img, wl*each, 0, each, img->height());
  }

}

void KImageNumbers::paintEvent(QPaintEvent*)
{
  display(m_value);
}

#include "kimagenumbers.moc"
