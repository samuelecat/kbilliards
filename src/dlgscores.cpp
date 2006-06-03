/***************************************************************************
                          dlgscores.cpp  -  description
                             -------------------
    begin                : Fri Feb 8 2002
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

#include "dlgscores.h"


DlgScores::DlgScores(QWidget *parent, const char *name,int scorep,QString *pname,KConfigBase * conf) :
                                                           DlgScoresUI(parent,name,true,0) {

 config=conf;
 score=scorep;

 connect((QObject *)ok_button, SIGNAL( clicked() ),this,SLOT(close()));
 connect((QObject *)reset_button, SIGNAL( clicked() ),this,SLOT(reset_scores()));

 if ( scorep == 0 ) filltable();
 else {
      playername=*pname;
      init();
      }
}
DlgScores::~DlgScores(){
}
/** No descriptions */
void DlgScores::init(){

 int min=0,sco=0,sco_2;
 QString key,Nam,name,name_2;

 config->setGroup("Score Table");
 min=filltable();
 /*
 if ( score >= min )
    {
     PlayerName pln(this,"player_name",&playername);
     pln.exec();
    }
 else close();
 */
 if ( score < min ) close();

 bool replace=false;

 for(int j=1;j <= 10;j++)
    {
     key.sprintf("score_%d",j);
     Nam.sprintf("Name_%d",j);
     if ( replace )
        {
         sco_2=config->readNumEntry( key, 0 );
         name_2=config->readEntry( Nam, "" );

         config->writeEntry(key,sco);
         config->writeEntry(Nam,name);
         sco=sco_2;
         name=name_2;
        }
     else if ( score >= config->readNumEntry( key, 0 ))
              {
               //substitute this position
               sco=config->readNumEntry( key, 0 );
               name=config->readEntry( Nam, "" );
               replace=true;
               config->writeEntry(key,score);
               config->writeEntry(Nam,playername);
              }
    }

 filltable(); //update view
}
/** No descriptions */
int DlgScores::filltable(){
 int i=0,min=0;

 config->setGroup("Score Table");

 i=config->readNumEntry( "score_1", 0 );
 if ( i == 0 ) return min;
 Position_1->setText( " 1" );
 Name_1->setText( config->readEntry( "Name_1", "" ) );
 score_1->setText( QString::number(i) );
 if ( i < min ) min=i;

 i=config->readNumEntry( "score_2", 0 );
 if ( i == 0 ) return min;
 Position_2->setText( " 2" );
 Name_2->setText( config->readEntry( "Name_2", "" ) );
 score_2->setText( QString::number(i) );
 if ( i < min ) min=i;

 i=config->readNumEntry( "score_3", 0 );
 if ( i == 0 ) return min;
 Position_3->setText( " 3" );
 Name_3->setText( config->readEntry( "Name_3", "" ) );
 score_3->setText( QString::number(i) );
 if ( i < min ) min=i;

 i=config->readNumEntry( "score_4", 0 );
 if ( i == 0 ) return min;
 Position_4->setText( " 4" );
 Name_4->setText( config->readEntry( "Name_4", "" ) );
 score_4->setText( QString::number(i) );
 if ( i < min ) min=i;

 i=config->readNumEntry( "score_5", 0 );
 if ( i == 0 ) return min;
 Position_5->setText( " 5" );
 Name_5->setText( config->readEntry( "Name_5", "" ) );
 score_5->setText( QString::number(i) );
 if ( i < min ) min=i;

 i=config->readNumEntry( "score_6", 0 );
 if ( i == 0 ) return min;
 Position_6->setText( " 6" );
 Name_6->setText( config->readEntry( "Name_6", "" ) );
 score_6->setText( QString::number(i) );
 if ( i < min ) min=i;

 i=config->readNumEntry( "score_7", 0 );
 if ( i == 0 ) return min;
 Position_7->setText( " 7" );
 Name_7->setText( config->readEntry( "Name_7", "" ) );
 score_7->setText( QString::number(i) );
 if ( i < min ) min=i;

 i=config->readNumEntry( "score_8", 0 );
 if ( i == 0 ) return min;
 Position_8->setText( " 8" );
 Name_8->setText( config->readEntry( "Name_8", "" ) );
 score_8->setText( QString::number(i) );
 if ( i < min ) min=i;

 i=config->readNumEntry( "score_9", 0 );
 if ( i == 0 ) return min;
 Position_9->setText( " 9" );
 Name_9->setText( config->readEntry( "Name_9", "" ) );
 score_9->setText( QString::number(i) );
 if ( i < min ) min=i;

 i=config->readNumEntry( "score_10", 0 );
 if ( i == 0 ) return min;
 Position_10->setText( "10" );
 Name_10->setText( config->readEntry( "Name_10", "" ) );
 score_10->setText( QString::number(i) );
 if ( i < min ) min=i;

 return min;
}


/*!
    \fn DlgScores::reset_scores()
 */
void DlgScores::reset_scores()
{
    QString Key,Nam;

    config->setGroup("Score Table");

    for(int i=1;i <= 10;i++)
         {
           Key.sprintf("score_%d",i);
           Nam.sprintf("Name_%d",i);
           config->deleteEntry( Key );
           config->deleteEntry( Nam );
    }

    config->deleteGroup("Score Table");

    Name_1->clear();
    score_1->clear();
    Position_1->clear();
    Name_2->clear();
    score_2->clear();
    Position_2->clear();
    Name_3->clear();
    score_3->clear();
    Position_3->clear();
    Name_4->clear();
    score_4->clear();
    Position_4->clear();
    Name_5->clear();
    score_5->clear();
    Position_5->clear();
    Name_6->clear();
    score_6->clear();
    Position_6->clear();
    Name_7->clear();
    score_7->clear();
    Position_7->clear();
    Name_8->clear();
    score_8->clear();
    Position_8->clear();
    Name_9->clear();
    score_9->clear();
    Position_9->clear();
    Name_10->clear();
    score_10->clear();
    Position_10->clear();

}

#include "dlgscores.moc"
