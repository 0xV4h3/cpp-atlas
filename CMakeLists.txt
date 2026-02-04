cmake_minimum_required(VERSION 3.16)
project(cpp-atlas VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Test)

# Find QScintilla
find_path(QSCINTILLA_INCLUDE_DIR Qsci/qsciscintilla.h
    PATHS
        /usr/include/x86_64-linux-gnu/qt5
        /usr/include/qt5
        /usr/local/include/qt5
        /usr/include
        /usr/local/include
)

find_library(QSCINTILLA_LIBRARY NAMES qscintilla2_qt5 qscintilla2-qt5
    PATHS
        /usr/lib/x86_64-linux-gnu
        /usr/lib
        /usr/local/lib
)

if(NOT QSCINTILLA_INCLUDE_DIR OR NOT QSCINTILLA_LIBRARY)
    message(FATAL_ERROR "QScintilla not found. Please install libqscintilla2-qt5-dev")
endif()

message(STATUS "QScintilla include dir: ${QSCINTILLA_INCLUDE_DIR}")
message(STATUS "QScintilla library: ${QSCINTILLA_LIBRARY}")

add_subdirectory(src)
add_subdirectory(tests)
enable_testing()
