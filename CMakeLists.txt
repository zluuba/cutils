cmake_minimum_required(VERSION 3.10...3.30)
project(cutils VERSION 0.1 LANGUAGES C)

set(CFLAGS
    -Werror -Wall -Wextra -Wshadow -Wconversion -Wformat=2 -Wcast-align
    -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wredundant-decls
    -Wundef -Wfloat-equal -Wpointer-arith -Wswitch-default -Wswitch-enum -Winit-self
    -Wcast-qual -Wnested-externs -Wstrict-overflow=5 -O0 -g3 -D_DEFAULT_SOURCE
    "-fmacro-prefix-map=${CMAKE_SOURCE_DIR}/="
)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CFLAGS ${CFLAGS}
        -Wlogical-op
        -Wstrict-overflow=5
        -Wbad-function-cast
    )
endif()

set(SOURCES
    ./src/log.c
    ./src/mem.c
)

add_library(cutils STATIC ${SOURCES})
target_include_directories(cutils PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_include_directories(cutils PRIVATE src)
target_compile_options(cutils PRIVATE ${CFLAGS})
