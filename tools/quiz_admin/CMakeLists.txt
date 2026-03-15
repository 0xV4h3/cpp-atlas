add_executable(quiz_admin
    main.cpp
    QuizAdminCli.h
    QuizAdminCli.cpp
)

# CppAtlasLib provides Qt::Widgets, Qt::Sql, Qt::Core, include dirs, etc.
# Its PUBLIC include dirs (${CMAKE_SOURCE_DIR}/include, ${CMAKE_BINARY_DIR})
# propagate automatically, so we only add the tool's own source directory.
target_link_libraries(quiz_admin
    PRIVATE
        CppAtlasLib
)

target_include_directories(quiz_admin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Console executable: no WIN32_EXECUTABLE, no MACOSX_BUNDLE.
# quiz_admin writes to stdout/stderr and must be runnable from a terminal.
set_target_properties(quiz_admin PROPERTIES
    OUTPUT_NAME quiz_admin
)

include(GNUInstallDirs)
install(TARGETS quiz_admin
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
