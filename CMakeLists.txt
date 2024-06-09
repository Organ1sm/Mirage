cmake_minimum_required(VERSION 3.20)
project(Mirage
        VERSION 0.1.0
        LANGUAGES CXX)


include(FetchContent)
fetchcontent_declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG
        v3.4.0 # or a later release
)
fetchcontent_makeavailable(Catch2)

add_library(Mirage INTERFACE)
target_include_directories(Mirage INTERFACE ./include)
target_compile_features(Mirage INTERFACE cxx_std_17)

option(MIRAGE_BUILD_TEST "build test" OFF)

if (PROJECT_IS_TOP_LEVEL)
    set(Mi ON CACHE BOOL "build test" FORCE)
endif ()

if (MIRAGE_BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif ()