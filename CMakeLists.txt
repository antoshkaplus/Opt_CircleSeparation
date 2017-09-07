# cmake -DCMAKE_BUILD_TYPE=Debug
# cmake -DCMAKE_BUILD_TYPE=Release

cmake_minimum_required (VERSION 3.2)
project (CirclesSeparation)


list(APPEND CMAKE_CXX_FLAGS "-std=c++1y -Werror -Wall -Wno-sign-compare")


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(BinDir ${PROJECT_SOURCE_DIR}/bin)
set(LibDir ${PROJECT_SOURCE_DIR}/lib)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BinDir} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LibDir} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LibDir} )
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BinDir} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LibDir} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${LibDir} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

file(GLOB SOURCE_FILES "./src/*.cpp")
file(GLOB HEADER_FILES "./include/*.hpp")
file(GLOB TEST_FILES "./tester/*")


include_directories("./include")
# for ant library probably
include_directories("/usr/local/include")
include_directories("/usr/include")

find_library(ANT_LIBRARY ant)

add_library(circlesep STATIC ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(circlesep ${ANT_LIBRARY})


# submit

add_executable(v_1 submit/v_1.cpp app/main_template.cpp)
set_target_properties(v_1 PROPERTIES COMPILE_FLAGS "-Wno-sign-compare -Wno-maybe-uninitialized -Wno-unused-variable -Wno-error=unused-variable -Wno-error=unused-value -Wno-return-type -Wno-parentheses -Wno-error=parentheses")

# others

add_executable(ainu7 others/ainu7.cpp app/main_template.cpp)
set_target_properties(ainu7 PROPERTIES COMPILE_FLAGS "-Wno-sign-compare -Wno-maybe-uninitialized -Wno-unused-variable -Wno-error=unused-variable -Wno-error=unused-value -Wno-return-type -Wno-parentheses -Wno-error=parentheses")

add_executable(murrayr others/murrayr.cpp app/main_template.cpp)
set_target_properties(murrayr PROPERTIES COMPILE_FLAGS "-Wno-sign-compare -Wno-maybe-uninitialized -Wno-unused-variable -Wno-error=unused-variable -Wno-error=unused-value -Wno-return-type -Wno-parentheses -Wno-error=parentheses")

# methods

add_executable(greedy app/methods.cpp app/main_template.cpp)
target_compile_definitions(greedy PUBLIC -DC_SEP_GREEDY)

add_executable(greedy_tester app/methods.cpp app/test_template.cpp)
target_link_libraries(greedy_tester circlesep)
target_compile_definitions(greedy_tester PUBLIC -DC_SEP_GREEDY)

add_executable(greedy_v2 app/methods.cpp app/main_template.cpp)
target_compile_definitions(greedy_v2 PUBLIC -DC_SEP_GREEDY_2)

add_executable(rep_v1 app/methods.cpp app/main_template.cpp)
target_link_libraries(rep_v1 circlesep)
target_compile_definitions(rep_v1 PUBLIC -DC_SEP_REP_1)

# helpers

add_executable(opt app/opt.cpp)
target_link_libraries(opt circlesep ${ANT_LIBRARY} tbb)

add_executable(tester app/test_template.cpp)
target_link_libraries(tester
        ${ANT_LIBRARY}
        circlesep)

