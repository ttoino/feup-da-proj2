cmake_minimum_required(VERSION 3.20)
project(da_proj2
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(da_proj2
               src/main.cpp
               src/ui.cpp
               src/utils.cpp
               src/dataset.cpp
               src/scenarios.cpp
               src/subscenarios.cpp
               src/graph.cpp)

add_executable(da_proj2_no_ansi
                src/main.cpp
                src/ui.cpp
                src/utils.cpp
                src/dataset.cpp
                src/scenarios.cpp
                src/subscenarios.cpp
                src/graph.cpp)
target_compile_definitions(da_proj2_no_ansi PUBLIC NO_ANSI)

include_directories(da_proj2 includes/)
include_directories(da_proj2_no_ansi includes/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/datasets
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/outputs)

# Doxygen Build
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(BUILD_DOC_DIR "${CMAKE_SOURCE_DIR}/docs")
    if(NOT EXISTS "${BUILD_DOC_DIR}")
        file(MAKE_DIRECTORY "${BUILD_DOC_DIR}")
    endif()

    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    configure_file("${DOXYGEN_IN}" "${DOXYGEN_OUT}" @ONLY)
    message("Doxygen build started") 
    add_custom_target(Doxygen ALL 
            COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUT}" 
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" 
            COMMENT "Generating API documentation with Doxygen" 
            VERBATIM) 
else(DOXYGEN_FOUND) 
    message("Doxygen needs to be installed to generate the documentation.") 
endif(DOXYGEN_FOUND)
