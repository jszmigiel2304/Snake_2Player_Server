QT -= gui
QT += network

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        c_board.cpp \
        c_eventcontroller.cpp \
        c_game.cpp \
        c_gamescontroller.cpp \
        c_mytimer.cpp \
        c_parser.cpp \
        c_player.cpp \
        c_server.cpp \
        c_snake.cpp \
        c_socket.cpp \
        c_wallet.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    _myData.h \
    c_board.h \
    c_eventcontroller.h \
    c_game.h \
    c_gamescontroller.h \
    c_mytimer.h \
    c_parser.h \
    c_player.h \
    c_server.h \
    c_snake.h \
    c_socket.h \
    c_wallet.h
