/***************************************************************************
                          psettings.cpp  -  description
                             -------------------
    begin                : Mon Feb 4 2002
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

#include "psettings.h"
#include <qstring.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <iostream>

PSettings::PSettings(QWidget *parent, const char *name,Configuration *conf )
                                           : confdialog(parent,name,true,0) {
 configuration=conf;

 connect((QObject *)ok_button, SIGNAL( clicked() ),this,SLOT(slot_ok()));
 connect((QObject *)update_button, SIGNAL( clicked() ),this,SLOT(slot_update()));
 connect((QObject *)close_button, SIGNAL( clicked() ),this,SLOT(slot_close()));

/*
 *  Configuration structure:
 *      {
 *       double decel;
 *       double hitballdecel;
 *       double hitedgedecel;
 *       int check_speed;
 *       int angle_factor;
 *      }
 */

 deceleration->setText( QString::number(configuration->decel,'f',6) );
 bouncing_drag_ball->setText( QString::number(configuration->hitballdecel,'f',6) );
 bouncing_drag_edge->setText( QString::number(configuration->hitedgedecel,'f',6) );
 angle_factor->setValue(configuration->angle_factor);
 speed_slider->setValue(configuration->check_speed);

 // now set correct width of labels from font metrics in use
 QFontMetrics fm( TextLabel1->font () ); // each label have same font
 int MaxStringWidth ;

 MaxStringWidth = fm.width(TextLabel1->text());
 if ( fm.width(TextLabel2->text()) > MaxStringWidth ) MaxStringWidth=fm.width(TextLabel2->text());
 if ( fm.width(TextLabel3->text()) > MaxStringWidth ) MaxStringWidth=fm.width(TextLabel3->text());
 if ( fm.width(TextLabel4->text()) > MaxStringWidth ) MaxStringWidth=fm.width(TextLabel4->text());
 if ( fm.width(TextLabel5->text()) > MaxStringWidth ) MaxStringWidth=fm.width(TextLabel5->text());

 if ( MaxStringWidth > TextLabel1->width() ) {
    int offset= MaxStringWidth - TextLabel1->width();
    QRect rect;
    // setting positions of all object on the window
    rect=geometry();
    rect.addCoords( 0, 0, offset, 0 ) ;
    setGeometry(rect);

    rect=TextLabel1->geometry();
    rect.addCoords( 0, 0, offset, 0 ) ;
    TextLabel1->setGeometry(rect);

    rect=TextLabel2->geometry();
    rect.addCoords( 0, 0, offset, 0 ) ;
    TextLabel2->setGeometry(rect);

    rect=TextLabel3->geometry();
    rect.addCoords( 0, 0, offset, 0 ) ;
    TextLabel3->setGeometry(rect);

    rect=TextLabel4->geometry();
    rect.addCoords( 0, 0, offset, 0 ) ;
    TextLabel4->setGeometry(rect);

    rect=TextLabel5->geometry();
    rect.addCoords( 0, 0, offset, 0 ) ;
    TextLabel5->setGeometry(rect);

    rect=deceleration->geometry();
    rect.addCoords( offset, 0, offset, 0 ) ;
    deceleration->setGeometry(rect);

    rect=bouncing_drag_ball->geometry();
    rect.addCoords( offset, 0, offset, 0 ) ;
    bouncing_drag_ball->setGeometry(rect);

    rect=bouncing_drag_edge->geometry();
    rect.addCoords( offset, 0, offset, 0 ) ;
    bouncing_drag_edge->setGeometry(rect);

    rect=angle_factor->geometry();
    rect.addCoords( offset, 0, offset, 0 ) ;
    angle_factor->setGeometry(rect);

    rect=speed_slider->geometry();
    rect.addCoords( offset, 0, offset, 0 ) ;
    speed_slider->setGeometry(rect);

    rect=ok_button->geometry();
    rect.addCoords( offset/2, 0, offset/2, 0 ) ;
    ok_button->setGeometry(rect);

    rect=update_button->geometry();
    rect.addCoords( offset/2, 0, offset/2, 0 ) ;
    update_button->setGeometry(rect);

    rect=close_button->geometry();
    rect.addCoords( offset/2, 0, offset/2, 0 ) ;
    close_button->setGeometry(rect);
 }


}
PSettings::~PSettings(){
}
/** No descriptions */
void PSettings::slot_close(){
 done(-1);
}

/** No descriptions */
void PSettings::slot_ok(){

 if ( checkvalues() )
    {
     configuration->decel=decel;
     configuration->hitballdecel=hitba;
     configuration->hitedgedecel=hited;

     configuration->check_speed =check;
     configuration->angle_factor=angle;

     done(1);
    }
}

/** No descriptions */
void PSettings::slot_update(){

 checkvalues();
}

/** No descriptions */
bool PSettings::checkvalues(){

 bool correct=true;
// double decel,hitba,hited;
// int  check,angle;

 decel=deceleration->text().toDouble();
 hitba=bouncing_drag_ball->text().toDouble();
 hited=bouncing_drag_edge->text().toDouble();

 check=speed_slider->value();
 angle=angle_factor->value();

 double max_decel,max_check;
 max_decel=((double)check/3)/10; //max deceleration
 max_check=((double)check/3)/11 ; // bumper drag should be < deceleration!

/*
 cout<<"angle: "<<angle<<endl;
 cout<<"check: "<<check<<endl;
 cout<<"hited: "<<hited<<endl;
 cout<<"hitba: "<<hitba<<endl;
 cout<<"deceleration: "<<decel<<endl;
 cout<<"max_decel: "<<max_decel<<endl;
 cout<<"max_check: "<<max_check<<endl;
*/

 //checking ..
 if ( decel < 0 )
    {
     KMessageBox::sorry(this,i18n("<qt>Deceleration is negative! </qt>"));
     correct=false;
    }

 if ( hitba < 0 )
    {
     KMessageBox::sorry(this,i18n("<qt>Bouncing drag ball is negative! </qt>"));
     correct=false;
    }

 if ( hited < 0 )
    {
     KMessageBox::sorry(this,i18n("<qt>Bouncing drag edge is negative! </qt>"));
     correct=false;
    }

 if (( check < 5 ) || ( check > 60 ))
    {
     KMessageBox::sorry(this,i18n("<qt>Checking delay is out of range! </qt>"));
     correct=false;
    }

 if (( angle < 6 ) || ( angle > 200 ))
    {
     KMessageBox::sorry(this,i18n("<qt>Angle factor is out of range! </qt>"));
     correct=false;
    }

 return correct;
}

#include "psettings.moc"

