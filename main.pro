CONFIG += debug
TARGET = main
INCLUDEPATH += D:\Programmes\Cygwin\usr\include\libxml2
HEADERS += OpenCellDialog.hpp
HEADERS += SaveCellDialog.hpp
HEADERS += CellViewer.hpp
HEADERS += CellWidget.hpp
HEADERS += InstancesWidget.hpp
HEADERS += CellsModel.hpp
HEADERS += InstancesModel.hpp
HEADERS += CellsLib.hpp
HEADERS += Indentation.hpp
HEADERS += Box.hpp
HEADERS += Point.hpp
HEADERS += Line.hpp
HEADERS += Net.hpp
HEADERS += Node.hpp
HEADERS += Shape.hpp
HEADERS += StringToInt.hpp
HEADERS += Symbol.hpp
HEADERS += Term.hpp
HEADERS += Instance.hpp
HEADERS += Cell.hpp
HEADERS += xmlutil.hpp
SOURCES += OpenCellDialog.cpp
SOURCES += SaveCellDialog.cpp
SOURCES += CellViewer.cpp
SOURCES += CellWidget.cpp
SOURCES += InstancesWidget.cpp
SOURCES += CellsModel.cpp
SOURCES += InstancesModel.cpp
SOURCES += CellsLib.cpp
SOURCES += Indentation.cpp
SOURCES += Box.cpp
SOURCES += Point.cpp
SOURCES += Line.cpp
SOURCES += Net.cpp
SOURCES += Node.cpp
SOURCES += Shape.cpp
SOURCES += StringToInt.cpp
SOURCES += Symbol.cpp
SOURCES += Term.cpp
SOURCES += Instance.cpp
SOURCES += Cell.cpp
SOURCES += xmlutil.cpp
SOURCES += main.cpp
QT += gui widgets core
LIBS += -lxml2