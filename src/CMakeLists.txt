# Translation files
set(TS_FILES ${CMAKE_SOURCE_DIR}/resources/cpp-atlas_hy_AM.ts)

# Collect source files by module
set(CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Project.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/FileManager.cpp
)

set(EDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/editor/CodeEditor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor/EditorTabWidget.cpp
)

set(COMPILER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/compiler/GccCompiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compiler/ClangCompiler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/compiler/CompilerRegistry.cpp
)

set(OUTPUT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/output/OutputPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/output/BuildOutputWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/output/RunOutputWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/output/ProblemsWidget.cpp
)

set(UI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/FileTreeWidget.cpp
)

# Collect all sources
set(APP_SOURCES
    ${CORE_SOURCES}
    ${EDITOR_SOURCES}
    ${COMPILER_SOURCES}
    ${OUTPUT_SOURCES}
    ${UI_SOURCES}
)

# Collect headers
file(GLOB_RECURSE APP_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")

# UI files
file(GLOB UI_FILES "${CMAKE_SOURCE_DIR}/ui/*.ui")

# Resource files
file(GLOB QRC_FILES "${CMAKE_SOURCE_DIR}/resources/*.qrc")

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Build app logic library
add_library(CppAtlasLib STATIC
    ${APP_SOURCES}
    ${APP_HEADERS}
    ${UI_FILES}
    ${QRC_FILES}
)

target_include_directories(CppAtlasLib PUBLIC
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CppAtlasLib PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)

# Main executable
add_executable(CppAtlas main.cpp)

target_link_libraries(CppAtlas PRIVATE CppAtlasLib Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(CppAtlas PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CppAtlas
)

include(GNUInstallDirs)
install(TARGETS CppAtlas
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CppAtlas)
endif()