TARGET = chessplugin
TEMPLATE = lib
QT += qt3support 
INCLUDEPATH += /usr/include ../../include /home/dimon/qutim/include
CONFIG += plugin

# Input
HEADERS += gameboard.h \
           xpm/black_bishop.xpm \
           xpm/black_castle.xpm \
           xpm/black_king.xpm \
           xpm/black_knight.xpm \
           xpm/black_pawn.xpm \
           xpm/black_queen.xpm \
           xpm/white_bishop.xpm \
           xpm/white_castle.xpm \
           xpm/white_king.xpm \
           xpm/white_knight.xpm \
           xpm/white_pawn.xpm \
           xpm/white_queen.xpm \
           xpm/chess.xpm \
           xpm/quit.xpm \
           xpm/new_game.xpm \
	   chessplugin.h

SOURCES += gameboard.cpp

SOURCES += chessplugin.cpp

RESOURCES += chessplugin.qrc
