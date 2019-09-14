# Application info
TEMPLATE = app
DESTDIR = bin
TARGET = bn-inference
VERSION = 1.0

# Qt config
CONFIG += c++11 console
CONFIG -= app_bundle
QT -= gui
QT += core

# Libs
#LIBS +=

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

# Include paths
INCLUDEPATH *= . src

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/bayesiannetwork/bayesiannetwork.cc \
    src/bayesiannetwork/node/node.cc \
    src/main.cc \
    src/utils/normal.cc \
    src/inference/inference.cc \
    src/utils/utils.cc

HEADERS += \
    src/bayesiannetwork/bayesiannetwork.hh \
    src/bayesiannetwork/node/node.hh \
    src/utils/normal.hh \
    src/inference/inference.hh \
    src/utils/utils.hh

DISTFILES += \
    data/gas-bn.json
