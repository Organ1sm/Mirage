macro(AddTest name)
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name} PRIVATE Mirage Catch2::Catch2WithMain)
    target_include_directories(${name} PRIVATE ../)
    add_test(NAME ${name}
            COMMAND $<TARGET_FILE:${name}>)
endmacro(AddTest)

add_subdirectory(Utility)
add_subdirectory(StaticRefl)