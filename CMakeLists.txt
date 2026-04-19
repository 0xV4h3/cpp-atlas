cmake_minimum_required(VERSION 3.16)
project(cpp-atlas VERSION 0.1 LANGUAGES CXX)

# Qt6 requires minimum C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ============================================================
# Developer-only build flag
# Enabled automatically for Debug; must be explicitly set for Release.
# Exposes: admin panel, quiz_admin CLI, admin user creation.
# In non-dev builds ALL admin UI / CLI / API is stripped at compile time.
# ============================================================
option(CPPATLAS_DEV_BUILD
    "Enable developer-only admin tools (admin panel, quiz_admin CLI). Auto-ON for Debug."
    OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CPPATLAS_DEV_BUILD ON CACHE BOOL "" FORCE)
    message(STATUS "Dev build detected — admin tools ENABLED")
endif()

if(CPPATLAS_DEV_BUILD)
    message(STATUS "CPPATLAS_DEV_BUILD=ON  → admin panel and quiz_admin CLI included")
    add_compile_definitions(CPPATLAS_DEV_BUILD)
else()
    message(STATUS "CPPATLAS_DEV_BUILD=OFF → admin panel and quiz_admin CLI EXCLUDED")
endif()

# Try Qt6 first, fall back to Qt5 if not available
find_package(Qt6 COMPONENTS Widgets Test Sql QUIET)
if(Qt6_FOUND)
    message(STATUS "Using Qt6")
    set(QT_VERSION_MAJOR 6)
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
    find_package(Qt5 REQUIRED COMPONENTS Widgets Test Sql)
    set(QT_VERSION_MAJOR 5)
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

# ------- Emoji font check (POSIX/WSL Linux only) -----------
if(UNIX AND NOT APPLE)
    find_path(EMOJI_FONT_PATH NAMES "NotoColorEmoji.ttf"
        PATHS /usr/share/fonts/truetype/noto /usr/share/fonts/ /usr/local/share/fonts/
        NO_DEFAULT_PATH
    )
    if(NOT EMOJI_FONT_PATH)
        message(WARNING "
========================================================================
Emoji font 'fonts-noto-color-emoji' is NOT installed!
Some emojis (for topics, UI) may appear as blank squares.

To fix: sudo apt install fonts-noto-color-emoji
========================================================================
")
    endif()
endif()

set(QSCINTILLA_INCLUDE_DIR ${QSCINTILLA_INCLUDE_DIR} CACHE PATH "QScintilla include directory")
set(QSCINTILLA_LIBRARY ${QSCINTILLA_LIBRARY} CACHE FILEPATH "QScintilla library")

# ============================================================
# Google Benchmark (FetchContent)
# ============================================================
include(FetchContent)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.1
    GIT_SHALLOW    TRUE
)
set(BENCHMARK_ENABLE_TESTING          OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS      OFF CACHE BOOL "" FORCE)
set(BENCHMARK_INSTALL_DOCS            OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_ASSEMBLY_TESTS   OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)

set(GOOGLE_BENCHMARK_INCLUDE_DIR
    "${googlebenchmark_SOURCE_DIR}/include"
    CACHE PATH "Google Benchmark include directory")
set(GOOGLE_BENCHMARK_LIBRARY
    benchmark
    CACHE STRING "Google Benchmark library target")

message(STATUS "Google Benchmark source: ${googlebenchmark_SOURCE_DIR}")

# ============================================================
# CppInsights
# ============================================================
find_program(CPPINSIGHTS_EXECUTABLE
    NAMES insights insights.exe
    PATHS
        "${CMAKE_SOURCE_DIR}/third_party/cppinsights/bin"
        /usr/local/bin
        /usr/bin
        "$ENV{HOME}/.local/bin"
    DOC "Path to cppinsights binary"
)

if(CPPINSIGHTS_EXECUTABLE)
    message(STATUS "Found cppinsights: ${CPPINSIGHTS_EXECUTABLE}")
else()
    message(STATUS "cppinsights not found - feature will require manual setup via Tools > Settings")
endif()

# ============================================================
# Qt Charts — optional
# ============================================================
if(Qt6_FOUND)
    find_package(Qt6 COMPONENTS Charts QUIET)
    if(Qt6Charts_FOUND)
        message(STATUS "Qt6 Charts found - chart visualizations enabled")
        set(QT_CHARTS_AVAILABLE TRUE)
    else()
        message(STATUS "Qt6 Charts NOT found - install qt6-charts-dev for chart support")
        set(QT_CHARTS_AVAILABLE FALSE)
    endif()
else()
    find_package(Qt5 COMPONENTS Charts QUIET)
    if(Qt5Charts_FOUND)
        message(STATUS "Qt5 Charts found - chart visualizations enabled")
        set(QT_CHARTS_AVAILABLE TRUE)
    else()
        message(STATUS "Qt5 Charts NOT found - install libqt5charts5-dev for chart support")
        set(QT_CHARTS_AVAILABLE FALSE)
    endif()
endif()

add_subdirectory(src)
enable_testing()

# quiz_admin CLI — dev builds only
if(CPPATLAS_DEV_BUILD)
    add_subdirectory(tools/quiz_admin)
endif()

add_subdirectory(tests)
