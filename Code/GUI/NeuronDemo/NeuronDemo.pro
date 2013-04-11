#-------------------------------------------------
#
# Project created by QtCreator 2011-04-30T12:01:23
#
#-------------------------------------------------

QT       +=  core gui opengl
TARGET = NeuronDemo
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    descriptorwidget.cpp \
    neuronwidget.cpp \
    qtglut.cpp \
    OgreVector4.cpp \
    OgreVector3.cpp \
    OgreVector2.cpp \
    OgreQuaternion.cpp \
    OgreMatrix4.cpp \
    OgreMatrix3.cpp \
    neuron.cpp \
    matrixmath.cpp \
    neuronmanager.cpp \
    desc.cpp

HEADERS  += mainwindow.h \
    descriptorwidget.h \
    neuronwidget.h \
    qtglut.h \
    quicksort.hpp \
    neuron.h \
    myexception.hpp \
    matrixmath.h \
    matrix.hpp \
    knnsearch.h \
    del_functor.hpp \
    dataio.h \
    OgreMath/OgreVector4.h \
    OgreMath/OgreVector3.h \
    OgreMath/OgreVector2.h \
    OgreMath/OgreQuaternion.h \
    OgreMath/OgreMatrix4.h \
    OgreMath/OgreMatrix3.h \
    OgreMath/OgreMath.h \
    OgreMath/OgreCommon.h \
    neuronmanager.h \
    desc.h


FORMS    += mainwindow.ui

OTHER_FILES += \
    NeuronDemo.pro.user
