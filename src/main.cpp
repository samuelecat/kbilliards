/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : gio gen 24 13:30:42 CET 2002
    copyright            : (C) 2001 by Samuele Catuzzi
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

#include <kcmdlineargs.h>
#include <kaboutdata.h>
#include <klocale.h>

#include "kbilliards.h"

#include "config.h"

static const char *description = "kbilliards";
// INSERT A DESCRIPTION FOR YOUR APPLICATION HERE


static KCmdLineOptions options[] =
{
  { 0, 0, 0 }
  // INSERT YOUR COMMANDLINE OPTIONS HERE
};

int main(int argc, char *argv[])
{

  KAboutData aboutData( "kbilliards", "KBilliards",
    VERSION, description, KAboutData::License_GPL,
    "(c) 2002, Samuele Catuzzi", 0, 0, "samuele.c@hostnotfound.it");
  aboutData.addAuthor("Samuele Catuzzi",0, "samuele.c@hostnotfound.it");
  KCmdLineArgs::init( argc, argv, &aboutData );
  KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

  KApplication a;
  Kbilliards *kbilliards = new Kbilliards(0,"KBilliards");
  a.setMainWidget(kbilliards);
  kbilliards->show();

  return a.exec();
}
