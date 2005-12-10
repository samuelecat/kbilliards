/***************************************************************************
                          configuration.h  -  description
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
#ifndef CONFIGURATION_H
#define CONFIGURATION_H

struct Configuration {
        double decel;
        double hitballdecel;
        double hitedgedecel;
        int check_speed;
        int angle_factor;
       } ;

struct MusicConfiguration {
        bool enablemusic;
        bool enableeffects;
       } ;

struct GameOptions {
        QString name_player1;
        QString name_player2;
        int game;
        // if true there are two players in game
        bool TwoPlayers;
        // if true player two is a Computer
        bool AiPlayerTwo;
       } ;

#endif
