# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers sources, headers, ou changer
# des flags de compilation.
champion-srcs = prologin.cc boardsimulator.cpp utils.cpp \
                actions/catalyse.cpp actions/placesample.cpp actions/transmute.cpp \
                strategy.cpp
champion-dists = prototypes.h boardsimulator.h gamesimulator.h actions/action.h \
                 actions/catalyse.h actions/placesample.h actions/transmute.h
champion-cxxflags = -ggdb3 -Wall

# Evite de toucher a ce qui suit
champion-dists += prologin.hh
STECHEC_LANG=cxx
include ../includes/rules.mk
