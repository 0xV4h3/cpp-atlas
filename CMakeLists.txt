cmake_minimum_required(VERSION 3.16)
project(cpp-atlas VERSION 0.2 LANGUAGES CXX)

# Qt6 requires minimum C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Try Qt6 first, fall back to Qt5 if not available
find_package(Qt6 COMPONENTS Widgets Test QUIET)
if(Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_VERSION_MAJOR 6)
    # QScintilla for Qt6
    find_path(QSCINTILLA_INCLUDE_DIR Qsci/qsciscintilla.h
        PATHS
            /usr/include/qt6
            /usr/include/x86_64-linux-gnu/qt6
            /usr/local/include/qt6
            ${Qt6Widgets_INCLUDE_DIRS}
        PATH_SUFFIXES Qsci
    )
    
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
else()
    message(STATUS "Qt6 not found, falling back to Qt5")
    find_package(Qt5 REQUIRED COMPONENTS Widgets Test)
    set(QT_VERSION_MAJOR 5)
    # QScintilla for Qt5
    find_path(QSCINTILLA_INCLUDE_DIR Qsci/qsciscintilla.h
        PATHS
            /usr/include/x86_64-linux-gnu/qt5
            /usr/include/qt5
            /usr/local/include/qt5
            /usr/include
            /usr/local/include
    )
    
    find_library(QSCINTILLA_LIBRARY 
        NAMES qscintilla2_qt5 qscintilla2-qt5
        PATHS
            /usr/lib/x86_64-linux-gnu
            /usr/lib
            /usr/local/lib
    )
endif()

if(NOT QSCINTILLA_INCLUDE_DIR OR NOT QSCINTILLA_LIBRARY)
    message(FATAL_ERROR 
        "QScintilla for Qt${QT_VERSION_MAJOR} not found!\n"
        "Please install:\n"
        "  Qt6: Ubuntu/Debian: sudo apt install libqscintilla2-qt6-dev\n"
        "  Qt5: Ubuntu/Debian: sudo apt install libqscintilla2-qt5-dev\n"
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
