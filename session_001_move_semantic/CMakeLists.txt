set(EXAMPLES_LIST
    ex_001_01_the_problem 98
    ex_001_02_move_constructor 23
)

list(LENGTH EXAMPLES_LIST LEN)
math(EXPR NUM_EXAMPLES "${LEN} / 2 - 1")

foreach(I RANGE 0 ${NUM_EXAMPLES})
    math(EXPR IDX_NAME "${I} * 2")
    math(EXPR IDX_STD  "${I} * 2 + 1")

    list(GET EXAMPLES_LIST ${IDX_NAME} TARGET_NAME)
    list(GET EXAMPLES_LIST ${IDX_STD}  CXX_STANDARD_VALUE)

    add_executable(${TARGET_NAME} "examples/${TARGET_NAME}.cpp")
    set_target_properties(${TARGET_NAME} PROPERTIES
        CXX_STANDARD ${CXX_STANDARD_VALUE}
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
endforeach()