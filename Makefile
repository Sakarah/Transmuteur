# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers sources, headers, ou changer
# des flags de compilation.
champion-srcs = prologin.cc boardsimulator.cpp utils.cpp \
                action_catalyse.cpp action_placesample.cpp action_transmute.cpp \
                action_wipeout.cpp strategy.cpp
champion-dists = prototypes.h boardsimulator.h gamesimulator.h action.h \
                 action_catalyse.h action_placesample.h action_transmute.h action_wipeout.h
champion-cxxflags = -ggdb3 -Wall

# Evite de toucher a ce qui suit
champion-dists += prologin.hh
STECHEC_LANG=cxx
include ../includes/rules.mk
