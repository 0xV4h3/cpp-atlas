file(GLOB TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(CppAtlasTests ${TEST_SOURCES})

target_link_libraries(CppAtlasTests
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Test
        CppAtlasLib
)
add_test(NAME RunUnitTests COMMAND CppAtlasTests)
