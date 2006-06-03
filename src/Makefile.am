## Makefile.am for kbilliards

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kbilliards

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# the library search path. 
kbilliards_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
kbilliards_LDADD = $(LIB_KDECORE) -lartskde -lbz2 $(LIB_KFILE) $(LIB_KDEPRINT)

# which sources should be compiled for kbilliards
kbilliards_SOURCES = main.cpp kbilliards.cpp ball.cpp ball.h configuration.h \
			dlgscores.cpp dlgscores.h edgeobject.cpp edgeobject.h mcanvas.cpp mcanvas.h \
			powerbar.cpp powerbar.h psettings.cpp psettings.h soundengine.cpp soundengine.h \
			soundmodule.cpp soundmodule.h throwvector.cpp throwvector.h confdialog.ui \
			dlgscoresui.ui samuele.cpp samuele.h scoredisplay.cpp canvasview.cpp \
			initgamewindow.cpp initgamewindowui.ui aiplayer.cpp holeobject.cpp bouncesegmentob.cpp \
	kimagenumbers.cpp

# these are the headers for your project
noinst_HEADERS = kbilliards.h scoredisplay.h canvasview.h initgamewindow.h \
			aiplayer.h holeobject.h bouncesegmentob.h kimagenumbers.h

# client stuff

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kbilliards.pot

KDE_ICON = kbilliards

# this is where the kdelnk file will go 
kdelnkdir   = $(kde_appsdir)/Games
kdelnk_DATA = kbilliards.desktop

# this is where the XML-GUI resource file goes
rcdir = $(kde_datadir)/kbilliards
rc_DATA = kbilliardsui.rc
