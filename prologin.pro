HEADERS += \
    prologin.hh \
    boardsimulator.h \
    gamesimulator.h \
    actions/action.h \
    actions/transmute.h \
    actions/catalyse.h \
    actions/placesample.h \
    prototypes.h \
    actions/wipeout.h

SOURCES += \
    prologin.cc \
    boardsimulator.cpp \
    actions/transmute.cpp \
    actions/catalyse.cpp \
    actions/placesample.cpp \
    strategy.cpp \
    utils.cpp \
    actions/wipeout.cpp

DISTFILES += \
    Makefile \
    config_spectator.yml \
    config_manual.yml \
    config_debug.yml

