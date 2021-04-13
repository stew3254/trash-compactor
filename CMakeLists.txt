cmake_minimum_required(VERSION 3.17)
project(compiler C)

set(CMAKE_C_STANDARD 99)
set(SRC src)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

SET(FlexOutput ${SRC}/lex.yy.c)
IF(FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${FlexOutput}
            COMMAND ${FLEX_EXECUTABLE}
            -l
            ${SRC}/pc.l
            COMMENT "Generating lex.yy.l"
    )
ENDIF()

SET(BisonOutput ${SRC}/y.tab.c)
IF(BISON_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${BisonOutput}
            COMMAND ${BISON_EXECUTABLE}
            -dv
            ${SRC}/pc.y
            COMMENT "Generating y.tab.c"
    )
ENDIF()

file(GLOB SOURCES src/* src/data_structures/*)

ADD_LIBRARY(c ${BisonOutput} ${FlexOutput})

add_executable(compiler
        ${SOURCES}
        ${FlexOutput}
        ${BisonOutput}
)