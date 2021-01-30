
cmake_minimum_required(VERSION 3.12)

project(bonjour-moderne VERSION 1.0.0)

add_library(bonjour-moderne STATIC)

target_sources(bonjour-moderne
PRIVATE
    include/bonjour_moderne.h
    source/bonjour_moderne.cpp
)

target_include_directories(bonjour-moderne
PUBLIC
    include
)

target_compile_features(bonjour-moderne
PUBLIC
    cxx_std_14
)

add_executable(bonjour-moderne-tests)

target_sources(bonjour-moderne-tests
PRIVATE
    tests/main.cpp
)

target_link_libraries(bonjour-moderne-tests
PRIVATE
    bonjour-moderne
)
