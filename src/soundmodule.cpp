/***************************************************************************
                          soundmodule.cpp  -  description
                             -------------------
    begin                : ven lug 12 2002
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

#include "soundmodule.h"

SoundModule::SoundModule(QString p_sound,QObject *parent)
     : QObject(parent), path_sound(p_sound), sLoop(false)

{
  musicEngine = new SoundEngine;
  effectEngine = new SoundEngine;
     stop();
  enablemusic=true;
  loop_ok=true;
  numbofloop=0;
}

SoundModule::~SoundModule(){
 delete musicEngine;
 delete effectEngine;
}
/** No descriptions */
void SoundModule::setLoop(bool state ){
 sLoop=state;
 if (state)  startTimer( 500 );
}
/** No descriptions */
void SoundModule::stop(){
     unfinished = false;
  musicEngine->stop();
}
/** No descriptions */
void SoundModule::timerEvent( QTimerEvent * ){

  if (enablemusic && loop_ok) {
     if(musicEngine->state() == SoundEngine::Stop && unfinished) // se lo stato e' Stop e la traccia non e' finita
     {
          if(sLoop) { // bool sLoop; settato da  setLoop(bool);
        numbofloop++;
        if ( numbofloop <= 2 )
                   play();
        else {
            numbofloop=0;
            loop_ok=false;
            QTimer::singleShot(15000,this,SLOT(setMusicOn()));
        }
      }
          else
               stop(); //     unfinished = false; musicEngine->stop();
     }
     else
      if(musicEngine->state() != SoundEngine::Stop)     unfinished = true;
  } 

}
/** No descriptions */
void SoundModule::loadfile(QString file){
  stop();
  QTimerEvent * e; // e(QEvent::Timer) ;
  KURL pathfile(path_sound + file );

  if ( musicEngine->load(pathfile) )
       timerEvent( e ); // ( &e ); 
}
/** No descriptions */
void SoundModule::play(){
  if ( enablemusic )  musicEngine->play();
}                
/** No descriptions */
void SoundModule::OneShot(QString file){
  if ( enableeffects ) {
     effectEngine->stop();
     QString pathfile(path_sound + file + (QString)".wav" );
     effectEngine->load(pathfile);
     effectEngine->play();
  }
}
/** if true Music can play if false not  */
void SoundModule::enableMusic(bool state){
  enablemusic=state;
  if ( !enablemusic ) stop();
  else play();
}
/** No descriptions */
void SoundModule::enableEffects(bool state){
  enableeffects=state;
}
/** retrive value of enableeffects */
bool SoundModule::getEffectState(){
 return  enableeffects;
}
/** retrive value of enablemusic  */
bool SoundModule::getMusicState(){
 return  enablemusic;
}
/** No descriptions */
void SoundModule::setMusicOn(){
  loop_ok=true;
}

#include "soundmodule.moc"

