/***************************************************************************
                               kbilliards.cpp
                             -------------------
    begin                : gio gen 24 13:37:53 CET 2002
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

#include "kbilliards.h"
#include <kmenubar.h>
#include <kstatusbar.h>
#include <kstddirs.h>
#include <kaboutapplication.h>
#include <kaboutkde.h>
#include <kiconloader.h>
#include <klocale.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qvaluelist.h>
#include <math.h>
#include <ktoolbarbutton.h>
#include <kmessagebox.h>
#include <kpopupmenu.h>
#include <kaction.h>
#include <kaccel.h>
#include <kstdaction.h>
#include <kstdaccel.h>
#include <unistd.h>
#include <bitset>
#include <stdlib.h>
#include <string>
#include <qfile.h>
#include <qimage.h>
#include <qcstring.h>
#include <qdom.h>
#include <qbuffer.h>
#include <bzlib.h>

#include <iostream>

#include "canvasview.h"
#include "initgamewindow.h"

#define BUFSIZE 65536

using namespace std;

struct edge_ball {
    BounceSegmentOb * edge;
    Ball * ball;
    double hit_value;
} ;

struct ball_ball {
    Ball * ball1;
    Ball * ball2;
    double hit_value;
} ;

//help message function
void msg_file_not_found_in_install(QWidget *object,QString file_name) {
    QString msg;
    msg=I18N_NOOP("<qt>File: _FILENAME_ not found or installation incomplete</qt>");
    msg.replace("_FILENAME_",file_name,TRUE); //this replace _FILENAME_ with the real file name
    KMessageBox::sorry(object,msg);
}

// hextext to bin converter
uchar * BinFromTextData(QString &);

Kbilliards::Kbilliards(QWidget *parent, const char *name) : KMainWindow(parent, name) {
    status=statusBar();
    menu=menuBar();
    gamestatus=0; //game not running
    currentPlayerNum=1;
    previousPlayerNum=1;

    status->insertItem(i18n(" Ready."), 1 );

    config=kapp->config();
    /*
     KIconLoader *ld = KGlobal::iconLoader();
     kapp->miniIcon() = ld->loadIcon("kbilliards",KIcon::Small);
     kapp->Icon() = ld->loadIcon("kbilliards",KIcon::Small);
    */
    /*
     *  from: "configuration.h"
     *
     *  Configuration structure:
     *      {
     *       double decel;
     *       double hitballdecel;
     *       double hitedgedecel;
     *       int check_speed;
     *       int angle_factor;
     *      }
     */

    defaultconfig.decel=0.035;
    defaultconfig.hitballdecel=0.020;
    defaultconfig.hitedgedecel=0.060;
    defaultconfig.check_speed=30;
    defaultconfig.angle_factor=180;

    // load configuration in actualconfig if exist ,else actualconfig =  defaultconfig
    // and load music configuration
    loadSettings();
    // load game optios like players name , multiplayer, game type ..
    loadGameOptions();

    MaxThrowPower=round((actualconfig.check_speed)*0.36) ;

    //MaxThrowPower=40; //DEBUG, do not use it!

    KStandardDirs sd;
    path_data=(QString)(sd.findResourceDir("data","kbilliards/leftarrow.png") + (QString)"kbilliards/");

    QString p_sound=path_data + (QString)"sound/";
    Music= new SoundModule(p_sound,this );

    /* ?? */
    kapp->miniIcon() = QPixmap(sd.findResource("icon","locolor/16x16/apps/kbilliards.png"));
    kapp->icon() = QPixmap(sd.findResource("icon","locolor/32x32/apps/kbilliards.png"));
    /* ?? */

    disconnect(kapp, SIGNAL(destroyed()),0,0);
    disconnect(kapp, SIGNAL(shutDown()),0,0);

    /****** OBJECTS ******/
    canvas= new MCanvas(this,743,397);
    canvas->setAdvancePeriod(actualconfig.check_speed);
    canvas->setBackgroundPixmap(QPixmap( sd.findResource("data","kbilliards/background.png") ));

    AIp=new AIplayer(this,canvas);

    BilliardArea= new CanvasView(canvas,this);
    setCentralWidget(BilliardArea);
    BilliardArea->resize(748,400);
    BilliardArea->setMaximumSize(748,400);
    BilliardArea->setVScrollBarMode(QScrollView::AlwaysOff);
    BilliardArea->setHScrollBarMode(QScrollView::AlwaysOff);

    /****** MENU ******/
    KPopupMenu* file = new KPopupMenu;
    KAction *newAct = new KAction(i18n("&New Game"), QIconSet(BarIcon("filenew")),
                                  KStdAccel::shortcut(KStdAccel::New), this,
                                  SLOT(newGame()), actionCollection());
    newAct->plug( file );

    file->insertItem(i18n("Sco&res"), this, SLOT(viewScores()), CTRL+Key_R);

    file->insertSeparator();

    newAct=KStdAction::quit(this, SLOT(close()), actionCollection());
    newAct->plug( file );

    menu->insertItem(i18n("&File"), file);

    //----------------------

    KPopupMenu* settings = new KPopupMenu;
    newAct = new KAction(i18n("&Preferences"), QIconSet(BarIcon("configure")),
                         KShortcut(CTRL+Key_P), this,
                         SLOT(confdialog()), actionCollection());
    newAct->plug( settings );

    KToggleAction *act=new  KToggleAction(i18n("&Music"),CTRL+Key_M, Music,"music object");
    act->setChecked(actualmusicconfig.enablemusic) ;
    Music->enableMusic(actualmusicconfig.enablemusic) ;
    connect( act,SIGNAL(toggled(bool)),Music, SLOT(enableMusic(bool)) );

    act->plug( settings );

    act=new  KToggleAction(i18n("&Effects"),CTRL+Key_E, Music,"music object");
    act->setChecked(actualmusicconfig.enableeffects) ;
    Music->enableEffects(actualmusicconfig.enableeffects) ;
    connect( act,SIGNAL(toggled(bool)),Music, SLOT(enableEffects(bool)) );

    act->plug( settings );

    settings->insertItem(i18n("&Save Preferences"), this, SLOT(saveSettings()), CTRL+Key_S);

    settings->insertSeparator();

    act=new  KToggleAction(i18n("AI: show hidden trajectory"), KShortcut::null(), AIp );
    connect( act,SIGNAL(toggled(bool)), AIp, SLOT(slotShowTrajectory(bool)) );

    act->plug( settings );

    menu->insertItem(i18n("Settings"), settings);

    //----------------------

    KPopupMenu* help = new KPopupMenu;
    newAct = KStdAction::aboutApp(this, SLOT(slotAbout()), actionCollection());
    newAct->plug(help);
    newAct = KStdAction::aboutKDE(this, SLOT(slotAboutKDE()), actionCollection());
    newAct->plug(help);


    menu->insertItem(i18n("&Help"), help);

    /****** TOOLBAR ******/
    toolbar = toolBar("Command Bar");
    addToolBar( toolbar,"Command Bar", Top, FALSE );
    KToolBarButton *p;

    p=new KToolBarButton( QPixmap( sd.findResource("data","kbilliards/ball.png" )),1,toolbar, "fire");
    connect(p,SIGNAL(clicked(int)),this,SLOT(throw_ball()));

    p=new KToolBarButton( QPixmap( sd.findResource("data","kbilliards/leftarrow.png"  )),2,toolbar, "Move Ball Left" );
    connect(p,SIGNAL(clicked(int)),this, SLOT(move_left_vector()));

    p=new KToolBarButton( QPixmap( sd.findResource("data","kbilliards/rightarrow.png" )),3,toolbar, "Move Ball Right" );
    connect(p,SIGNAL(clicked(int)),this, SLOT(move_right_vector()));

    powerbar=new PowerBar(toolbar,"Power Bar");
    powerbar->setMaximumSize( 77, 30 );
    powerbar->Stop();

    scoredisplay = new ScoreDisplay(path_data,toolbar,"Display Score");

    /****** INITIALIZATION *****/
    setMinimumSize(748,478);
    placeObjects();

    placeBalls(300,184);
    gamestatus=0;
    throwvector->hide();

    startTimer( 200 );
}

Kbilliards::~Kbilliards() {}
/** No descriptions */
void Kbilliards::slotAbout() {
    KAboutApplication *a = new KAboutApplication(this);
    a->show();
}
/** No descriptions */
void Kbilliards::slotAboutKDE() {
    KAboutKDE *akde = new KAboutKDE(this);
    akde->show();
}
/** No descriptions */
void Kbilliards::enableTwoPlayersMode(bool etwop) {
    actualgameoptions.TwoPlayers = etwop;
    scoredisplay->enableTwoPlayersMode(etwop);
}
/** No descriptions */
void Kbilliards::placeObjects() {

    //======== map  =========================================================================

    if ( QFile::exists(path_data + (QString)"maps/kbilliards2004.kbm") ) {
        if ( loadMap(path_data + (QString)"maps/kbilliards2004.kbm") == 1 ) {
            KMessageBox::sorry(this,i18n("Error loading table map"));
            exit(0);
        }
    } else {
        msg_file_not_found_in_install(this,"kbilliards2004.kbm");
        exit(0);
    }

    //======== balls =========================================================================

    QCanvasPixmapArray *shadow=new QCanvasPixmapArray();

    if ( QFile::exists(path_data + (QString)"balls/ball_shadow.png") ) {
        shadow->setImage( 0, new QCanvasPixmap(path_data + (QString)"balls/ball_shadow.png") );
    } else {
        msg_file_not_found_in_install(this,"ball_shadow.png");
        exit(0);
    }

    if ( QFile::exists(path_data + (QString)"balls/ball_shadowb.png") ) {
        shadow->setImage( 1, new QCanvasPixmap(path_data + (QString)"balls/ball_shadowb.png") );
    } else {
        msg_file_not_found_in_install(this,"ball_shadowb.png");
        exit(0);
    }

    if ( QFile::exists(path_data + (QString)"balls/whiteball.png") ) {
        whiteball= new Ball(new QCanvasPixmapArray(path_data + (QString)"balls/whiteball.png"),shadow,canvas,this,0);
        ingameballs.append(whiteball);
    } else {
        msg_file_not_found_in_install(this,"whiteball.png");
        exit(0);
    }

    throwvector=new ThrowVector(canvas,whiteball);
    throwvector->setResolution(actualconfig.angle_factor);
    throwvector->setdirection(throwvector->getResolution());
    throwvector->setfromCanvasPeriod(actualconfig.check_speed);
    throwvector->show();

    Ball *b;
    QString str, file_name ;

    for(int i=1;i < 16;i++) {
        file_name.sprintf("ball%.2d",i );
        str= path_data + (QString)"balls/"  + file_name ;
        QCanvasPixmapArray *qcpa=new QCanvasPixmapArray();

        if ( QFile::exists(path_data + (QString)"balls/ball_shadow.png") ) {
            shadow->setImage( 0, new QCanvasPixmap(path_data + (QString)"balls/ball_shadow.png") );
        } else {
            msg_file_not_found_in_install(this,"ball_shadow.png");
            exit(0);
        }

        if ( QFile::exists(path_data + (QString)"balls/ball_shadowb.png") ) {
            shadow->setImage( 1, new QCanvasPixmap(path_data + (QString)"balls/ball_shadowb.png") );
        } else {
            msg_file_not_found_in_install(this,"ball_shadowb.png");
            exit(0);
        }

        if ( QFile::exists(str + ".png") ) {
            qcpa->setImage(0,new QCanvasPixmap(str + ".png"));
        } else {
            msg_file_not_found_in_install(this,file_name + ".png");
            exit(0);
        }

        if ( QFile::exists(str + "b.png") ) {
            qcpa->setImage(1,new QCanvasPixmap(str + "b.png"));
        } else {
            msg_file_not_found_in_install(this,file_name + "b.png");
            exit(0);
        }

        b = new Ball(qcpa,shadow,canvas,this,i);

        ingameballs.append(b);
    }

}
/** No descriptions */
void Kbilliards::throw_ball() {

    // warnings: in this function any routines work only
    // with local human player

    if ( throwvector->visible() ) {
        if (  powerbar->stopped ) {
            powerbar->Start();
        } else {
            double xsp=0,ysp=0;
            double speed_f;

            if ( first_fire ) {
                speed_f=(MaxThrowPower*1.2)/10 ;//increase power on first fire
                first_fire=false;
            } else
                speed_f=MaxThrowPower/10;

            //xsp=pow(2,0.36*powerbar->power)*cos(throwvector->getalpha());//exponential power
            //ysp=pow(2,0.36*powerbar->power)*sin(throwvector->getalpha());
            xsp=speed_f*(powerbar->power)*cos(throwvector->getalpha());
            ysp=speed_f*(powerbar->power)*sin(throwvector->getalpha());
            whiteball->setVelocity(xsp,ysp);

            throwvector->hide();
            powerbar->Stop();

            Music->OneShot("shot");
        }
    }

}
/** No descriptions */
void Kbilliards::timerEvent( QTimerEvent *) {

    Ball *it;
    bool allbs=true;  // check if all balls are stopped
    bool whiteingame=false;
    bool aNextPlayer=false;

    if ( gamestatus == 0 )
        return;

    for(it=ingameballs.first(); it != 0; it=ingameballs.next()) {

        if ( it->stopped() == false )
            allbs=false;
        if ( it->ballnumber == 0 )
            whiteingame=true;
    }

    if ( allbs == true ) {

        if ( whiteingame == true ) {
            if ( ! throwvector->visible() && ( powerbar->stopped ) ) {

                aNextPlayer=false;

                //to check score and other things we awaiting that first blow was made
                if ( !first_fire ) {

                    if ( ballinhole == false ) {
                        //in this turn no ball going into any hole!
                        scoredisplay->addScorePlayerNum(currentPlayerNum,-3);
                        aNextPlayer=true;
                    }

                    if ( firstHittedBall == -1 ) {
                        //in this turn no ball was hitted!
                        scoredisplay->addScorePlayerNum(currentPlayerNum,-5);
                        aNextPlayer=true;
                    }

                    if ( aNextPlayer )
                        activateNextPlayer();

                }

                ballinhole = false;
                firstHittedBall = -1; // reset

                powerbar->Stop();

                if ( actualgameoptions.AiPlayerTwo && (currentPlayerNum == 2) ) {
                    throwvector->hide();
                    AIp->stroke();
                    first_fire=false;// AI player can't use extrapower or set first_fire
                } else {
                    throwvector->setdirection(throwvector->angle);
                    throwvector->show();
                }
            }
        } else { // ( whiteingame == false )
            if ( gamestatus !=0 ) {
                Music->stop();
                gamestatus=0;
                if ( actualgameoptions.TwoPlayers ) {
                    if ( actualgameoptions.AiPlayerTwo ) {
                        if (currentPlayerNum == 1)  {
                            status->changeItem(i18n(" Computer win "), 1 );
                            KMessageBox::information(this,i18n(" Computer win "));
                        } else {
                            status->changeItem(i18n(" .. the computer lost the white ball! "), 1 );
                            KMessageBox::information(this, i18n(" .. the computer lost the game! "));
                            winner(1);
                        }
                    } else {
                        status->changeItem(scoredisplay->getPlayerName(currentPlayerNum) + i18n(" .. you have lost the white ball! "), 1 );
                        KMessageBox::information(this,scoredisplay->getPlayerName(currentPlayerNum) + i18n(" .. you have lost the game! "));
                    }
                } else {
                    status->changeItem(scoredisplay->getPlayerName(currentPlayerNum) + i18n(" .. you have lost the white ball! "), 1 );
                    KMessageBox::information(this,scoredisplay->getPlayerName(currentPlayerNum) + i18n(" .. you have lost the game! "));
                }
            }
            gamestatus=0;
        }

    }

    if ((ingameballs.count() == 1) && ( whiteingame == true )) {
        Music->stop();
        gamestatus=0;
        throwvector->hide();

        if ( actualgameoptions.TwoPlayers ) {
            if ( scoredisplay->getPlayerScore(1) > scoredisplay->getPlayerScore(2) ) {
                winner(1);

            } else if ( scoredisplay->getPlayerScore(1) < scoredisplay->getPlayerScore(2) ) {

                if ( actualgameoptions.AiPlayerTwo ) {
                    status->changeItem(i18n(" Computer win "), 1 );
                    KMessageBox::information(this,i18n(" Computer win "));
                } else {
                    status->changeItem(scoredisplay->getPlayerName(2) + i18n("  Congratulations you win!  "), 1 );
                    KMessageBox::information(this,scoredisplay->getPlayerName(2) + i18n("  Congratulations you win!  "));
                    winner(2);
                }

            } else {
                status->changeItem(i18n("  Draw!  "), 1 );
                KMessageBox::information(this,i18n("  Draw!  "));
                winner(1);
            }

        } else {
            status->changeItem(i18n("  Congratulations you win!  "), 1 );
            KMessageBox::information(this,i18n("  Congratulations you win!  "));
            winner(1);
        }

    }
}
/** No descriptions */
void Kbilliards::move_right_vector() {
    if( throwvector->visible())
        throwvector->setdirection(throwvector->angle +1);
}
/** No descriptions */
void Kbilliards::move_left_vector() {
    if( throwvector->visible())
        throwvector->setdirection(throwvector->angle -1);
}
/** No descriptions */
void Kbilliards::keyPressEvent( QKeyEvent *key ) {

    if ( actualgameoptions.AiPlayerTwo && ( currentPlayerNum == 2 ) )
        return;

    switch( key->key() ) {

    case Key_Left:
        move_left_vector();
        break;
    case Key_Right:
        move_right_vector();
        break;
    case Key_Space:
        throw_ball();
        break;

    }

}
/** No descriptions */
void Kbilliards::placeBalls(int xpto,int ypto) {

    QList<Ball> list;
    int count=0,xp,yp;
    Ball *it;

    for(it=ingameballs.first(); it != 0; it=ingameballs.next()) {
        count++;
        list.append(it);
        it->setAnimateL(false);
        it->hide();
    }

    ingameballs.clear();

    for(it=outgameballs.first(); it != 0; it=outgameballs.next()) {
        count++;
        list.append(it);
        it->setAnimateL(false);
        it->hide();
    }

    outgameballs.clear();

    xp=xpto;
    yp=ypto;

    if ( count < 16 )
        cout<<"Error!!! there is only: "<<count<<" Balls in game!"<<endl;

    int i=1;

    for(it=list.first(); (it != 0); it=list.next()) {
        //white ball
        if (it->ballnumber == 0) {
            it->setVelocity(0,0);
            it->setPosition(500,184);
            it->show();
            ingameballs.append(it);
            it->is_draggable = false;
            continue;
        }

        // 11
        //    7
        // 12   4
        //    8   2
        // 13   5   1                    0
        //    9   3
        // 14   6
        //   10
        // 15

        if ( i == 1 ) {
            xp = xpto;
            yp = ypto;
            ingameballs.append(it);
        } else if (( i == 2 )||( i == 3)) {
            xp = xpto  -  (int)Ball::diameter + 1;
            yp = ypto  -  (int)Ball::diameter/2 + (i - 2)*((int)Ball::diameter);
            ingameballs.append(it);
        } else if (( i >= 4 )&&( i <= 6)) {
            xp = xpto - 2*(int)Ball::diameter + 2;
            yp = ypto -   (int)Ball::diameter + (i - 4)*((int)Ball::diameter);
            ingameballs.append(it);
        } else if (( i >= 7 )&&( i <= 10)) {
            xp = xpto - 3*(int)Ball::diameter + 3;
            yp = ypto - 3*(int)Ball::diameter/2 + (i - 7)*((int)Ball::diameter);
            ingameballs.append(it);
        } else {
            xp = xpto - 4*(int)Ball::diameter + 4;
            yp = ypto - 2*(int)Ball::diameter + (i - 11)*((int)Ball::diameter);
            ingameballs.append(it);
        }

        it->setVelocity(0,0);
        it->vect_Vel.null();
        it->setPosition(xp,yp);
        it->is_draggable = false;
        it->setAnimated(true );
        it->show();
        i++;
    }//end for

    throwvector->setdirection(throwvector->getResolution());

}
/** No descriptions */
void Kbilliards::newGame() {

    gamestatus = 0 ;

    //increase power on first fire and is a flag to start checking on score
    first_fire=true;
    AIp->hideTrajectories() ;

    placeBalls(300,184);

    powerbar->Stop();
    ballinhole=false;
    firstHittedBall = -1;

    initgamedialog();

    enableTwoPlayersMode(actualgameoptions.TwoPlayers);

    //first initialize some value
    currentPlayerNum=1;
    previousPlayerNum=1;
    scoredisplay->setCurrentPlayer(1);

    scoredisplay->setScorePlayerOne(0);
    scoredisplay->setScorePlayerTwo(0);

    throwvector->setPen( QPen( QColor(red), 2, DashLine )  );


    status->changeItem(i18n(" Ready. "), 1 );

    Ball *it;
    for(it=ingameballs.first(); it != 0; it=ingameballs.next()) {
        // blink only ball number 1
        if ( it->ballnumber == 1 ) {
            it->setAnimateL(true) ;
        } else
            it->setAnimateL(false) ;
        //reload actual physics parameters
        it->deceleration=actualconfig.decel;
        it->hitballdecel=actualconfig.hitballdecel;
        it->hitedgedecel=actualconfig.hitedgedecel;

        if ( actualgameoptions.game == 2 )
            it->is_draggable = true;
    }

    gamestatus=1;

    Music->setLoop(true);
    Music->loadfile("music01.ogg");
    Music->play();
}
/** No descriptions */
void Kbilliards::BallinHole(int num) {
    if ( num != 0 ) {
        Music->OneShot("ballinhole");
        ballinhole=true;

        if ( outgameballs.count() == 1 )
            nextBallseq=1;

        Ball *it;
        QString mex;

        const char *mexstr=I18N_NOOP(" Ball number _NUMBALL_ out ");
        mex=mexstr ;
        mex.replace("_NUMBALL_",QString::number(num),TRUE);

        int min=16;
        if ( num == nextBallseq )
            scoredisplay->addScorePlayerNum(currentPlayerNum,5);
        scoredisplay->addScorePlayerNum(currentPlayerNum,15);

        for(it=ingameballs.first(); it != 0; it=ingameballs.next()) {
            if ((it->ballnumber != 0 ) && ( it->ballnumber < min ))
                min=it->ballnumber;
        }
        nextBallseq=min;

        for(it=ingameballs.first(); it != 0; it=ingameballs.next()) {
            if ( it->ballnumber == min ) {
                it->setAnimateL(true) ;
                break;
            }
        }

        status->changeItem(mex, 1 );

    } else {
        Music->OneShot("gameover");
    }
}
/** show configuration window dialog */
void Kbilliards::confdialog() {

    Ball *it;
    bool allbs=true;  // check if all balls are stopped
    int ac_gamestatus;
    bool isvisible=false;

    for(it=ingameballs.first(); it != 0; it=ingameballs.next())
        if ( it->stopped() == false ) {
            allbs=false;
            break;
        }

    if ( allbs ) {
        if (throwvector->visible()) {
            isvisible=true;
            throwvector->hide(); //animation stopped
        }

        ac_gamestatus=gamestatus;
        gamestatus=0;

        int result=0;
        Configuration ac_conf;
        ac_conf=actualconfig;
        PSettings conf(this,"config",&ac_conf);
        result=conf.exec(); // == 1  is done!

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

        if ( result == 1 ) {
            //apply settings

            actualconfig=ac_conf;
            canvas->setAdvancePeriod(100); // very slow, I do not want any problem during this phase

            for(it=ingameballs.first(); it != 0; it=ingameballs.next()) {
                it->deceleration=actualconfig.decel;
                it->hitballdecel=actualconfig.hitballdecel;
                it->hitedgedecel=actualconfig.hitedgedecel;
            }
            for(it=outgameballs.first(); it != 0; it=outgameballs.next()) {
                it->deceleration=actualconfig.decel;
                it->hitballdecel=actualconfig.hitballdecel;
                it->hitedgedecel=actualconfig.hitedgedecel;
            }

            throwvector->setResolution(actualconfig.angle_factor);
            throwvector->setfromCanvasPeriod(actualconfig.check_speed); //set correct animation speed

            canvas->setAdvancePeriod(actualconfig.check_speed); // set current animation speed

            MaxThrowPower=round((actualconfig.check_speed)*0.36) ; // set proportional max power
        }

        if (isvisible) {
            throwvector->setdirection(throwvector->getResolution());
            throwvector->show();
        }
        gamestatus=ac_gamestatus;

    }// end  if ( allbs )

}
/** No descriptions */
void Kbilliards::loadSettings() {

    config->setGroup("Physical Configuration");

    //double entry
    actualconfig.decel= config->readDoubleNumEntry("decel",defaultconfig.decel );
    actualconfig.hitballdecel= config->readDoubleNumEntry("hitballdecel",defaultconfig.hitballdecel );
    actualconfig.hitedgedecel= config->readDoubleNumEntry("hitedgedecel",defaultconfig.hitedgedecel );
    //int  entry
    actualconfig.check_speed= config->readNumEntry( "check_speed", defaultconfig.check_speed );
    actualconfig.angle_factor= config->readNumEntry( "angle_factor", defaultconfig.angle_factor );

    config->setGroup("Music Configuration");

    actualmusicconfig.enablemusic=config->readBoolEntry( "enablemusic", true );
    actualmusicconfig.enableeffects=config->readBoolEntry( "enableeffects", true );

}
/** No descriptions */
void Kbilliards::saveSettings() {

    config->setGroup("Physical Configuration");

    //double entry
    config->writeEntry("decel", actualconfig.decel );
    config->writeEntry("hitballdecel", actualconfig.hitballdecel );
    config->writeEntry("hitedgedecel", actualconfig.hitedgedecel );
    //int  entry
    config->writeEntry("check_speed", actualconfig.check_speed );
    config->writeEntry("angle_factor",actualconfig.angle_factor);

    config->setGroup("Music Configuration");

    //bool  entry
    config->writeEntry("enablemusic",Music->getMusicState());
    config->writeEntry("enableeffects",Music->getEffectState());

}
/** No descriptions */
void Kbilliards::loadGameOptions() {

    config->setGroup("Game Options");

    actualgameoptions.name_player1=config->readEntry( "nameplayer1", i18n("Player one") );
    actualgameoptions.name_player2=config->readEntry( "nameplayer2", i18n("Player two") );

    actualgameoptions.game=config->readNumEntry("gametype",1); //default 1

    actualgameoptions.TwoPlayers=config->readBoolEntry( "TwoPlayers", false );

    if ( actualgameoptions.TwoPlayers )
        actualgameoptions.AiPlayerTwo=config->readBoolEntry( "AiPlayerTwo", false );
    else
        actualgameoptions.AiPlayerTwo=false;
}
/** No descriptions */
void Kbilliards::saveGameOptions() {

    config->setGroup("Game Options");

    config->writeEntry("nameplayer1",actualgameoptions.name_player1);
    config->writeEntry("nameplayer2",actualgameoptions.name_player2);

    config->writeEntry("gametype",actualgameoptions.game);

    config->writeEntry("TwoPlayers",actualgameoptions.TwoPlayers);

    if ( actualgameoptions.TwoPlayers )
        config->writeEntry("AiPlayerTwo",actualgameoptions.AiPlayerTwo);
    else
        config->writeEntry("AiPlayerTwo",false);
}
/** plnumber player is the winner */
void Kbilliards::winner(int plnumber) {

    int i=0,min=0;
    QString key;

    if ( actualgameoptions.TwoPlayers ) {
        if ( !( (scoredisplay->getPlayerScore(1) != scoredisplay->getPlayerScore(2)) &&
                (actualgameoptions.AiPlayerTwo && (plnumber == 2)) ) )
            Music->OneShot("applause");
    } else
        Music->OneShot("applause");

    config->setGroup("Score Table");

    for(int j=1;j <= 10;j++) {
        key.sprintf("score_%d",j);

        i=config->readNumEntry( key, 0 );
        if ( i < min )
            min=i;
    }

    if ( scoredisplay->getPlayerScore(plnumber) >= min ) {
        if (( actualgameoptions.TwoPlayers )&&( scoredisplay->getPlayerScore(1) == scoredisplay->getPlayerScore(2))) {
            //Two Players finish Draw, concatene player's name
            QString playeroneandtwo, playertwo;
            playeroneandtwo=scoredisplay->getPlayerName(1);
            if ( playeroneandtwo.length() >= 14 ) {
                playeroneandtwo.truncate(14);
                playeroneandtwo=playeroneandtwo + ".";
            }
            if( actualgameoptions.AiPlayerTwo )
                playertwo="Computer";
            else {
                playertwo=scoredisplay->getPlayerName(2);
                if ( playertwo.length() >= 14 ) {
                    playertwo.truncate(14);
                    playertwo = playertwo + ".";
                }
            }
            playeroneandtwo=playeroneandtwo + " - " + playertwo ;
            DlgScores dlgs(this,"dlg_scores",scoredisplay->getPlayerScore(1),&playeroneandtwo, config);
            dlgs.exec();
        } else {
            QString pname;
            pname=scoredisplay->getPlayerName(plnumber);
            DlgScores dlgs(this,"dlg_scores",scoredisplay->getPlayerScore(plnumber),&pname, config);
            dlgs.exec();
        }
    }

    gamestatus = 0 ;
}
/** No descriptions */
void Kbilliards::viewScores() {
    DlgScores dlgs(this,"dlg_scores",0,0,config);
    dlgs.exec();
}
/** No descriptions */
void Kbilliards::clock() {

    if ( gamestatus == 0 )
        return;

    mutex.lock();

    int infinite_loop=1; //prevent freeze application (infinite do-while loop)
    Ball *i,*j;
    BounceSegmentOb *k ;
    QList<Ball> clone ;
    double step=1,bmin_step=1,emin_step=1; //1 is whitout collision
    double min_step=1,tmp_min_step=1;
    bool flag_b,flag_e;

    clone=ingameballs;

    ball_ball ball_ball_array[32]; //big arrays, max 32 hit at the same time (two for ball,improbable)
    edge_ball edge_ball_array[32]; //max 32 hit at the same time (2 hit for each ball,improbable)
    int ball_ball_index=0;
    int edge_ball_index=0;

    //(to remove duplicate x-y and y-x) if x hit y [y][x]= TRUE , else = FALSE
    bool ball_ball_hitted[16][16] ;
    int bn1,bn2;

    int z1,z2;

    double estep_ret,step_ret;

    do {
        flag_e=false;
        flag_b=false;

        for(i=ingameballs.first(); i != 0; i=ingameballs.next()) {
            //************* EDGES ****************
            if (( i->vect_Vel.x != 0.0 )||( i->vect_Vel.y != 0.0 )) {
                for(k=edgeobjects.first(); k != 0; k=edgeobjects.next()) {
                    estep_ret=k->CPrediction(i);
                    if ( i->visible() && (estep_ret >= 0) && (estep_ret <= emin_step) && (estep_ret < step) ) {
                        flag_e=true; //found a collision with an edge
                        emin_step=estep_ret;

                        edge_ball_array[edge_ball_index].edge = k;
                        edge_ball_array[edge_ball_index].ball = i;
                        edge_ball_array[edge_ball_index].hit_value = estep_ret;
                        edge_ball_index++ ;
                        //cout<<"edge collision edge="<<k->edgenumber<<" ball="<<(i->ballnumber)<<" estep_ret="<<estep_ret<<endl;
                    }
                }
            }
            //************* BALLS ****************
            for(j=clone.first(); j != 0; j=clone.next()) {
                if ( i->visible() && j->visible() &&( (i->ballnumber) != (j->ballnumber) ) ) {
                    step_ret=i->BallCPrediction(j);
                    if ( (step_ret >= 0) && (step_ret <= bmin_step) && (step_ret < step) ) {
                        flag_b=true; //found a collision with a ball
                        bmin_step=step_ret; // minimum step

                        ball_ball_array[ball_ball_index].ball1 = i;
                        ball_ball_array[ball_ball_index].ball2 = j;
                        ball_ball_array[ball_ball_index].hit_value = step_ret;
                        ball_ball_index++ ;
                        //cout<<"ball collision bn1="<<(i->ballnumber)<<" bn2="<<(j->ballnumber)<<" step_ret="<<step_ret<<endl;
                    }
                }
            }
            //************************************
        }

        if ( emin_step < bmin_step )
            tmp_min_step=emin_step;
        else if ( bmin_step <= emin_step )
            tmp_min_step=bmin_step;

        if (( tmp_min_step < step )&&( tmp_min_step >= 0.0 )) {
            min_step=tmp_min_step;
        } else {
            flag_b=FALSE;
            flag_e=FALSE;
        }

        //traslate
        for(i=ingameballs.first(); ( i != 0 )&&(min_step != 0); i=ingameballs.next()) {
            i->vect_Pos = i->vect_Pos + (i->vect_Vel*min_step);
        }

        if ( !flag_b && !flag_e )
            break;
        //else
        //     cout<<"*step="<<step<<" min_step="<<min_step<<" bmin_step="<<bmin_step<<" emin_step="<<emin_step<<endl;

        if (( bmin_step == min_step )&& flag_b) {

            for(z1=0;z1 < 16;z1++)
                for(z2=0;z2 < 16;z2++)
                    ball_ball_hitted[z1][z2]=FALSE;

            for (ball_ball_index--; (ball_ball_index >= 0) &&( ball_ball_array[ball_ball_index].hit_value == min_step) ; ball_ball_index--) {
                bn1=ball_ball_array[ball_ball_index].ball1->ballnumber;
                bn2=ball_ball_array[ball_ball_index].ball2->ballnumber;
                if ( ball_ball_hitted[bn1][bn2] == FALSE ) {
                    ball_ball_array[ball_ball_index].ball1->DoBallCollision(ball_ball_array[ball_ball_index].ball2);
                    //cout<<"OK - bn1="<<bn1<<" bn2="<<bn2<<" Ball min_step="<<min_step<<endl;

                    //remember first hitted ball
                    if ( firstHittedBall == -1 ) {
                        if (( bn1 == 0 )&&( bn2 != 0 ))
                            firstHittedBall = bn2;
                        else if (( bn1 != 0 )&&( bn2 == 0 ))
                            firstHittedBall = bn1;
                    }

                    ball_ball_hitted[bn2][bn1] = TRUE; //prevent inverse match
                } //else cout<<"NO - bn1="<<bn1<<" bn2="<<bn2<<" Ball min_step="<<min_step<<endl;
            }

            Music->OneShot("hitball");
        }

        if (( emin_step == min_step ) && flag_e ) {
            for (edge_ball_index--; (edge_ball_index >= 0) &&( edge_ball_array[edge_ball_index].hit_value == min_step) ; edge_ball_index--) {
                edge_ball_array[edge_ball_index].edge->DoCollision(edge_ball_array[edge_ball_index].ball);
                //cout<<"edge="<<(edge_ball_array[edge_ball_index].edge->edgenumber)<<" ball="<<(edge_ball_array[edge_ball_index].ball->ballnumber)<<" - Edg min_step="<<min_step<<endl;
            }

            Music->OneShot("hitedge");
        }
        edge_ball_index=0;
        ball_ball_index=0;

        step = step - min_step;
        min_step=step;
        emin_step=1;
        bmin_step=1;
        tmp_min_step=1;

        //cout<<"-----------------------------"<<endl;
        infinite_loop++;

    } while (( step > 0 )&&( step < 1 )&&( infinite_loop < 100));

    mutex.unlock();

    if( infinite_loop > 99)
        cout<<"Infinite Loop!!"<<endl;
}

/** No descriptions */
void Kbilliards::activateNextPlayer() {
    if ( actualgameoptions.TwoPlayers ) {
        if ( currentPlayerNum == 1 ) {
            //change to player two
            currentPlayerNum=2;
            //change vector color
            throwvector->setPen( QPen( QColor(blue), 2, DashLine )  );
            //change led light
            scoredisplay->setCurrentPlayer(2);
        } else {
            currentPlayerNum=1;
            throwvector->setPen( QPen( QColor(red) , 2, DashLine )  );
            scoredisplay->setCurrentPlayer(1);
            if ( actualgameoptions.AiPlayerTwo )
                AIp->hideTrajectories() ;
        }

    } else
        currentPlayerNum=1;
}

bool Kbilliards::otherBallpresence(int myballnum,double xpos,double ypos) {

    Ball *i;
    bool return_val = false;
    samuele::Vector v(xpos,ypos,0.0);
    samuele::Vector res(0,0,0);

    for(i=ingameballs.first(); i != 0; i=ingameballs.next()) {
        if ( i->ballnumber != myballnum ) {
            res = i->vect_Pos - v ;
            if ( res.abs() < 20 )
                return_val = true; // 20 = diameter of ball
        }
    }

    return return_val;
}

void Kbilliards::initgamedialog() {
    int result;
    GameOptions game_selection ;

    game_selection = actualgameoptions;

    initGameWindow game_window(this,"Game selection",&game_selection);

    result=game_window.exec(); // == 1  is done!

    if ( result == 1 ) {
        //TODO: add multiplayer choice into init game dialog window
        if ( game_selection.TwoPlayers ) {
            actualgameoptions.name_player1 = game_selection.name_player1;
            if ( ! game_selection.AiPlayerTwo )
                actualgameoptions.name_player2 = game_selection.name_player2;
            actualgameoptions.game = game_selection.game;
            actualgameoptions.TwoPlayers = game_selection.TwoPlayers;
            actualgameoptions.AiPlayerTwo = game_selection.AiPlayerTwo ;
        } else {
            actualgameoptions.name_player1 = game_selection.name_player1;
            actualgameoptions.game = game_selection.game;
            actualgameoptions.TwoPlayers = false;
            actualgameoptions.AiPlayerTwo = false;
        }

        scoredisplay->setPlayerName(1,actualgameoptions.name_player1);
        if ( actualgameoptions.AiPlayerTwo )
            scoredisplay->setPlayerName(2,"Computer");
        else
            scoredisplay->setPlayerName(2,actualgameoptions.name_player2);

        saveGameOptions();
    }

}

int Kbilliards::loadMap(QString filemapname)
{
    FILE*   f;
    BZFILE* b;
    int     nBuf;
    char    buf[ BUFSIZE ];
    int     bzerror;

    if ( filemapname.isEmpty() ) return 1;

    f = fopen ( filemapname, "rb" );
    if (!f) {
        cout << "error during data map decompression" << endl;
        return 1;
    }

    b = BZ2_bzReadOpen (&bzerror, f, 0, 0, NULL, 0);
    if (bzerror != BZ_OK) {
        cout<< "error during data map decompression" <<endl;
        BZ2_bzReadClose (&bzerror, b);
        return 1;
    }

    QByteArray array;
    QBuffer buffer( array );
    buffer.open( IO_WriteOnly );
    QTextStream ts( &buffer );

    while( bzerror == BZ_OK )  {
        nBuf = BZ2_bzRead( &bzerror, b, buf, BUFSIZE);
        if ( (bzerror == BZ_OK) || (bzerror == BZ_STREAM_END) ) {
            //write data into stream
            ts.writeRawBytes( buf, nBuf) ;
        }
    }

    buffer.close();
    fclose(f);

    if ( bzerror != BZ_STREAM_END ) {
        cout<< "error during data map decompression" <<endl;
        BZ2_bzReadClose( &bzerror, b);
        return 1;
    } else {
        BZ2_bzReadClose( &bzerror, b);
    }

    // parsing map document..

    QDomDocument doc( "kbilliardsmap" );

    if ( !doc.setContent(array) ) {
        cout<<"error loading data map"<<endl;
        return 1;
    }

    QString data;
    uchar *ptr_data;
    QImage * image;
    QCanvasPixmap * qcpixmap;
    QCanvasPixmapArray * qpixmaparray;
    QString file_img_name;

    SpriteObject *s;
    BounceSegmentOb *e;
    HoleObject *h;

    QDomElement docElem = doc.documentElement();
    QDomNode root = docElem.firstChild();

    if ( !root.isNull() ) {
        QDomElement level0 = root.toElement(); // try to convert the node to an element.

        while( !level0.isNull() ) {
            //cout << level0.tagName() << endl;

            QDomElement level1 = level0.firstChild().toElement();
            if ( level0.tagName() == "holes" ) {
                  while ( !level1.isNull() ) {
                        if ( level1.tagName() == "hole" ) {
                              h = new HoleObject(level1.attribute("x").toDouble(),level1.attribute("y").toDouble(),level1.attribute("radius").toDouble());
                              holeobjects.append(h);
                        }

                        level1 = level1.nextSibling().toElement();
                  }
            }

            if ( level0.tagName() == "segmentedges" ) {
                  while ( !level1.isNull() ) {

                        QDomElement level2 = level1.firstChild().toElement();

                        e= new BounceSegmentOb();

                        while ( !level2.isNull() ) {

                              if ( level2.tagName() == "segment" ) {
                                    e->setBounceLine(level2.attribute("fromx").toDouble(),level2.attribute("fromy").toDouble(),level2.attribute("tox").toDouble(),level2.attribute("toy").toDouble());
                              }
                              if ( level2.tagName() == "bouncedir" ) {
                                    e->setBounceDir(level2.attribute("x").toDouble(),level2.attribute("y").toDouble());
                              }

                              level2 = level2.nextSibling().toElement();
                        }

                        edgeobjects.append(e);

                        level1 = level1.nextSibling().toElement();
                  }
            }

            if ( level0.tagName() == "images" ) {
                  while ( !level1.isNull() ) {
                        //cout <<"   "<< level1.tagName() << endl;

                        if ( level1.tagName() == "image" ) {
                              QDomElement data_element= level1.firstChild().toElement();
                              if ( data_element.tagName() == "data" ) {
                                    data=data_element.text(); //data
                                    ptr_data=BinFromTextData(data);
                                    image=new QImage;
                                    image->loadFromData( (const uchar*)ptr_data, (data.length() / 2) , "PNG" );
                                    qcpixmap = new QCanvasPixmap(*image);
                                    qpixmaparray = new QCanvasPixmapArray();
                                    qpixmaparray->setImage(0,qcpixmap);
                                    s= new SpriteObject(qpixmaparray,canvas,level1.attribute("z").toInt());
                                    s->move(level1.attribute("x").toDouble(),level1.attribute("y").toDouble());
                                    s->name=level1.attribute("name","image");;
                                    spriteobjects.append(s);


                              } else {
                                    cout<<"Error on parsing map file"<<endl;
                              }
                        }

                        level1 = level1.nextSibling().toElement();
                  }
            }

            level0 = level0.nextSibling().toElement();

        }

    } else {
        cout<<"Error on loading map file"<<endl;
    }

    return 0;
}

uchar * BinFromTextData(QString &data)
{
     //read in hexdecimal (00 -- ff)
     int baSize = data.length() / 2 ;
     uchar *buffer = new uchar[ baSize ];

     for ( int i = 0; i < baSize; ++i ) {
     char h = data[ 2 * i ].latin1();
     char l = data[ 2 * i + 1 ].latin1();
     uchar r = 0;
     if ( h <= '9' )
          r += h - '0';
     else
          r += h - 'a' + 10;
     r = r << 4;
     if ( l <= '9' )
          r += l - '0';
     else
          r += l - 'a' + 10;
     buffer[ i ] = r;
     }

     return buffer;
}

#include "kbilliards.moc"

