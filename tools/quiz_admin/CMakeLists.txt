# quiz_admin/CMakeLists.txt
#
# This target is only added to the build when CPPATLAS_DEV_BUILD is ON.
# The condition is enforced at the root CMakeLists.txt level:
#   if(CPPATLAS_DEV_BUILD)
#       add_subdirectory(tools/quiz_admin)
#   endif()
#
# A second line of defence is the static_assert in main.cpp which causes a
# compile error if CPPATLAS_DEV_BUILD is not defined, preventing accidental
# builds via direct cmake invocation.

add_executable(quiz_admin
    main.cpp
    QuizAdminCli.h
    QuizAdminCli.cpp
)

target_link_libraries(quiz_admin
    PRIVATE
        CppAtlasLib
)

target_include_directories(quiz_admin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Propagate the dev flag into the CLI binary's compilation units
target_compile_definitions(quiz_admin PRIVATE
    CPPATLAS_DEV_BUILD
)

set_target_properties(quiz_admin PROPERTIES
    OUTPUT_NAME quiz_admin
)

message(STATUS "quiz_admin CLI will be built (dev build)")
