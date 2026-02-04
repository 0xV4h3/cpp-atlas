cmake_minimum_required(VERSION 3.16)
project(cpp-atlas VERSION 0.2 LANGUAGES CXX)

# Qt6 requires minimum C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt6 ONLY - no Qt5 fallback to avoid mixing
find_package(Qt6 REQUIRED COMPONENTS Widgets Test)

# QScintilla for Qt6
# Package: libqscintilla2-qt6-dev (Ubuntu/Debian)
# Headers: /usr/include/qt6/Qsci or /usr/include/x86_64-linux-gnu/qt6/Qsci
# Library: libqscintilla2_qt6.so
find_path(QSCINTILLA_INCLUDE_DIR Qsci/qsciscintilla.h
    PATHS
        /usr/include/qt6
        /usr/include/x86_64-linux-gnu/qt6
        /usr/local/include/qt6
        ${Qt6Widgets_INCLUDE_DIRS}
    PATH_SUFFIXES Qsci
    NO_DEFAULT_PATH
)

# Fallback search if not found in Qt6 paths
if(NOT QSCINTILLA_INCLUDE_DIR)
    find_path(QSCINTILLA_INCLUDE_DIR Qsci/qsciscintilla.h)
endif()

find_library(QSCINTILLA_LIBRARY 
    NAMES qscintilla2_qt6 qscintilla2-qt6 libqscintilla2_qt6
    PATHS
        /usr/lib/x86_64-linux-gnu
        /usr/lib
        /usr/local/lib
        ${Qt6Widgets_LIBRARY_DIRS}
)

if(NOT QSCINTILLA_INCLUDE_DIR OR NOT QSCINTILLA_LIBRARY)
    message(FATAL_ERROR 
        "QScintilla for Qt6 not found!\n"
        "Please install:\n"
        "  Ubuntu/Debian: sudo apt install libqscintilla2-qt6-dev\n"
        "  Fedora: sudo dnf install qscintilla-qt6-devel\n"
        "  Arch: sudo pacman -S qscintilla-qt6\n"
        "  macOS: brew install qscintilla2"
    )
endif()

message(STATUS "Found QScintilla:")
message(STATUS "  Include: ${QSCINTILLA_INCLUDE_DIR}")
message(STATUS "  Library: ${QSCINTILLA_LIBRARY}")

# Export for subdirectories
set(QSCINTILLA_INCLUDE_DIR ${QSCINTILLA_INCLUDE_DIR} CACHE PATH "QScintilla include directory")
set(QSCINTILLA_LIBRARY ${QSCINTILLA_LIBRARY} CACHE FILEPATH "QScintilla library")

add_subdirectory(src)
add_subdirectory(tests)
enable_testing()
