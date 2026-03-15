add_executable(AdminAccessTests
    ${CMAKE_CURRENT_SOURCE_DIR}/test_admin_access.cpp
)

target_link_libraries(AdminAccessTests
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        CppAtlasLib
)

add_test(NAME AdminAccessTests COMMAND AdminAccessTests)
