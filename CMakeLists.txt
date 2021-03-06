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
target_link_libraries(greedy_v2 circlesep)
target_compile_definitions(greedy_v2 PUBLIC -DC_SEP_GREEDY_2)

add_executable(greedy_v3 app/methods.cpp app/main_template.cpp)
target_link_libraries(greedy_v3 circlesep)
target_compile_definitions(greedy_v3 PUBLIC -DC_SEP_GREEDY_3)

add_executable(greedy_v3_tester app/methods.cpp app/test_template.cpp)
target_link_libraries(greedy_v3_tester circlesep)
target_compile_definitions(greedy_v3_tester PUBLIC -DC_SEP_GREEDY_3)

add_executable(reorder app/methods.cpp app/main_template.cpp)
target_compile_definitions(reorder PUBLIC -DC_SEP_REORDER)

add_executable(reorder_tester app/methods.cpp app/test_template.cpp)
target_link_libraries(reorder_tester circlesep)
target_compile_definitions(reorder_tester PUBLIC -DC_SEP_REORDER)

add_executable(exchange app/methods.cpp app/main_template.cpp)
target_link_libraries(exchange circlesep)
target_compile_definitions(exchange PUBLIC -DC_SEP_EXCHANGE)

add_executable(exchange_tester app/methods.cpp app/test_template.cpp)
target_link_libraries(exchange_tester circlesep)
target_compile_definitions(exchange_tester PUBLIC -DC_SEP_EXCHANGE)

add_executable(rep_v1 app/methods.cpp app/main_template.cpp)
target_link_libraries(rep_v1 circlesep)
target_compile_definitions(rep_v1 PUBLIC -DC_SEP_REP_1)

add_executable(rep_v2 app/methods.cpp app/main_template.cpp)
target_link_libraries(rep_v2 circlesep)
target_compile_definitions(rep_v2 PUBLIC -DC_SEP_REP_2)

add_executable(rep_v2_tester app/methods.cpp app/test_template.cpp)
target_link_libraries(rep_v2_tester circlesep)
target_compile_definitions(rep_v2_tester PUBLIC -DC_SEP_REP_2)

add_executable(rep_v3 app/methods.cpp app/main_template.cpp)
target_link_libraries(rep_v3 circlesep)
target_compile_definitions(rep_v3 PUBLIC -DC_SEP_REP_3)

add_executable(rep_v3_tester app/methods.cpp app/test_template.cpp)
target_link_libraries(rep_v3_tester circlesep)
target_compile_definitions(rep_v3_tester PUBLIC -DC_SEP_REP_3)

add_executable(rep_v4 app/methods.cpp app/main_template.cpp)
target_link_libraries(rep_v4 circlesep)
target_compile_definitions(rep_v4 PUBLIC -DC_SEP_REP_4)

add_executable(rep_v4_tester app/methods.cpp app/test_template.cpp)
target_link_libraries(rep_v4_tester circlesep)
target_compile_definitions(rep_v4_tester PUBLIC -DC_SEP_REP_4)

add_executable(sa_v2 app/methods.cpp app/main_template.cpp)
target_link_libraries(sa_v2 circlesep)
target_compile_definitions(sa_v2 PUBLIC -DC_SEP_SA_2)

add_executable(sa_v2_tester app/methods.cpp app/test_template.cpp)
target_link_libraries(sa_v2_tester circlesep)
target_compile_definitions(sa_v2_tester PUBLIC -DC_SEP_SA_2)

add_executable(sa_v3 app/methods.cpp app/main_template.cpp)
target_link_libraries(sa_v3 circlesep)
target_compile_definitions(sa_v3 PUBLIC -DC_SEP_SA_3)

add_executable(sa_v3_tester app/methods.cpp app/test_template.cpp)
target_link_libraries(sa_v3_tester circlesep)
target_compile_definitions(sa_v3_tester PUBLIC -DC_SEP_SA_3)

add_executable(hill_climb app/methods.cpp app/main_template.cpp)
target_link_libraries(hill_climb circlesep)
target_compile_definitions(hill_climb PUBLIC -DC_SEP_HILL_CLIMB)

add_executable(vlad_d app/methods.cpp app/main_template.cpp)
target_link_libraries(vlad_d circlesep)
target_compile_definitions(vlad_d PUBLIC -DC_SEP_VLAD_D)

# helpers

add_executable(opt app/opt.cpp)
target_link_libraries(opt circlesep ${ANT_LIBRARY} tbb)

add_executable(tester_method app/test_template.cpp)
target_link_libraries(tester_method ${ANT_LIBRARY} circlesep)

add_executable(tester_common app/test_template.cpp)
target_link_libraries(tester_common ${ANT_LIBRARY} circlesep)
target_compile_definitions(tester_common PUBLIC -DC_SEP_TESTER)

add_executable(stats app/state_stats.cpp)
target_link_libraries(stats circlesep ant tbb)