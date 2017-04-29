HEADERS += \
    prologin.hh \
    boardsimulator.h \
    gamesimulator.h \
    actions_action.h \
    actions_transmute.h \
    actions_catalyse.h \
    actions_placesample.h \
    prototypes.h \
    actions_wipeout.h

SOURCES += \
    prologin.cc \
    boardsimulator.cpp \
    actions_transmute.cpp \
    actions_catalyse.cpp \
    actions_placesample.cpp \
    strategy.cpp \
    utils.cpp \
    actions_wipeout.cpp

DISTFILES += \
    Makefile \
    config_spectator.yml \
    config_manual.yml \
    config_debug.yml

