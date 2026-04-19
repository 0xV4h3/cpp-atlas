# Translation files
set(TS_FILES ${CMAKE_SOURCE_DIR}/resources/cpp-atlas_hy_AM.ts)

# Collect source files by module
set(CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Project.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/FileManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/RecentProjectsManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/ProjectManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/AppSettings.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/output/TerminalWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/output/ProblemsWidget.cpp
)

set(UI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/FileTreeWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/ThemeManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/GotoLineDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/FindReplaceDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/WelcomeScreen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/NewFileDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/NewProjectDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/InsightsWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/AssemblyWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/BenchmarkWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/BenchmarkChartWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/AnalysisPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/LoginDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/QuizModeWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/QuizSelectionWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/QuizSessionWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/QuizResultsWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/UserProfileWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/CustomTestBuilderWidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/SettingsDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/QuizAdminPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/AdminQuestionEditorDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/AdminQuizEditorDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/AdminContentMaintenanceWidget.cpp
)

# Tools module - runners and configuration
set(TOOLS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/ToolsConfig.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/CppInsightsRunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/AssemblyRunner.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/BenchmarkRunner.cpp
)

# Quiz module — database, user management, engine
set(QUIZ_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/QuizDatabase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/UserManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/AdminAccessController.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/ContentPatchService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/QuizRepository.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/QuizEngine.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/AnswerEvaluationService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/ProgressAnalyzer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/QuizExporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/ExportCrypto.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/QuizContentExporter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/AdaptiveSystem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/AdminContentService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/ContentValidationService.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/quiz/AdminPatchWorkflowService.cpp
)

set(APP_SOURCES
    ${CORE_SOURCES}
    ${EDITOR_SOURCES}
    ${COMPILER_SOURCES}
    ${OUTPUT_SOURCES}
    ${UI_SOURCES}
    ${TOOLS_SOURCES}
    ${QUIZ_SOURCES}
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
    ${QSCINTILLA_INCLUDE_DIR}
    ${GOOGLE_BENCHMARK_INCLUDE_DIR}
)

# Link Qt Charts if available
if(QT_CHARTS_AVAILABLE)
    target_compile_definitions(CppAtlasLib PUBLIC CPPATLAS_CHARTS_AVAILABLE)
    target_link_libraries(CppAtlasLib PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Charts
        Qt${QT_VERSION_MAJOR}::Sql
        ${QSCINTILLA_LIBRARY}
    )
else()
    target_link_libraries(CppAtlasLib PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
        ${QSCINTILLA_LIBRARY}
    )
endif()

# CppInsights default path compile-time hint
if(CPPINSIGHTS_EXECUTABLE)
    target_compile_definitions(CppAtlasLib PRIVATE
        CPPINSIGHTS_DEFAULT_PATH="${CPPINSIGHTS_EXECUTABLE}")
endif()

# Google Benchmark paths for ToolsConfig initialization
target_compile_definitions(CppAtlasLib PRIVATE
    GOOGLE_BENCHMARK_INCLUDE_DIR="${GOOGLE_BENCHMARK_INCLUDE_DIR}"
    GOOGLE_BENCHMARK_LIB_TARGET="${GOOGLE_BENCHMARK_LIBRARY}"
)

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
