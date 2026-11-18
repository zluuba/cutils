file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/*/*.c"
)

set(CFLAG "-fmacro-prefix-map=${CMAKE_SOURCE_DIR}/=")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SOURCE})

    target_link_libraries(${TEST_NAME} PRIVATE
        cutils
        unity
    )

    target_compile_options(${TEST_NAME} PRIVATE ${CFLAG})

    if(ENABLE_COVERAGE)
        target_compile_options(${TEST_NAME} PRIVATE
            --coverage
            -O0
            -g3
            -fprofile-abs-path
        )
    endif()

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
