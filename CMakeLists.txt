cmake_minimum_required(VERSION 3.10...3.30)
project(cutils VERSION 0.1 LANGUAGES C)

option(ENABLE_COVERAGE "Enable code coverage" OFF)

set(CFLAGS
    -Werror -Wall -Wextra -Wconversion -Wformat=2 -Wcast-align
    -Wstrict-prototypes -Wredundant-decls -Wundef -Wfloat-equal
    -Wpointer-arith -Wswitch-default -Wswitch-enum -Winit-self
    -Wcast-qual -Wnested-externs -Wstrict-overflow=5 -O0 -g3
    -D_DEFAULT_SOURCE "-fmacro-prefix-map=${CMAKE_SOURCE_DIR}/="
)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CFLAGS ${CFLAGS}
        -Wlogical-op
        -Wstrict-overflow=5
        -Wbad-function-cast
    )
endif()

set(CUTILS_SOURCE
    ./src/mem.c
)

add_library(cutils STATIC ${CUTILS_SOURCE})
add_library(cutils::cutils ALIAS cutils)

target_include_directories(cutils PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/cutils
)

target_include_directories(cutils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_compile_options(cutils PRIVATE ${CFLAGS})

# Coverage
if(ENABLE_COVERAGE)
    if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
        message(FATAL_ERROR "Coverage is supported only with GCC")
    endif()

    target_compile_options(cutils PRIVATE
        --coverage
        -fprofile-abs-path
    )

    target_link_options(cutils INTERFACE --coverage)
endif()

# Tests
enable_testing()

add_subdirectory(third_party/Unity)
add_subdirectory(tests)

# Coverage report
if(ENABLE_COVERAGE)
    find_program(GCOVR_EXECUTABLE gcovr REQUIRED)

    add_custom_target(coverage
        COMMAND ${CMAKE_CTEST_COMMAND}
            --output-on-failure
            --test-dir ${CMAKE_BINARY_DIR}

        COMMAND ${CMAKE_COMMAND} -E make_directory
            ${CMAKE_BINARY_DIR}/coverage

        COMMAND ${GCOVR_EXECUTABLE}
            --root ${CMAKE_SOURCE_DIR}
            --filter ${CMAKE_SOURCE_DIR}/src/
            --html-details ${CMAKE_BINARY_DIR}/coverage/index.html
            ${CMAKE_BINARY_DIR}

        COMMAND ${CMAKE_COMMAND} -E echo
            "Coverage report: ${CMAKE_BINARY_DIR}/coverage/index.html"

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running tests and generating coverage report"
        VERBATIM
    )
endif()
