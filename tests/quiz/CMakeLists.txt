add_executable(AdminAccessTests
    ${CMAKE_CURRENT_SOURCE_DIR}/test_admin_access.cpp
)

target_link_libraries(AdminAccessTests
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        CppAtlasLib
)

add_test(NAME AdminAccessTests COMMAND AdminAccessTests)

# ── ContentPatchService tests ─────────────────────────────────────────────────
add_executable(ContentPatchServiceTests
    ${CMAKE_CURRENT_SOURCE_DIR}/test_content_patch_service.cpp
)

target_link_libraries(ContentPatchServiceTests
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        CppAtlasLib
)

add_test(NAME ContentPatchServiceTests COMMAND ContentPatchServiceTests)

# ── QuizAdminCli tests ────────────────────────────────────────────────────────
add_executable(QuizAdminCliTests
    ${CMAKE_CURRENT_SOURCE_DIR}/test_quiz_admin_cli.cpp
)

target_link_libraries(QuizAdminCliTests
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        CppAtlasLib
)

# The CLI test includes QuizAdminCli.h/.cpp directly from the tools directory.
target_sources(QuizAdminCliTests PRIVATE
    ${CMAKE_SOURCE_DIR}/tools/quiz_admin/QuizAdminCli.cpp
)

target_include_directories(QuizAdminCliTests PRIVATE
    ${CMAKE_SOURCE_DIR}/tools/quiz_admin
)

add_test(NAME QuizAdminCliTests COMMAND QuizAdminCliTests)

# ── FillBlank evaluation tests ────────────────────────────────────────────────
add_executable(FillBlankEvaluationTests
    ${CMAKE_CURRENT_SOURCE_DIR}/test_fill_blank_evaluation.cpp
)

target_link_libraries(FillBlankEvaluationTests
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        CppAtlasLib
)

add_test(NAME FillBlankEvaluationTests COMMAND FillBlankEvaluationTests)
