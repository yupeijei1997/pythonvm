cmake_minimum_required(VERSION 2.8)

PROJECT(PYTHONVM)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

ADD_EXECUTABLE(main main.cpp
    object/hiObject.cpp
    object/hiInteger.cpp
    object/hiString.cpp
    object/hiList.cpp
    object/klass.cpp
    util/arrayList.cpp
    code/codeObject.cpp
    code/binaryFileParser.cpp
    runtime/interpreter.cpp
    runtime/universe.cpp
    runtime/frameObject.cpp
    runtime/functionObject.cpp
    runtime/stringTable.cpp
    util/map.cpp)

INCLUDE_DIRECTORIES(./)