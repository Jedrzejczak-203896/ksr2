#-------------------------------------------------
#
# Project created by QtCreator 2011-12-30T17:00:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = db_summarization
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../qjson/include/

LIBS += -L../qjson/lib -lqjson

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
	trianglefunction.cpp \
	trapezoidfunction.cpp \
	discretefunction.cpp \
	basicfuzzyset.cpp \
	alphacut.cpp \
	fuzzyset.cpp \
	union.cpp \
	intersection.cpp \
	complement.cpp \
	support.cpp \
	quantifier.cpp \
	qualitymeasures.cpp \
	MembershipFunctionFactory.cpp \
    summarization.cpp \
    linguisticvalue.cpp \
    fileparser.cpp \
    summarizationgenerator.cpp

HEADERS += \
	membershipfuncineterface.h \
	trianglefunction.h \
	trapezoidfunction.h \
	discretefunction.h \
	basicfuzzyset.h \
	fuzzyset.h \
	alphacut.h \
	union.h \
	intersection.h \
	complement.h \
	support.h \
	quantifier.h \
	qualitymeasures.h \
	MembershipFunctionFactory.h \
    linguisticvalue.h \
    summarization.h \
    fileparser.h \
    summarizationgenerator.h
