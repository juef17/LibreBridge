VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_BUILD = 1

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"

TEMPLATE = app
TARGET = LibreBridge
INCLUDEPATH += .

QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 

CONFIG += release
CONFIG += warn_on
CONFIG += c++14

QMAKE_CXXFLAGS += -Wall -Wextra -pedantic -fPIC
#QMAKE_CXX = g++

# Input
HEADERS += src/AIPlayer.hpp \
           src/Bid.hpp \
           src/Card.hpp \
           src/Contract.hpp \
           src/Game.hpp \
           src/HumanPlayer.hpp \
           src/LibreBridge.hpp \
           src/Misc.hpp \
           src/Player.hpp \
           src/AIPlayers/AI_Random.hpp \
           src/gui/WelcomeWindow.hpp \
           src/gui/PlayWindow.hpp
SOURCES += src/AIPlayer.cpp \
           src/Bid.cpp \
           src/Card.cpp \
           src/Contract.cpp \
           src/Game.cpp \
           src/HumanPlayer.cpp \
           src/LibreBridge.cpp \
           src/Misc.cpp \
           src/Player.cpp \
           src/AIPlayers/AI_Random.cpp \
           src/gui/WelcomeWindow.cpp \
           src/gui/PlayWindow.cpp
