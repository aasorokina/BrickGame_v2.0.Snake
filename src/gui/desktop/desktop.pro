QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 static release

QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++17

TEMPLATE = app

contains(DEFINES, SNAKE) {
    LIBS += $$PWD/../../snake.a
    TARGET = snake_desktop
} else:contains(DEFINES, TETRIS) {
    LIBS += $$PWD/../../tetris.a
    TARGET = tetris_desktop
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gamefield.cpp \
    main.cpp \
    gamewindow.cpp

HEADERS += \
    gamefield.h \
    gamewindow.h \
    ../../brick_game/brickgame_api.h \

FORMS += \
    gamewindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += resourses.qrc
