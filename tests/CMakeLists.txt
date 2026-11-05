file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*/*.c"
)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_include_directories(
        ${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include
    )
    target_link_libraries(${TEST_NAME} PRIVATE unity)

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
