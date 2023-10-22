greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

SOURCES += \
    src/debugdraw.cpp \
    src/fontmanager.cpp \
    src/gamescene.cpp \
    src/main.cpp \
    src/piece.cpp \
    src/pixmapmanager.cpp \
    src/statistics.cpp \
    src/view.cpp \
    src/world.cpp

HEADERS += \
    src/debugdraw.h \
    src/fontmanager.h \
    src/gamescene.h \
    src/piece.h \
    src/pixmapmanager.h \
    src/resourceholder.h \
    src/statistics.h \
    src/utils.h \
    src/view.h \
    src/world.h

RESOURCES += \
    resource.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/release/ -lbox2d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/debug/ -lbox2d
else:unix: LIBS += -L$$PWD/libs/ -lbox2d

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/release/libbox2d.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/debug/libbox2d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libs/release/box2d.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libs/debug/box2d.lib
else:unix: PRE_TARGETDEPS += $$PWD/libs/libbox2d.a
