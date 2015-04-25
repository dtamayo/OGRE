TEMPLATE = app

QT += core gui xml opengl

INCLUDEPATH += . ./OrbitalReaders ./OrbitalDisplays ./Eigen # ./ffmpeg /usr/local/lib

#LIBS += -L/usr/local/lib -lavdevice -lavfilter -lavformat -lavcodec -lpostproc -lswresample -lswscale -lavutil
#LIBS += -lz -lbz2 -liconv -lx264

DEPENDPATH += . ./OrbitalReaders ./OrbitalDisplays ./Eigen # ./ffmpeg

include(OrbitalReaders/OrbitalReaders.pri)
include(OrbitalDisplays/OrbitalDisplays.pri)
include(Eigen/Eigen.pri)
include(Helpers/Helpers.pri)
#include(ffmpeg/ffmpeg.pri)
