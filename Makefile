# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers sources, headers, ou changer
# des flags de compilation.
champion-srcs = prologin.cc boardsimulator.cpp utils.cpp \
                actions_catalyse.cpp actions_placesample.cpp actions_transmute.cpp \
                actions_wipeout.cpp strategy.cpp
champion-dists = prototypes.h boardsimulator.h gamesimulator.h actions_action.h \
                 actions_catalyse.h actions_placesample.h actions_transmute.h actions_wipeout.h
champion-cxxflags = -ggdb3 -Wall

# Evite de toucher a ce qui suit
champion-dists += prologin.hh
STECHEC_LANG=cxx
include ../includes/rules.mk
